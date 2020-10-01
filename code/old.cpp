//This program was written during the first round of the Swiss Olympiad in Informatics, based on the template provided by the SOI. It only works in the special case that all trains, lines and stations are available from the beginning.

//5329
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>

#define int long long

using namespace std;
struct Station;
struct Line;
struct Train;
struct Passenger;
struct Event;
struct trainArr;
struct outEvent;

struct Station {
  int idx, x, y, typ, li;
  priority_queue<Passenger> passengers;
};

struct Line {
  int idx;
  vector<int> st;
};

struct Train {
  int idx;
  vector<Passenger*> passengers;
};

struct Passenger {
  int idx, goal, dist, strt;
  Station* start = NULL;
  bool active = false;
  bool done = false;
};

struct Event {
  char typ;
  int time;
  int idx;
};

struct trainArr{
  Train* trn;
  int time;
  int st; // index of line in lines[0].st
};

struct outEvent{
  char typ; // r line, z train, e board, a unboard
  int time, idx, oid, pos; // time, thing id, id of other involved thing, position to place train
};
bool operator<(const Passenger &p1, const Passenger &p2){
  return p1.dist >= p2.dist;
}
bool operator<(const trainArr ta1, const trainArr ta2){
  int t1 = ta1.time;
  int t2 = ta2.time;
  return t1 >= t2;
}

int eventCount, lineChangeLimit, trainSize, stationMaxPassengers;
int stationCount = 0;
int lineCount = 0;
int trainCount = 0;
int passengerCount = 0;
Station* stations;
Line* lines;
Train* trains;
Passenger* passengers;
vector<Event> events;
vector<outEvent> outEvents;
priority_queue<trainArr> trainArrs;

void output(){
  for(outEvent ev : outEvents){
    cout << ev.time << " " << ev.typ << " " << ev.idx;
    switch (ev.typ)
    {
    case 'r':
      cout << " " << lines[ev.oid].st.size();
      for(int st : lines[ev.idx].st){
        cout << " " << st; 
      }
      cout << '\n';
      break;
    case 'z':
      cout << " " << ev.oid << " " << ev.pos << '\n';
      break;
    default:
      cout << " " << ev.oid << '\n';
    }
  }
}

int calc_dist(Passenger* p){
  int dist = 0;
  Station* cur_stat = p->start;
  while(cur_stat->typ != p->goal){
    dist ++;
    cur_stat = &stations[lines[0].st[(cur_stat->li + 1) % stationCount]];
  }
  return dist;
}

void simulate() {
  int pass_ok = 0;
  int active_pass = 0;
  while(pass_ok < passengerCount){
    //train arrival
    trainArr ta = trainArrs.top();
    trainArrs.pop();
    Station* stat = &stations[lines[0].st[ta.st]];
    Train* trn = ta.trn;
    //activate new passengers
    for(int i = 0; i < passengerCount; i ++){
      Passenger* p = &passengers[i];
      if(p->strt < ta.time && !p->done && !p->active){
        p->dist = calc_dist(p);
        active_pass ++;
        p->start->passengers.push(*p);
        p->active = true;
      }
    }
    //passengers get out
    for(int i = 0; i < trn->passengers.size(); i ++){
      Passenger* p = trn->passengers[i];
      if(p->goal == stat->typ){
        p->active = false;
        p->done = true;
        trn->passengers.erase(trn->passengers.begin() + i, trn->passengers.begin() + i + 1);
        pass_ok ++;
        active_pass --;
        outEvents.push_back({'a', ta.time, p->idx, stat->idx, -1});
      }
    }
    //passengers get in
    bool found = true;
    while(found && stat->passengers.size() > 0){
      found = false;
      if(trn->passengers.size() == trainSize) break;
      Passenger p = stat->passengers.top();
      if(stat->passengers.size() >= active_pass / stationCount /*&& (stat->idx == 3 || trn->passengers.size() < trainSize - 1)*/){
        stat->passengers.pop();
        trn->passengers.push_back(&passengers[p.idx]);
        found = true;
        outEvents.push_back({'e', ta.time, p.idx, trn->idx, -1});
      }
    }
    //schedule next arrival
    int nextst = (ta.st + 1)% stationCount;
    Station* nextstat = &stations[lines[0].st[nextst]];
    int x1 = stat->x;
    int y1 = stat->y;
    int x2 = nextstat->x;
    int y2 = nextstat->y;
    int dist = ceil(sqrtf(float((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))) / 12);
    trainArrs.push({trn, ta.time + dist + 1, nextst});
  }
}

void start_trains(){
  for(int i = 0; i < trainCount; i ++){
    outEvents.push_back({'z', 0, i, 0, stationCount / trainCount * i});
    trainArrs.push({&trains[i], 0, stationCount / trainCount * i});
  }
}

int dif(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    int dist12 = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    int dist34 = sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4));
    int dist13 = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
    int dist24 = sqrt((x2 - x4) * (x2 - x4) + (y2 - y4) * (y2 - y4));
    return dist13 + dist24 - dist12 - dist34;
}

vector<int> best_swap(vector<int>& line){
    int besti = -1, bestj = -1, bestdif = 0;
    for(int i = 0; i < line.size(); i ++){
        for(int j = 0; j < i; j ++){ // 1-2 3-4 -> 1-3 2-4
            pair<int, int> p1 = {stations[line[(j + stationCount - 1) % stationCount]].x, stations[line[(j + stationCount - 1) % stationCount]].y};
            pair<int, int> p2 = {stations[line[j]].x, stations[line[j]].y};
            pair<int, int> p3 = {stations[line[i]].x, stations[line[i]].y};
            pair<int, int> p4 = {stations[line[(i + 1) % stationCount]].x, stations[line[(i + 1) % stationCount]].y};
            if(j == 0 && i == stationCount - 1){
                continue;
            }
            int curdif = dif(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second, p4.first, p4.second);
            if(curdif < bestdif){
                bestdif = curdif;
                besti = i;
                bestj = j;
            }
        }
    }
    if(besti >= 0){
        vector<int> line2 = line;
        for(int i = bestj; i <= besti; i ++){
            line[i] = line2[besti - i + bestj];
        }
    }
    return line;
}

void find_line(){
    vector<int> line(stationCount);
    for(int i = 0; i < stationCount; i ++){line[i] = i;}
    for(int i = 0; i < 300; i ++){
        best_swap(line);
        //for(int i = 0; i < stationCount; i ++){cout << line[i] << " ";}
        //cout << '\n';
    }
    for(int i = 0; i < stationCount; i ++){
      stations[line[i]].li = i;
    }
    lines[0].st = line;
    outEvents.push_back({'r', 0, 0, 0, 0});
}

void readInput() {
  cin >> eventCount >> lineChangeLimit >> trainSize >> stationMaxPassengers >> stationCount >> passengerCount >> lineCount >> trainCount;

  stations = new Station[stationCount];
  lines = new Line[lineCount];
  trains = new Train[trainCount];
  passengers = new Passenger[passengerCount];

  for(int i = 0; i < eventCount; i++) {
    int aTime;
    char e;
    cin >> aTime >> e;

    switch(e) {
      case 's': {
        int i, x, y, a;
        cin >> i >> x >> y >> a;
        stations[i] = {
          .idx = i,
          .x = x,
          .y = y,
          .typ = a,
        };
        events.push_back({
          .typ = 's',
          .time = aTime,
          .idx = i
        });
        break;
      }
      case 'p': {
        int i, a, b;
        cin >> i >> a >> b;
        passengers[i] = {
          .idx = i,
          .goal = b,
          .start = &stations[a],
          .strt = aTime,
        };
        events.push_back({
          .typ = 'p',
          .time = aTime,
          .idx = i
        });
        break;
      }
      case 'l': {
        int i;
        cin >> i;
        lines[i] = {
          .idx = i,
        };
        events.push_back({
          .typ = 'l',
          .time = aTime,
          .idx = i
        });
        break;
      }
      case 'z': {
        int i;
        cin >> i;
        trains[i] = {
          .idx = i,
        };
        events.push_back({
          .typ = 'z',
          .time = aTime,
          .idx = i
        });
        break;
      }
    }
  }
}

signed main() {
  readInput();
  find_line();
  start_trains();
  simulate();
  output();
  return 0;
}
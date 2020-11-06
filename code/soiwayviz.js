/* JS File for visualization, provided by the Swiss Olympiad in Informatics, written by Benjamin Schmid */

'use strict';

var soiwayviz = {
  input: "",
  output: "",
  now: 0,
  frames: [],
  playSpeed: 10,
  playing: false,
  lastFrame: null,
};

function openFullscreen(elem) {
  if (elem.requestFullscreen) {
    elem.requestFullscreen();
  } else if (elem.mozRequestFullScreen) { /* Firefox */
    elem.mozRequestFullScreen();
  } else if (elem.webkitRequestFullscreen) { /* Chrome, Safari and Opera */
    elem.webkitRequestFullscreen();
  } else if (elem.msRequestFullscreen) { /* IE/Edge */
    elem.msRequestFullscreen();
  }
}

function soiwayviz_ready() {
  window.addEventListener('resize', soiwayviz_canvassize);
  soiwayviz_canvassize();

  document.addEventListener('keydown', function(event) {
    if (event.key == ' ' || event.key == '.') {
      togglePlay();
    } else if (event.key == 'ArrowLeft' || event.key == 'PageUp') {
      soiwayviz.now--;
      soiwayviz_draw();
    } else if (event.key == 'ArrowRight' || event.key == 'PageDown') {
      soiwayviz.now++;
      soiwayviz_draw();
    } else if (event.key == 'ArrowUp') {
      if (event.shiftKey) {
        soiwayviz.playSpeed += 1;
      } else {
        soiwayviz.playSpeed += 0.1;
      }
      document.getElementById('soiwayviz_speedinput').value = Math.round(soiwayviz.playSpeed*10)/10;
    } else if (event.key == 'ArrowDown') {
      if (event.shiftKey) {
        soiwayviz.playSpeed -= 1;
      } else {
        soiwayviz.playSpeed -= 0.1;
      }
      document.getElementById('soiwayviz_speedinput').value = Math.round(soiwayviz.playSpeed*10)/10;
    }
  });
}

function soiwayviz_canvassize() {
  var canvas = document.getElementById('soiwayviz_canvas');
  canvas.width = canvas.parentElement.clientWidth;
  canvas.height = canvas.parentElement.clientHeight;
  soiwayviz_draw();
}

function soiwayviz_fullscreen() {
  openFullscreen(document.getElementById('soiwayviz_vizwrapper'));
}

function soiwayviz_loadData(ev) {
  ev.preventDefault();
  var input = document.getElementById('soiwayviz_input_file');
  var output = document.getElementById('soiwayviz_output_file');
  if (input.files.length == 0 || output.files.length == 0) {
    if (typeof(debug_input) !== 'undefined' && typeof(debug_output) !== 'undefined') {
      soiwayviz.input = debug_input;
      soiwayviz.output = debug_output;
      soiwayviz_calc_frames();
      soiwayviz_draw();
      return false;
    }
    alert('Missing input');
    return false;
  }
  var readerInput = new FileReader();
  readerInput.onload = function() {
    soiwayviz.input = readerInput.result;
    var readerOutput = new FileReader();
    readerOutput.onload = function() {
      soiwayviz.output = readerOutput.result;
      soiwayviz_calc_frames();
      soiwayviz_draw();
    }
    readerOutput.readAsText(output.files[0]);
  }
  readerInput.readAsText(input.files[0]);
  return false;
}

function soiwayviz_calc_frames() {
  if (!soiwayviz.input || !soiwayviz.output) {
    alert('Missing input');
    return;
  }
  soiwayviz.frames = [];
  soiwayviz.current_frame = 0;
  var eventLinesOrig = soiwayviz.input.match(/[^\r\n]+/g);
  soiwayviz.consts = eventLinesOrig[0].split(" ").map(function (a) {
    return parseInt(a, 10);
  });
  var consts = eventLinesOrig[0].split(" ").map(function(a) {
    return parseInt(a, 10);
  });
  var eventLines = eventLinesOrig.slice(1).map(function (line) {
    return line.split(" ").map(function (a, idx) {
      return idx == 1 ? a : parseInt(a, 10);
    });
  });
  var actionLines = soiwayviz.output.match(/[^\r\n]+/g).map(function (line) {
    if (line.split(" ")[1] == 'd') {
      return [parseInt(line.split(" ")[0]), 'd', line];
    } else {
      return line.split(" ").map(function (a, idx) {
        return idx == 1 ? a : parseInt(a, 10);
      });
    }
  });

  var stations = [];
  var lines = [];
  var trains = [];
  var passengers = [];
  var errors = [];
  var now = 0;
  var aevent = 0;
  var aaction = 0;

  function boom(msg) {
    if (errors.length < 1000) {
      errors.push(now + ": " + msg);
      console.error(now + ": " + msg);
    }
  }

  while(true) {
    while (aevent < eventLines.length && eventLines[aevent][0] == now) {
      var ev = eventLines[aevent];
      switch(ev[1]) {
        case 's':
          stations.push({
            idx: ev[2],
            x: ev[3],
            y: ev[4],
            typ: ev[5],
            passengers: [],
          });
          break;
        case 'p':
          passengers.push({
            idx: ev[2],
            station: stations[ev[3]],
            train: null,
            goal: ev[4],
          });
          stations[ev[3]].passengers.push(passengers[passengers.length-1]);
          if (stations[ev[3]].passengers.length > soiwayviz.consts[3]) {
            boom("Station " + ev[3] + " is too full");
          }
          break;
        case 'l':
          lines.push({
            idx: ev[2],
            route: [],
            trains: [],
          });
          break;
        case 'z':
          trains.push({
            idx: ev[2],
            line: null,
            stationIdx: 0,
            oldStation: null,
            newStation: null,
            departure: 0,
            arrival: 0,
            passengers: [],
            lastChange: -1e9,
          });
          break;
      }
      aevent++;
    }

    while (aaction < actionLines.length && actionLines[aaction][0] == now) {
      var ac = actionLines[aaction];
      switch(ac[1]) {
        case 'r':
          if (lines[ac[2]].trains.length > 0) {
            boom("Line " + ac[2] + " is not empty. Route can not be changed.");
          }
          if (ac[3] != ac.length - 4) {
            boom("Length of new route is not correct for line " + ac[2]);
          }
          lines[ac[2]].route = ac.slice(4).map(function (idx) {
            return stations[idx];
          });
          if(lines[ac[2]].route.length > 2*consts[0]) {
            boom("Length of new route is too long for line " + ac[2]);
          }
          break;
        case 'z':
          var train = trains[ac[2]];
          if (train.line) {
            if (now < train.lastChange + soiwayviz.consts[1]) {
              boom("Train " + ac[2] + " changed too often. Last change was at " + train.lastChange + ", now is " + now);
            }
            train.line.trains.splice(train.line.trains.indexOf(train), 1);
          }
          if (ac[3] == -1) {
            train.line = null;
          } else {
            train.line = lines[ac[3]];
            train.line.trains.push(train);
            train.stationIdx = ac[4];
            if (train.stationIdx < 0 || train.stationIdx >= train.line.route.length) {
              boom("Station at index " + ac[3] + " is not on route of line " < ac[3]);
              break;
            }
            train.departure = now;
            train.arrival = now;
            train.oldStation = train.line.route[train.stationIdx];
            train.newStation = train.line.route[train.stationIdx];
            train.lastChange = now;
          }
          break;
        case 'e':
          var pass = passengers[ac[2]];
          if (!pass.station) {
            boom("Passenger " + ac[2] + " is not in a station");
          }
          if (!trains[ac[3]].line) {
            boom("Train " + ac[3] + " is currently not on a line");
          }
          if (trains[ac[3]].newStation != pass.station) {
            boom("Train " + ac[3] + " is currently not in station " + pass.station.idx);
          }
          if (trains[ac[3]].arrival != now) {
            boom("Train " + ac[3] + " is currently not in a station");
          }
          if (trains[ac[3]].passengers.length >= soiwayviz.consts[2]) {
            boom("Train " + ac[3] + " is already full");
          }
          pass.station.passengers.splice(pass.station.passengers.indexOf(pass), 1);
          pass.station = null;
          pass.train = trains[ac[3]];
          pass.train.passengers.push(pass);
          break;
        case 'a':
          var pass = passengers[ac[2]];
          if (!pass.train) {
            boom("Passenger " + ac[2] + " is not in a train");
          }
          if (pass.train.arrival != now) {
            boom("Train " + pass.train.idx + " is currently not in a station");
          }
          if (pass.train.newStation != stations[ac[3]]) {
            boom("Train " + pass.train.idx + " is currently not in station " + ac[3]);
          }
          pass.train.passengers.splice(pass.train.passengers.indexOf(pass), 1);
          pass.train = null;
          if (stations[ac[3]].typ != pass.goal) {
            pass.station = stations[ac[3]];
            pass.station.passengers.push(pass);
            if (pass.station.passengers.length > soiwayviz.consts[3]) {
              boom("Station " + ac[3] + " is too full " + pass.idx);
              for(var i = 0; i < pass.station.passengers.length; i ++)
              {
                boom(pass.station.passengers[i].idx);
              }
            }
          }
          break;
        case 'd':
          errors.push(ac[0] + ": " + ac[2].slice(ac[2].indexOf('d')+2));
          break;
      }
      aaction++;
    }

    for (var idx in trains) {
      var train = trains[idx];
      if (train.line && now == train.arrival && train.line.route.length > 1) {
        train.oldStation = train.line.route[train.stationIdx];
        train.stationIdx++;
        train.stationIdx %= train.line.route.length;
        train.newStation = train.line.route[train.stationIdx];
        var dx = train.newStation.x - train.oldStation.x;
        var dy = train.newStation.y - train.oldStation.y;
        var dist = Math.ceil(Math.sqrt(dx*dx + dy*dy)/12);
        train.departure = now + 1;
        train.arrival = now + 1 + dist;
      }
    }

    for (var idx in stations) {
      if (stations[idx].passengers.length > soiwayviz.consts[3]) {
        boom("Station " + idx + " is too full");
      }
    }

    var frame = {
      now: now,
      sizex: 0,
      sizey: 0,
      stations: [],
      lines: [],
      trains: [],
      errors: errors.slice(),
    };

    for (var idx in stations) {
      var station = stations[idx];
      var passs = [];
      for (var pidx in station.passengers) {
        passs.push({
          goal: station.passengers[pidx].goal,
        });
      }
      frame.sizex = Math.max(frame.sizex, Math.abs(station.x));
      frame.sizey = Math.max(frame.sizey, Math.abs(station.y));
      frame.stations.push({
        x: station.x,
        y: station.y,
        typ: station.typ,
        passengers: passs,
      });
    }

    for (var idx in lines) {
      var route = lines[idx].route;
      var points = [];
      for (var ridx in route) {
        points.push({
          x: route[ridx].x,
          y: route[ridx].y,
        });
      }
      frame.lines.push({
        idx: idx,
        points: points,
      });
    }

    for (var idx in trains) {
      var train = trains[idx];
      if (!train.line || !train.newStation || !train.oldStation) {
        frame.trains.push({
          visible: false,
        });
      } else {
        var passs = [];
        for (var pidx in train.passengers) {
          passs.push({
            goal: train.passengers[pidx].goal,
          });
        }
        var progress = (now - train.departure) / (train.arrival - train.departure);
        if (isNaN(progress)) {
          progress = 0;
        }
        progress = Math.max(0, progress);
        var rotation = Math.atan2(train.newStation.y - train.oldStation.y, train.newStation.x - train.oldStation.x);
        if (now == train.departure - 1 && now > 0 && soiwayviz.frames[soiwayviz.frames.length-1].trains[idx]) {
          rotation = soiwayviz.frames[soiwayviz.frames.length-1].trains[idx].rotation;
        }
        frame.trains.push({
          visible: true,
          lineIdx: train.line.idx,
          x: progress * train.newStation.x + (1-progress) * train.oldStation.x,
          y: progress * train.newStation.y + (1-progress) * train.oldStation.y,
          passengers: passs,
          rotation: rotation,
          departure: train.departure,
        });
      }
    }

    soiwayviz.frames.push(frame);

    if (aevent >= eventLines.length && aaction >= actionLines.length) {
      break;
    }

    if (soiwayviz.frames.length > 30000) {
      //alert('The simulation is too long for the visualization.');
      //break;
    }

    now++;
    var minpos = 0x3fffffff;
    if (aevent < eventLines.length) {
      minpos = Math.min(minpos, eventLines[aevent][0]);
    }
    if (aaction < actionLines.length) {
      minpos = Math.min(minpos, actionLines[aaction][0]);
    }
    for (var idx in trains) {
      if (trains[idx].arrival >= now) {
        minpos = Math.min(minpos, trains[idx].arrival);
      }
      if (trains[idx].departure >= now) {
        minpos = Math.min(minpos, trains[idx].departure);
      }
    }
    now = Math.max(now, minpos);
  }
  soiwayviz.now = 0;
  document.getElementById('soiwayviz_nowinput').value = 0;
}

function soiwayviz_draw() {
  var canvas = document.getElementById('soiwayviz_canvas');
  var ctx = canvas.getContext('2d');
  ctx.save();
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.fillStyle = 'white';
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  ctx.beginPath();
  ctx.translate(canvas.width/2, canvas.height/2);

  if (!soiwayviz.frames.length) {
    ctx.restore();
    return;
  }
  var oldFr = null, newFr = null;
  for(var idx in soiwayviz.frames) {
    if (soiwayviz.frames[idx].now >= soiwayviz.now) {
      newFr = soiwayviz.frames[idx];
      if (idx > 0) {
        oldFr = soiwayviz.frames[idx-1];
      } else {
        oldFr = newFr;
      }
      break;
    }
  }
  if (!oldFr || !newFr) {
    newFr = oldFr = soiwayviz.frames[soiwayviz.frames.length - 1];
  }
  var progress = (newFr.now != oldFr.now) ? (soiwayviz.now - oldFr.now) / (newFr.now - oldFr.now) : 0;

  function interp(v1, v2) {
    return progress * v2 + (1-progress) * v1;
  }

  var sizex = interp(oldFr.sizex, newFr.sizex) + 40;
  var sizey = interp(oldFr.sizey, newFr.sizey) + 40;
  var scale = Math.min(10, Math.min(canvas.width / (2*sizex), canvas.height / (2*sizey)));
  ctx.scale(scale, scale);

  var forms = [
    function() {
      ctx.fillRect(-6, -6, 12, 12);
      ctx.strokeRect(-6, -6, 12, 12);
    },
    function() {
      ctx.arc(0, 0, 8, 0, 2*Math.PI);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.moveTo(-8, 4);
      ctx.lineTo(8, 4);
      ctx.lineTo(0, -8);
      ctx.lineTo(-8, 4);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.arc(0, 0, 8, Math.PI/2, 3*Math.PI/2);
      ctx.moveTo(0, -8);
      ctx.lineTo(0, 8);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.arc(0, 0, 8, 3*Math.PI/2, 5*Math.PI/2);
      ctx.moveTo(0, -8);
      ctx.lineTo(0, 8);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.arc(0, 0, 8, 0, Math.PI);
      ctx.moveTo(8, 0);
      ctx.lineTo(0, -8);
      ctx.lineTo(-8, 0);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.moveTo(-8, 4);
      ctx.lineTo(-8, -4);
      ctx.lineTo(-4, -4);
      ctx.lineTo(-4, -8);
      ctx.lineTo(4, -8);
      ctx.lineTo(4, -4);
      ctx.lineTo(8, -4);
      ctx.lineTo(8, 4);
      ctx.lineTo(4, 4);
      ctx.lineTo(4, 8);
      ctx.lineTo(-4, 8);
      ctx.lineTo(-4, 4);
      ctx.lineTo(-8, 4);
      ctx.lineTo(-8, -4);
      ctx.fill();
      ctx.stroke();
    },
    function() {
      ctx.save();
      ctx.rotate(Math.PI/4);
      ctx.fillRect(-6, -6, 12, 12);
      ctx.strokeRect(-6, -6, 12, 12);
      ctx.restore();
    },
  ];

  var lineColors = [
    '#FF0000',
    '#00FF00',
    '#0000FF',
    '#70a1ef',
    '#9d38e0',
    '#216805',
    '#f2c202',
    '#ef17e4',
    '#a5460b',
    '#991a59',
  ]
  function getLineCol(idx) {
    if (idx < lineColors.length) {
      return lineColors[idx];
    }
    return '#AAAAAA';
  }

  function drawForm(form, x, y, rotation, scale) {
    ctx.save();
    ctx.translate(x, y);
    ctx.scale(scale, scale);
    ctx.fillStyle = '#FFFFFF';
    ctx.strokeStyle = '#000000';
    ctx.lineWidth = 0.5;
    ctx.beginPath();
    if (form < forms.length) {
      forms[form]();
    } else {
      ctx.arc(0, 0, 8, 0, Math.PI);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(0, 0, 8, Math.PI, 2*Math.PI);
      ctx.fill();
      ctx.stroke();
      ctx.beginPath();
      ctx.fillStyle = '#000000';
      ctx.fillText(""+form, form < 10 ? -3 : -5.5, 4);
    }
    ctx.restore();
  }

  ctx.save();
  ctx.translate(-oldFr.lines.length, -oldFr.lines.length);
  for(var idx in oldFr.lines) {
    var pts = oldFr.lines[idx].points;
    if (pts.length == 0) {
      continue;
    }
    ctx.beginPath();
    ctx.strokeStyle = getLineCol(idx);
    ctx.lineWidth = 4;
    ctx.lineJoin = "round";
    ctx.lineCap = "round";
    ctx.moveTo(pts[pts.length-1].x, pts[pts.length-1].y);
    for(var i in pts) {
      ctx.lineTo(pts[i].x, pts[i].y);
    }
    ctx.stroke();
    ctx.translate(2, 2);
  }
  ctx.restore();

  for(var idx in oldFr.stations) {
    var ost = oldFr.stations[idx];
    drawForm(ost.typ, ost.x, ost.y, 0, 1);

    if (ost.passengers.length > soiwayviz.consts[3]) {
      for (var i = 5; i >= 1; i--) {
        drawForm(ost.typ, ost.x, ost.y, 0, i);
      }
    }

    for(var pidx in ost.passengers) {
      var pas = ost.passengers[pidx];
      drawForm(pas.goal, ost.x + 14 + pidx * 10, ost.y, 0, 0.7);
    }
  }
  ctx.beginPath();

  for(var idx in oldFr.trains) {
    var ot = oldFr.trains[idx];
    if (!ot.visible) {
      continue;
    }
    var nt = newFr.trains[idx];
    ctx.save();
    ctx.translate(-oldFr.lines.length + ot.lineIdx * 2, -oldFr.lines.length + ot.lineIdx * 2);
    ctx.translate(interp(ot.x, nt.x), interp(ot.y, nt.y));
    if (ot.departure > soiwayviz.now) {
      var myProgress = Math.min(1, ot.departure - soiwayviz.now);
      var newRot = nt.rotation;
      if (newRot - ot.rotation > Math.PI) {
        newRot -= Math.PI*2;
      } else if (ot.rotation - newRot > Math.PI) {
        newRot += Math.PI*2;
      }
      var rotation = (myProgress * ot.rotation + (1-myProgress) * newRot);
      ctx.rotate(rotation - Math.PI/2);
    } else {
      ctx.rotate(ot.rotation - Math.PI/2);
    }
    ctx.beginPath();
    ctx.fillStyle = getLineCol(ot.lineIdx);
    ctx.fillRect(-12, -24, 24, 40);
    ctx.moveTo(-12, 16);
    ctx.lineTo(0, 30);
    ctx.lineTo(12, 16);
    ctx.fill();
    ctx.restore();
  }
  for(var idx in oldFr.trains) {
    var ot = oldFr.trains[idx];
    if (!ot.visible) {
      continue;
    }
    var nt = newFr.trains[idx];
    ctx.save();
    ctx.translate(-oldFr.lines.length + ot.lineIdx * 2, -oldFr.lines.length + ot.lineIdx * 2);
    ctx.translate(interp(ot.x, nt.x), interp(ot.y, nt.y));
    if (ot.departure > soiwayviz.now) {
      var myProgress = Math.min(1, ot.departure - soiwayviz.now);
      var newRot = nt.rotation;
      if (newRot - ot.rotation > Math.PI) {
        newRot -= Math.PI*2;
      } else if (ot.rotation - newRot > Math.PI) {
        newRot += Math.PI*2;
      }
      var rotation = (myProgress * ot.rotation + (1-myProgress) * newRot);
      ctx.rotate(rotation - Math.PI/2);
    } else {
      ctx.rotate(ot.rotation - Math.PI/2);
    }
    ctx.beginPath();
    for(var pidx in ot.passengers) {
      var col = pidx % 3;
      var row = (pidx-col) / 3;
      var pas = ot.passengers[pidx];
      drawForm(pas.goal, (col - 1) * -12, (row - 1) * -12, 0, 0.7);
    }
    ctx.restore();
  }

  ctx.restore();

  ctx.save();
  var errCnt = Math.min(20, oldFr.errors.length);
  for (var i = oldFr.errors.length - errCnt; i < oldFr.errors.length; i++) {
    ctx.fillText(oldFr.errors[i], 20, 20);
    ctx.translate(0, 20);
  }
  ctx.restore();

  document.getElementById('soiwayviz_nowinput').value = Math.round(soiwayviz.now*10)/10;
}

function soiwayviz_setnow(ev) {
  soiwayviz.now = parseInt(ev.target.value, 10);
  soiwayviz_draw();
}

function soiwayviz_setspeed(ev) {
  soiwayviz.playSpeed = parseInt(ev.target.value, 10);
}

function soiwayviz_vizstep() {
  var now = new Date().getTime();
  var elapsed = soiwayviz.lastFrame ? now - soiwayviz.lastFrame : 0;
  elapsed = Math.min(elapsed, 1000);
  soiwayviz.lastFrame = now;
  soiwayviz.now += soiwayviz.playSpeed * elapsed / 1000;
  soiwayviz_draw();
  if (soiwayviz.playing) {
    window.requestAnimationFrame(soiwayviz_vizstep);
  } else {
    soiwayviz.lastFrame = null;
  }
}

function togglePlay() {
  soiwayviz.playing = !soiwayviz.playing;
  if (soiwayviz.playing) {
    soiwayviz_vizstep();
  }
}

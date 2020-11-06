------------ GENERAL INFORMATION ------------


Git repository for my matura project. 

The idea was to use simulated annealing and genetic algorithms to optimize a train network. 

code: here you can find all the code that was written for the project as well as the visualization of the train network

latex: everything related to the written part of the project


------------ RUNNING THE PROGRAMS ------------



Requirements: g++ compiler

(1) Create an input file

	(a) Go to the directory where 'input_generator.cpp' is stored
	(b) Run 'g++ input_generator.cpp -o input_generator' in the terminal
	(c) Run './input_generator num > input.txt' in the terminal
		('num' could be any positive integer)
	Now a file named 'input.txt' has been created in the directory.


(2) Create an output file

	(a) Go to the directory where 'program.cpp' and 'input.txt' are stored
	(b) Run 'g++ program.cpp -o program' in the terminal
	(c) Run './program < input.txt > output.txt' in the terminal
	This will create a file called 'output.txt' in the directory.


(3) Run simulation

	(a) Go to the directory where 'soiwayviz.html', 'soiwayviz.js' and 'soiwayviz.css' are stored
	(b) Open 'soiwayviz.html' in a browser
	(c) Choose 'input.txt' for the Input and 'output.txt' for the output in the visualization
	(d) Click 'Load' to load the simulation
	(e) Click 'Play/Pause' to start the simulation.

	Now the simulation will run.
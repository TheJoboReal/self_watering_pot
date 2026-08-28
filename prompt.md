### Intro 
This project is about a pico pi w controlling a small water pump and reading from a moisture sensor. The project is low power focused and uses picos dormant mode along with an external wake up call.

### Setup 
The project is structure with one main.c file and an inc/ folder containing .h files. the project does not use .c files for declarations, only .h to keep it simple.

In the examples/ folder a lot of pico pi examples can be found, these examples are used heavily in this project and should be used any time you are confused or in doubt about implementation.

### Goal
The pico should wake up, sense on the moisture sensor, drive the pump and then go back to sleep. In the furture the pico shall also read/write data to an SD card along with accessing wifi to get the time and date for the SD logging.

### TODO 

1. Change the current scheduler to a run to complete scheduler that does not use time.
2. Fix dormant mode to go to sleep correctly after the scheduler has run and make sure it wakes up correctly to then run the scheduler again.

### Tips 
Build the project with ```cmake --build build```.
You shall only fix the todo's, the goal is only for the context of the project.
Refer to the prompt.md for this file.

### Intro
This project is for an elective self study course on SDU and concerns a very low powered self watering plant pot, using a pico pi. The theoretical battery life based on the component datasheets is approx 3 months on an 1800mAh lithium battery.

### Code
All code is written in C and uses a simple run to complete scheduler.

It can be compiled with:

```bash
mkdir build 
cd build 
cmake ..
cd ..
```

and build with:

```bash
cmake --build build
```

### PCB
PCB is designed in FreeCAD and meant to house all the components used in the project. It is recommended to use sockets for the pin modules to enable quick swap of possible damaged components(Lesson learned the hard way..).

### TODO
* Implement SD card logging.
* Replace burnt Eink and make UI interactable with encoder.
* Add soldering pad for an esp32 to allow choice between pico pi and esp.


### Acknowledgements 
CAD design is derived from the CAD work of Martin [here](https://www.thingiverse.com/thing:4921885).

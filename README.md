## Introduction

[![Join the chat at https://gitter.im/koppi/stl2ngc](https://badges.gitter.im/koppi/stl2ngc.svg)](https://gitter.im/koppi/stl2ngc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

stl2ngc converts an [STL](https://en.wikipedia.org/wiki/STL_(file_format)) file to [LinuxCNC](http://linuxcnc.org/) compatible [G-Code](http://linuxcnc.org/docs/html/gcode.html).

### Build, install and run stl2ngc

First, install OpenCAMLib, (see: https://github.com/aewallin/opencamlib ):
```bash
sudo apt -y install cmake doxygen libboost-all-dev
git clone https://github.com/aewallin/opencamlib && cd opencamlib
mkdir build && cd build && cmake ../src && make -j4
sudo make install
sudo cp libocl.so.* /usr/lib
```

Next, checkout stl2ngc from git and run make and sudo make install:
```bash
git clone https://github.com/koppi/stl2ngc && cd stl2ngc
make
sudo make install
```

To convert ```file.stl``` to ```file.ngc``` run:
```bash
cat file.stl | stl2ngc > file.ngc
```

### Demo

See [Wiki](../../wiki/).

### FAQ

* This is an early release, expect errors and missing features.
  
  Please [fork](https://github.com/koppi/stl2ngc/network) the repo.

* No Gui? - Yes, only command-line for now.

### Authors

* [Jakob Flierl](https://github.com/koppi)


## Introduction

stl2ngc converts an [STL](https://en.wikipedia.org/wiki/STL_(file_format)) file to [LinuxCNC](http://linuxcnc.org/) compatible [G-Code](http://linuxcnc.org/docs/html/gcode.html).

This is an early release, expect errors and missing features.

### Build, install and run stl2ngc

First, install OpenCAMLib, see: https://github.com/aewallin/opencamlib
```bash
git clone https://github.com/aewallin/opencamlib && cd opencamlib
mkdir build && cd build && cmake ../src && make -j4
sudo make install
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

### Authors

* https://github.com/koppi

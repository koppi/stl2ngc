# stl2ngc

The ```stl2ngc``` program converts an [STL](https://en.wikipedia.org/wiki/STL_(file_format)) file to [LinuxCNC](http://linuxcnc.org/) compatible [G-code](http://linuxcnc.org/docs/html/gcode.html).

## Clone, build, install and run

First, install OpenCAMLib, see: https://github.com/aewallin/opencamlib#building-for-c
```bash
git clone https://github.com/aewallin/opencamlib
cd opencamlib
mkdir build
cd build
cmake .. -D CXX_LIB="ON"
make . # try make -j4 for a faster build if you have a multi-core machine
make install .
```

or use a prebuilt release:

```bash
cd Downloads/
wget https://github.com/aewallin/opencamlib/releases/download/2023.01.11/linux-cxx-x86_64.zip
unzip linux-cxx-x86_64.zip && sudo cp -rpv prebuilds/linux-cxx-x86_64/* /usr
```
and as root (```su -```):

```bash
echo '/usr/lib/opencamlib' > /etc/ld.so.conf.d/opencamlib.conf
ldconfig
```

Next, checkout ```stl2ngc``` from git and run ```make``` and ```sudo make install```:
```bash
git clone https://github.com/koppi/stl2ngc && cd stl2ngc
make
sudo make install
```

To convert ```example.stl``` to ```example.ngc``` run:
```bash
cat example.stl | stl2ngc > example.ngc
```

## Demo

See [Wiki](../../wiki/).

## FAQ

* This is an early release, expect errors and missing features.
  
  Please [fork](https://github.com/koppi/stl2ngc/network) the repo.

* No Gui? - Yes, only command-line for now.

## Author

* **Jakob Flierl** - [koppi](https://github.com/koppi)

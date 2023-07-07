CXXFLAGS+=-Wall -O3 -fopenmp -std=gnu++11
LDFLAGS +=-L/usr/lib/opencamlib
LDLIBS  +=-locl

all: stl2ngc
	strip stl2ngc
#	upx -9 stl2ngc

install: stl2ngc
	install -o root -g root stl2ngc /usr/bin

clean:
	/bin/rm -f stl2ngc

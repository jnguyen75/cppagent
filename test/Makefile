# CppUnit-Tutorial
# file: makefile# next line has to be changed to the installation-path of CppUnit

INCLUDE_DIRECTORIES=/usr/local/include/libxml++-2.6 \
	/usr/local/lib/libxml++-2.6/include \
	/usr/lib/glibmm-2.4 /usr/include/glibmm-2.4/ \
	/usr/lib/glibmm-2.4/include \
	/usr/include/glib-2.0 /usr/lib/glib-2.0/include \
	/usr/local/include ../lib ../devices

CXXFLAGS = -g -Wall $(INCLUDE_DIRECTORIES:%=-I%) -DDLIB_NO_GUI_SUPPORT
LDFLAGS = $(LIB_DIRECTORIES:%=-L%)
LIBRARIES=  xml++-2.6 xml2 pthread z m glibmm-2.4 gobject-2.0 sigc-2.0 glib-2.0

CPPUNIT_PATH=../lib/cppunit-1.12.1

SOURCES = \
axes.

LIBS=$(LIBRARIES:%=-l%)

test: test.o axes.o component.o axes_test.o globals.o
	g++ ${CXXFLAGS} ${LDFLAGS} -o test test.o axes.o component.o axes_test.o globals.o -L${CPPUNIT_PATH}/lib -lstdc++ -lcppunit -ldl ${LIBS}

axes.o: ../devices/axes.cpp component.o
	g++ -c ${CXXFLAGS} ../devices/axes.cpp

#device.o: ../devices/device.cpp
#	g++ -c ${CXXFLAGS} ../devices/device.cpp

component.o: ../devices/component.cpp
	g++ -c ${CXXFLAGS} ../devices/component.cpp

globals.o: ../agent/globals.cpp
	g++ -c ${CXXFLAGS} ../agent/globals.cpp

axes_test.o: devices/axes_test.cpp
	g++ -c -I${CPPUNIT_PATH}/include devices/axes_test.cpp

test.o: test.cpp
	g++ -c test.cpp -I${CPPUNIT_PATH}/include

clean:
	rm -f *.o test


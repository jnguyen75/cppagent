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

#vpath %.hpp agent
#vpath %.cpp agent
#vpath %.hpp devices
#vpath %.cpp devices
#vpath %.hpp ../agent/
#vpath %.hpp ../devices/

SOURCES= \
../agent/globals.cpp \
test.cpp \
devices/component_test.cpp


OBJS=$(SOURCES:%.cpp=%.o)
LIBS=$(LIBRARIES:%=-l%)

all: test

test: test.o component_test.o
	#g++ -o test test.o adapter_test.o -L${CPPUNIT_PATH}/lib -lstdc++ -lcppunit -ldl

%.o: %.cpp %.hpp
	@echo Compiling $<
	@g++ -c -I${CPPUNIT_PATH}/include ${CXXFLAGS} $<

#adapter.o: ../agent/adapter.cpp
#	g++ -c ${CXXFLAGS} ../agent/adapter.cpp

#adapter_test.o: agent/adapter_test.cpp
#	g++ -c agent/adapter_test.cpp -I${CPPUNIT_PATH}/include

#component_test.o: devices/component_test.cpp
#	g++ -c devices/component_test.cpp

#test.o: test.cpp
#	g++ -c -I${CPPUNIT_PATH}/includetest.cpp -I${CPPUNIT_PATH}/include

clean:
	rm -f *.o test


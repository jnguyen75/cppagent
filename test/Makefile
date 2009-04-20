
INCLUDE_DIRECTORIES=/usr/local/include/libxml++-2.6 \
	/usr/local/lib/libxml++-2.6/include \
	/usr/lib/glibmm-2.4 /usr/include/glibmm-2.4/ \
	/usr/lib/glibmm-2.4/include \
	/usr/include/glib-2.0 /usr/lib/glib-2.0/include \
	/usr/local/include ../lib ../devices \
	../lib/cppunit-1.12.1/include

#INCLUDE_DIRECTORIES=/opt/local/include/libxml++-2.6 \
#	/opt/local/lib/libxml++-2.6/include /opt/local/include/libxml2 \
#	/opt/local/include/glibmm-2.4 /opt/local/lib/glibmm-2.4/include \
#	/opt/local/include/sigc++-2.0 /opt/local/lib/sigc++-2.0/include \
#	/opt/local/include/glib-2.0 /opt/local/lib/glib-2.0/include \
#	/opt/local/include ../lib ../devices

LIB_DIRECTORIES=../lib/cppunit-1.12.1/lib

CXXFLAGS = -g -Wall $(INCLUDE_DIRECTORIES:%=-I%) -DDLIB_NO_GUI_SUPPORT \
  -fprofile-arcs -ftest-coverage
LDFLAGS = $(LIB_DIRECTORIES:%=-L%)
LIBRARIES=  xml++-2.6 xml2 pthread z m glibmm-2.4 gobject-2.0 sigc-2.0 \
	glib-2.0 stdc++ cppunit dl gcov#intl iconv 

vpath %.hpp ../src
vpath %.cpp ../src

SOURCES= \
test.cpp \
adapter.cpp \
agent.cpp \
axes.cpp \
component.cpp \
component_event.cpp \
connector.cpp \
controller.cpp \
data_item.cpp \
device.cpp \
linear.cpp \
power.cpp \
spindle.cpp \
thermostat.cpp \
xml_parser.cpp \
xml_printer.cpp \
globals.cpp \
agent_test.cpp \
axes_test.cpp \
component_event_test.cpp \
globals_test.cpp \
data_item_test.cpp \
device_test.cpp \
xml_parser_test.cpp \
xml_printer_test.cpp

OBJS=$(SOURCES:%.cpp=%.o)
LIBS=$(LIBRARIES:%=-l%)

all: test

clean:
	@echo Removing all object, binary and test files
	@rm -rf *.o *.gcov *.gcda *.gcno test

test: $(OBJS)
	@echo Building test agent
	@g++ ${CXXFLAGS} ${LDFLAGS} -o test $+ ${LIBS}

%.o: %.cpp %.hpp
	@echo Compiling $<
	@g++ -c ${CXXFLAGS} $<

#test.o: axes.hpp axes_test.hpp component.hpp

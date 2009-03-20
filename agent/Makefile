
INCLUDE_DIRECTORIES=/opt/local/include/libxml++-2.6 \
	/opt/local/lib/libxml++-2.6/include /opt/local/include/libxml2 \
	/opt/local/include/glibmm-2.4 /opt/local/lib/glibmm-2.4/include \
	/opt/local/include/sigc++-2.0 /opt/local/lib/sigc++-2.0/include \
	/opt/local/include/glib-2.0 /opt/local/lib/glib-2.0/include \
	/opt/local/include ../lib ../devices

CXXFLAGS = -g -Wall $(INCLUDE_DIRECTORIES:%=-I%)
LDFLAGS = -L/opt/local/lib
LIBRARIES=  xml++-2.6 xml2 pthread z m glibmm-2.4 gobject-2.0 sigc-2.0 glib-2.0 intl iconv 

vpath %.hpp ../devices
vpath %.cpp ../devices

SOURCES= \
adapter.cpp \
component_event.cpp \
connector.cpp \
http.cpp \
test_connector.cpp \
units.cpp \
xml_parser.cpp \
xml_printer.cpp \
component.cpp \
device.cpp \
data_item.cpp \
axes.cpp \
spindle.cpp \
linear.cpp \
controller.cpp \
power.cpp


OBJS=$(SOURCES:%.cpp=%.o)
LIBS=$(LIBRARIES:%=-l%)

all: agent

agent: $(OBJS)
	g++ ${CXXFLAGS} ${LDFLAGS} -o agent $+ ${LIBS}

.cpp.o: 
	g++ -c ${CXXFLAGS} $< 

adapter.o: adapter.hpp
component_event.o: component_event.hpp
connector.o: connector.hpp
http.o: http.hpp
xml_parser.o: xml_parser.hpp
xml_printer.o: xml_printer.hpp
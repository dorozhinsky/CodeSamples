CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o

LIBS = 

TARGET =	TestApp

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

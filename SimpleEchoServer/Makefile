CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		SimpleEchoServer.o ThreadManager.o main.o

LIBS = -pthread

TARGET =	SimpleEchoServer

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

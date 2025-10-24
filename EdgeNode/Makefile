TARGET = motion
SRC = motion_detection_pt3.cpp
CXX = g++
CXXFLAGS = -Wall
LIBS = -lwiringPi -lcurl -lpthread

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

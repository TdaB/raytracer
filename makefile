CXX = g++

CXXFLAGS = -Wall -g -std=c++11

LIBS = -framework GLUT

TARGET = raytracer

all: $(TARGET)

$(TARGET): main.o objects.o $(TARGET).o scenes.o
	$(CXX) $(CXXFLAGS) $(LIBS) -o $(TARGET) main.o objects.o $(TARGET).o scenes.o

main.o: src/main.cpp src/objects.h src/$(TARGET).h src/scenes.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp

objects.o: src/objects.cpp
	$(CXX) $(CXXFLAGS) -c src/objects.cpp

$(TARGET).o: src/$(TARGET).cpp
	$(CXX) $(CXXFLAGS) -c src/$(TARGET).cpp

scenes.o: src/scenes.cpp src/objects.h
	$(CXX) $(CXXFLAGS) -c src/scenes.cpp

clean:
	$(RM) $(TARGET) *.o
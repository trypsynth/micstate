CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -s -mwindows
LDFLAGS = -Llib -lUniversalSpeech -lKernel32 -Wl,--gc-sections -Wl,--subsystem,windows -s
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)
TARGET = micstate

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

obj/%.o: src/%.cpp | obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj $(TARGET)

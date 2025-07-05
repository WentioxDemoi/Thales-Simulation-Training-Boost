CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lboost_system -lboost_thread -pthread
SRC = main.cpp Network/NetworkServer.cpp Session/NetworkSession.cpp Session/SessionManager.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = ThalesServer

all: $(TARGET)
	make baguette

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

baguette:
	rm -f $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean baguette run
CXX = g++
CXXFLAGS = -std=c++17
INCLUDES = -I/opt/homebrew/opt/sfml@2/include -I/opt/homebrew/include
LIBS = -L/opt/homebrew/opt/sfml@2/lib -L/opt/homebrew/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio -lboost_system -lboost_thread

SRC = main.cpp Core/Core.cpp Network/Network.cpp UI/UI.cpp

OBJ = $(SRC:.cpp=.o)

TARGET = ThalesClient

all: $(TARGET)
	@echo "Executable created successfully!"
	make baguette

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

baguette:
	rm -f $(OBJ)

clean:
	rm -f $(TARGET) $(OBJ)

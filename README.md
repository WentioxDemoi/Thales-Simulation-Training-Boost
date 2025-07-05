# Thales Simulation Training Boost

A client-server simulation and training system developed in C++17 using SFML for the graphical interface and Boost.Asio for network communication.

## 📋 Project Description

This project consists of a client-server application enabling simulation and training with a modern graphical interface. The system includes:

- **Client**: Graphical application with SFML user interface
- **Server**: Communication server managing multiple sessions
- **Modular Architecture**: Clear separation between Core, Network and UI

### Project Structure

```
Thales-Simulation-Training-Boost/
├── Client/
│   ├── Core/           # Business logic and message management
│   ├── Network/        # Client network communication
│   ├── UI/            # SFML graphical interface
│   ├── assets/        # Resources (fonts, etc.)
│   └── main.cpp       # Client entry point
├── Server/
│   ├── Network/       # Server network management
│   ├── Session/       # User session management
│   └── main.cpp       # Server entry point
└── README.md
```

## 🛠️ Prerequisites

### System Dependencies
- **CMake** >= 3.14
- **C++17 Compiler** (g++, clang++, or MSVC)
- **Boost** (version 1.70 or higher)
- **SFML** (version 2)

### Installing Dependencies

#### Linux (Debian/Ubuntu)
```bash
sudo apt update
sudo apt install build-essential cmake libboost-all-dev libsfml-dev
```

#### Linux (Fedora/RHEL)
```bash
sudo dnf install gcc-c++ cmake boost-devel SFML-devel
```

#### macOS (Homebrew)
```bash
brew install cmake boost sfml@2
```

#### Windows (vcpkg)
```bash
vcpkg install boost-system boost-thread sfml
```

## 🚀 Compilation

### Method 1: Using Makefiles (Recommended)

#### Compiling the Client
```bash
cd Client
make
```

#### Compiling the Server
```bash
cd Server
make
```

#### Cleaning
```bash
# Client
cd Client
make clean

# Server
cd Server
make clean
```

### Method 2: Manual Compilation

#### Client (macOS with Homebrew)
```bash
cd Client
g++ -std=c++17 main.cpp Core/Core.cpp Network/Network.cpp UI/UI.cpp \
  -I/opt/homebrew/opt/sfml@2/include \
  -I/opt/homebrew/include \
  -L/opt/homebrew/opt/sfml@2/lib \
  -L/opt/homebrew/lib \
  -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio \
  -lboost_system -lboost_thread \
  -o ThalesClient
```

#### Client (Linux)
```bash
cd Client
g++ -std=c++17 main.cpp Core/Core.cpp Network/Network.cpp UI/UI.cpp \
  -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio \
  -lboost_system -lboost_thread \
  -o ThalesClient
```

#### Server (macOS with Homebrew)
```bash
cd Server
g++ -std=c++17 main.cpp Network/NetworkServer.cpp Session/NetworkSession.cpp Session/SessionManager.cpp \
  -I/opt/homebrew/include \
  -L/opt/homebrew/lib \
  -lboost_system -lboost_thread \
  -pthread \
  -o ThalesServer
```

#### Server (Linux)
```bash
cd Server
g++ -std=c++17 main.cpp Network/NetworkServer.cpp Session/NetworkSession.cpp Session/SessionManager.cpp \
  -lboost_system -lboost_thread \
  -pthread \
  -o ThalesServer
```

## 🎮 Usage

### Starting the Server
```bash
cd Server
./ThalesServer
```
The server starts on port 8080 by default.

### Starting the Client
```bash
cd Client
./ThalesClient
```
The client automatically connects to the local server (127.0.0.1:8080).

### Shutdown
- **Server**: Ctrl+C to stop properly
- **Client**: Close the window or Ctrl+C

## 🔧 Configuration

### Server Port
The default port is 8080. To modify it, edit `Server/main.cpp` line 20:
```cpp
server->startServer(io_context, 8080); // Change 8080 to desired port
```

### Client Connection Address
To modify the connection address, edit `Client/main.cpp` line 58:
```cpp
if (!networkManager.connect("127.0.0.1", "8080")) {
    // Change "127.0.0.1" and "8080" according to your needs
}
```

## 🐛 Troubleshooting

### Compilation Errors

#### "SFML not found"
- Check that SFML is installed: `brew list sfml@2` (macOS) or `dpkg -l | grep sfml` (Linux)
- On macOS, make sure to use SFML 2.x: `brew install sfml@2`

#### "Boost not found"
- Check Boost installation: `brew list boost` (macOS) or `dpkg -l | grep libboost` (Linux)
- On some Linux systems, install headers: `sudo apt install libboost-all-dev`

#### Linking Errors
- Check that all libraries are properly linked
- On macOS, ensure Homebrew paths are correct

### Runtime Errors

#### "Impossible to connect to the server"
- Check that the server is started
- Check the port and connection address
- Check firewalls

#### "Error UI initialization"
- Check that fonts are present in `Client/assets/roboto/`
- Check file access permissions

## 📝 Development Notes

### Architecture
- **Singleton Pattern** used for managers (MessageManager, NetworkManager, UIManager)
- **Callbacks** for communication between modules
- **Threading** for asynchronous event handling

### Possible Extensions
- Multiple server support
- Configuration interface
- Detailed logging
- Unit tests
- API documentation

# Generate Executable File
add_executable(Receiver.out Receiver.cpp)

# Link Libraries
target_link_libraries(Receiver.out ComLib)

# Set Include Directories
target_include_directories(Receiver.out
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Lib/Com
  ${PROJECT_SOURCE_DIR}/Lib/Com/Socket
  ${PROJECT_SOURCE_DIR}/Lib/Com/Udp
  ${PROJECT_SOURCE_DIR}/Lib/Config
  ${PROJECT_SOURCE_DIR}/Receiver
)


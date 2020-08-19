# Generate Executable File
add_executable(Sender.out Sender.cpp)

# Link Libraries
target_link_libraries(Sender.out ComLib)

# Set Include Directories
target_include_directories(Sender.out
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Lib/Com
  ${PROJECT_SOURCE_DIR}/Lib/Com/Exception
  ${PROJECT_SOURCE_DIR}/Lib/Com/Socket
  ${PROJECT_SOURCE_DIR}/Lib/Com/Udp
  ${PROJECT_SOURCE_DIR}/Lib/Config
  ${PROJECT_SOURCE_DIR}/Lib/Utility
  ${PROJECT_SOURCE_DIR}/Receiver
)

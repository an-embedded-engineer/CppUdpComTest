# Generate Executable File
add_executable(Test.out Test.cpp)

# Link Libraries
target_link_libraries(Test.out ComLib)

# Set Include Directories
target_include_directories(Test.out
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Lib/Com
  ${PROJECT_SOURCE_DIR}/Lib/Com/Exception
  ${PROJECT_SOURCE_DIR}/Lib/Com/Socket
  ${PROJECT_SOURCE_DIR}/Lib/Com/Udp
  ${PROJECT_SOURCE_DIR}/Lib/Config
  ${PROJECT_SOURCE_DIR}/Lib/Utility
  ${PROJECT_SOURCE_DIR}/Test
)


# Generate Com Library
add_library(ComLib
  STATIC
    Com/Socket/SocketAdapter.cpp
)

# Set Include Directories
target_include_directories(ComLib
  PUBLIC
  ${PROJECT_SOURCE_DIR}/Lib/Com
  ${PROJECT_SOURCE_DIR}/Lib/Com/Socket
  ${PROJECT_SOURCE_DIR}/Lib/Com/Udp
  ${PROJECT_SOURCE_DIR}/Lib/Config
  ${PROJECT_SOURCE_DIR}/Lib/Utility
)

# Project Configuration
PROJECT ( "POSCA Server" )
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )
SET ( PROJECT_VERSION_MAJOR 1 )
SET ( PROJECT_VERSION_MINOR 2 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )

# Add Libraries
# LINK_LIBRARIES ( websocketpp )

# Build Options
# ADD_COMPILE_OPTIONS ( -march=armv7-a )
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 14 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Generate `connector_users.log.hpp`
EXECUTE_PROCESS ( bash connector_users.sh )

# Executables
ADD_EXECUTABLE (
  poscas
  common.hpp common.cpp
  core.hpp main.core.cpp opt.core.cpp
  keras.hpp proc.keras.cpp
  log.hpp connector_users.log.hpp soc.log.cpp csi.log.cpp
  ws.hpp server.ws.cpp
)

# Project Configuration
PROJECT ( "POSCA Server" )
CMAKE_MINIMUM_REQUIRED ( VERSION 3.7 )
SET ( PROJECT_VERSION_MAJOR 1 )
SET ( PROJECT_VERSION_MINOR 2 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )

# Add
#LINK_LIBRARIES ( websocketpp )
#LINK_LIBRARIES ( keras2cpp )

# Build Options
#ADD_COMPILE_OPTIONS ( -march=armv7-a )
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 14 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Executables
ADD_EXECUTABLE (
  poscas
  common.hpp common.cpp
  core.hpp main.core.cpp opt.core.cpp
  keras.hpp proc.keras.cpp
  log.hpp soc.log.cpp csi.log.cpp
)

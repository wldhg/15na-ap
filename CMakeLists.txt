# IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
# You can obtain a copy of MPL at LICENSE.md of root directory.
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )

# Set project parameters
# HERE IS THE ONLY SECTION USER CAN MODIFY
# <(Actual PPS(Packets Per Second)) * SEND_SECONDS> is the number of sending packets.
# <(Actual PPS) * (WINDOW_SECONDS - SLIDE_SECONDS)> is the number of remaining packets of one sending packets.
IF ( NOT DEFINED IRONA_SEND_SECONDS )
  SET ( IRONA_SEND_SECONDS 1.2 )
ENDIF ()
IF ( NOT DEFINED IRONA_WINDOW_SECONDS )
  SET ( IRONA_WINDOW_SECONDS 0.6 )
ENDIF ()
IF ( NOT DEFINED IRONA_SLIDE_SECONDS )
  SET ( IRONA_SLIDE_SECONDS 0.1 )
ENDIF ()
IF ( NOT DEFINED IRONA_INTERFACE )
  SET ( IRONA_INTERFACE "wlp1s0" )
ENDIF ()
IF ( NOT DEFINED IRONA_TX_FLAG )
  SET ( IRONA_TX_FLAG "0x4d01" )
ENDIF ()
IF ( NOT DEFINED IRONA_TLS )
  SET ( IRONA_TLS ON )
ENDIF ()

# BELOW HERE IS AUTOMATIC CONFIGURATION!
# YOU DONT NEED TO CHANGE BELOW UNLESS FOR DEVELOPING PURPOSE. :)

# CMake Configuration
CMAKE_POLICY ( SET CMP0048 NEW )
IF ( NOT ${CMAKE_VERSION} VERSION_LESS "3.13.0" )
  CMAKE_POLICY ( SET CMP0077 OLD )
ENDIF ()
INCLUDE ( ${CMAKE_ROOT}/Modules/ExternalProject.cmake )

# Build/CXX Options
IF ( NOT ${CMAKE_HOST_SYSTEM_VERSION} MATCHES "BPI-R2" )
  ADD_COMPILE_OPTIONS ( -march=armv7-a+vfpv4 -mtune=cortex-a7 )
ENDIF ()
ADD_COMPILE_OPTIONS ( -O3 -static -Wall )
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 17 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Project Configuration
PROJECT ( "IRONA AP" VERSION 1.4.2 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE OFF )
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin )

# Get CN_NETLINK_USERS
EXECUTE_PROCESS ( COMMAND uname -r
                  OUTPUT_VARIABLE KERNEL )
STRING ( STRIP ${KERNEL} KERNEL )
SET ( KERNEL_SOURCE /lib/modules/${KERNEL}/build )
IF ( EXISTS ${KERNEL_SOURCE}/include/uapi )
  MESSAGE ( "Using headers inside kernel source with uapi" )
  SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include/uapi )
ELSEIF ( EXISTS ${KERNEL_SOURCE}/include )
  MESSAGE ( "Using headers inside kernel source" )
  SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include )
ELSEIF ( EXISTS /usr/include/linux/connector.h )
  MESSAGE ( "Using headers inside /usr/include" )
  SET ( KERNEL_HEADERS /usr/include )
ELSE ()
  MESSAGE ( FATAL_ERROR "Kernel headers not found!" )
ENDIF ()
EXECUTE_PROCESS ( COMMAND bash -c "grep '#define CN_NETLINK_USERS' ${KERNEL_HEADERS}/linux/connector.h | cut -f 3"
                  OUTPUT_VARIABLE CN_NETLINK_USERS )

# Automatic variable conversion
IF ( IRONA_TLS )
  SET ( IRONA_PROTOCOL "https" )
  SET ( IRONA_PORT "443" )
ELSE ()
  SET ( IRONA_PROTOCOL "http" )
  SET ( IRONA_PORT "80" )
ENDIF ()

# Generate `.hpp`s from header template and update .gitignore
SET ( CMAKE_HPL_IGNORES "# Source files configured by CMake" )
SET ( IRONA_HEMPLATES 15na.hpl csi/csi.hpl )
FOREACH ( IRONA_HEMPLATE ${IRONA_HEMPLATES} )
  STRING ( REPLACE .hpl .hpp IRONA_HPP ${IRONA_HEMPLATE} )
  STRING ( CONCAT CMAKE_HPL_IGNORES ${CMAKE_HPL_IGNORES} "\nsrc/${IRONA_HPP}" )
  CONFIGURE_FILE ( ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_HEMPLATE} ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_HPP} )
ENDFOREACH ( IRONA_HEMPLATE )
CONFIGURE_FILE ( ${CMAKE_CURRENT_LIST_DIR}/.gitignore.in ${CMAKE_CURRENT_LIST_DIR}/.gitignore )

# Make dependency on libraries
SET ( BUILD_TESTING OFF ) # To block socket.io to generate test cases
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/lorcon )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io/src )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/rapidjson/include )

# Link libraries
LINK_LIBRARIES ( lorcon pthread )
IF ( IRONA_TLS )
  LINK_LIBRARIES ( sioclient_tls )
ELSE ()
  LINK_LIBRARIES ( sioclient )
ENDIF ()

# Use subdirectories
SET ( IRONA_DIRECTORIES . core csi ws )
FOREACH ( IRONA_DIR ${IRONA_DIRECTORIES} )
  INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_DIR} )
  AUX_SOURCE_DIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_DIR} IRONA_SOURCES )
ENDFOREACH ( IRONA_DIR )

# Executable
ADD_EXECUTABLE ( 15na-ap ${IRONA_SOURCES} )
ADD_DEPENDENCIES ( 15na-ap lorcon )
IF ( IRONA_TLS )
  ADD_DEPENDENCIES ( 15na-ap sioclient_tls )
  SET_TARGET_PROPERTIES ( sioclient PROPERTIES EXCLUDE_FROM_ALL 1 )
ELSE ()
  ADD_DEPENDENCIES ( 15na-ap sioclient )
  SET_TARGET_PROPERTIES ( sioclient_tls PROPERTIES EXCLUDE_FROM_ALL 1 )
ENDIF ()

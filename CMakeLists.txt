# CMake Configuration
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )
CMAKE_POLICY ( SET CMP0048 NEW )
IF ( NOT ${CMAKE_VERSION} VERSION_LESS "3.13.0" )
  CMAKE_POLICY ( SET CMP0077 OLD )
ENDIF ()
INCLUDE ( ${CMAKE_ROOT}/Modules/ExternalProject.cmake )

# Build/CXX Options
IF ( NOT ${CMAKE_HOST_SYSTEM_VERSION} MATCHES "BPI-R2" )
  SET ( CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc )
  SET ( CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++ )
  ADD_COMPILE_OPTIONS ( -march=armv7-a+vfpv4 -mtune=cortex-a7 )
ENDIF ()
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 17 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Project Configuration
PROJECT ( "15na AP" VERSION 1.1.4 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )
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

# Set project parameters
SET ( IRONA_SEND_CNT 4700 )
SET ( IRONA_WINDOW_CNT 2820 )
SET ( IRONA_SLIDE_CNT 470 )

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
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io/src )

# Link libraries
LINK_LIBRARIES ( sioclient_tls )
LINK_LIBRARIES ( pthread )

# Use subdirectories
SET ( IRONA_DIRECTORIES . core csi ws )
FOREACH ( IRONA_DIR ${IRONA_DIRECTORIES} )
  INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_DIR} )
  AUX_SOURCE_DIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/src/${IRONA_DIR} IRONA_SOURCES )
ENDFOREACH ( IRONA_DIR )

# Executable
ADD_EXECUTABLE ( 15na-ap ${IRONA_SOURCES} )
ADD_DEPENDENCIES ( 15na-ap sioclient_tls )

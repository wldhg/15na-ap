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
PROJECT ( "Syaa AP" VERSION 1.0.0 )
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
SET ( SYAA_WINDOW 1880 )
SET ( SYAA_SLIDE 200 )

# Generate `.hpp`s from header template and update .gitignore
SET ( CMAKE_HPL_IGNORES "# Source files configured by CMake" )
SET ( SYAA_HEMPLATES syaa.hpl csi/csi.hpl )
FOREACH ( SYAA_HEMPLATE ${SYAA_HEMPLATES} )
  STRING ( REPLACE .hpl .hpp SYAA_HPP ${SYAA_HEMPLATE} )
  STRING ( CONCAT CMAKE_HPL_IGNORES ${CMAKE_HPL_IGNORES} "\nsrc/${SYAA_HPP}" )
  CONFIGURE_FILE ( ${CMAKE_CURRENT_LIST_DIR}/src/${SYAA_HEMPLATE} ${CMAKE_CURRENT_LIST_DIR}/src/${SYAA_HPP} )
ENDFOREACH ( SYAA_HEMPLATE )
CONFIGURE_FILE ( ${CMAKE_CURRENT_LIST_DIR}/.gitignore.in ${CMAKE_CURRENT_LIST_DIR}/.gitignore )

# Make dependency on libraries
SET ( BUILD_TESTING OFF ) # To block nlohmann_json build test binaries
FIND_PACKAGE ( Eigen3 3.3 REQUIRED NO_MODULE )
EXTERNALPROJECT_ADD ( FunctionalPlus
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus
  BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus
  PREFIX ${CMAKE_CURRENT_LIST_DIR}/tmp
  CMAKE_ARGS -DFPLUS_BUILD_UNITTEST=OFF -DFPLUS_BUILD_EXAMPLES=OFF )
EXTERNALPROJECT_ADD ( nlohmann_json
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/json
  BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/json
  PREFIX ${CMAKE_CURRENT_LIST_DIR}/tmp
  CMAKE_ARGS -DBUILD_TESTING=FALSE )
EXTERNALPROJECT_ADD ( frugally-deep
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep
  BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep
  PREFIX ${CMAKE_CURRENT_LIST_DIR}/tmp
  CMAKE_ARGS -DFDEEP_BUILD_UNITTEST=OFF
  BUILD_COMMAND ""
  INSTALL_COMMAND "" )
ADD_DEPENDENCIES ( frugally-deep nlohmann_json FunctionalPlus )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus/include )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/json/include )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep/include )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io/src )

# Link libraries
LINK_LIBRARIES ( Eigen3::Eigen )
LINK_LIBRARIES ( sioclient_tls )
LINK_LIBRARIES ( pthread )

# Use subdirectories
SET ( SYAA_DIRECTORIES . core csi keras ws )
FOREACH ( SYAA_DIR ${SYAA_DIRECTORIES} )
  INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/src/${SYAA_DIR} )
  AUX_SOURCE_DIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/src/${SYAA_DIR} SYAA_SOURCES )
ENDFOREACH ( SYAA_DIR )

# Executable
ADD_EXECUTABLE ( syaa-ap ${SYAA_SOURCES} )
ADD_DEPENDENCIES ( syaa-ap frugally-deep sioclient_tls )

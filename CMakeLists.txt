# Project Configuration
PROJECT ( "POSCA Server" )
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )
SET ( PROJECT_VERSION_MAJOR 1 )
SET ( PROJECT_VERSION_MINOR 0 )
SET ( PROJECT_VERSION_PATCH 3 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin )

# Build/CXX Options
# ADD_COMPILE_OPTIONS ( -march=armv7-a )
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 17 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Generate `connector_users.log.hpp`
IF ( NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/csi/connector_users.csi.hpp )
  EXECUTE_PROCESS ( COMMAND uname -r
                    OUTPUT_VARIABLE KERNEL )
  STRING ( STRIP ${KERNEL} KERNEL )
  SET ( KERNEL_SOURCE /lib/modules/${KERNEL}/build )
  IF ( EXISTS ${KERNEL_SOURCE}/include/uapi )
    SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include/uapi )
  ELSEIF ( EXISTS ${KERNEL_SOURCE}/include )
    SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include )
  ELSE ()
    MESSAGE ( FATAL_ERROR "Kernel headers not found!" )
  ENDIF ()
  EXECUTE_PROCESS ( COMMAND bash -c "echo '#undef CN_NETLINK_USERS' > ${CMAKE_CURRENT_LIST_DIR}/src/csi/connector_users.csi.hpp" )
  EXECUTE_PROCESS ( COMMAND bash -c "grep '#define CN_NETLINK_USERS' ${KERNEL_HEADERS}/linux/connector.h >> ${CMAKE_CURRENT_LIST_DIR}/src/csi/connector_users.csi.hpp" )
ENDIF ()

# Make dependency on libraries
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io-client-cpp )

# Link libraries
LINK_LIBRARIES ( frugally-deep )
LINK_LIBRARIES ( pthread )
LINK_LIBRARIES ( sioclient )

# Use subdirectories
SET ( POSCA_DIRECTORIES . core csi keras ws )
FOREACH ( POSCA_DIR ${POSCA_DIRECTORIES} )
  INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_DIR} )
  AUX_SOURCE_DIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_DIR} POSCA_SOURCES )
ENDFOREACH( POSCA_DIR )

# Executable
ADD_EXECUTABLE (
  poscas
  ${POSCA_SOURCES}
)

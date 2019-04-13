# CMake Configuration
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )
CMAKE_POLICY ( SET CMP0048 NEW )

# Project Configuration
PROJECT ( "POSCA Server" VERSION 1.0.3 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )
SET ( CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin )

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
FIND_PACKAGE ( Eigen3 3.3 REQUIRED NO_MODULE )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/json )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/json/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io-client-cpp )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/socket.io-client-cpp/src )

# Link libraries
LINK_LIBRARIES ( Eigen3::Eigen )
LINK_LIBRARIES ( FunctionalPlus )
LINK_LIBRARIES ( nlohmann_json )
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

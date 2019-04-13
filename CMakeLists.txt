# CMake Configuration
CMAKE_MINIMUM_REQUIRED ( VERSION 3.8 )
CMAKE_POLICY ( SET CMP0048 NEW )
CMAKE_POLICY ( SET CMP0077 OLD )

# Project Configuration
PROJECT ( "POSCA Server" VERSION 1.0.3 )
SET ( CMAKE_BUILD_TYPE Release )
SET ( CMAKE_VERBOSE_MAKEFILE false )
SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin )

# Build/CXX Options
# ADD_COMPILE_OPTIONS ( -march=armv7-a )
SET ( CMAKE_C_FLAGS_DEBUG "-g" )
SET ( CMAKE_CXX_STANDARD 17 )
SET ( CMAKE_CXX_STANDARD_REQUIRED ON )

# Get CN_NETLINK_USERS
EXECUTE_PROCESS ( COMMAND uname -r
                  OUTPUT_VARIABLE KERNEL )
STRING ( FIND ${KERNEL} -Microsoft IS_WSL )
IF ( ${IS_WSL} EQUAL -1 )
  STRING ( STRIP ${KERNEL} KERNEL )
  SET ( KERNEL_SOURCE /lib/modules/${KERNEL}/build )
  IF ( EXISTS ${KERNEL_SOURCE}/include/uapi )
    SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include/uapi )
  ELSEIF ( EXISTS ${KERNEL_SOURCE}/include )
    SET ( KERNEL_HEADERS ${KERNEL_SOURCE}/include )
  ELSE ()
    MESSAGE ( FATAL_ERROR "Kernel headers not found!" )
  ENDIF ()
  EXECUTE_PROCESS ( COMMAND bash -c "grep '#define CN_NETLINK_USERS' ${KERNEL_HEADERS}/linux/connector.h | cut -f 3"
                    OUTPUT_VARIABLE CN_NETLINK_USERS )
ELSE ()
  MESSAGE ( WARNING "In WSL mode, `CN_NETLINK_USER` will be a DUMMY one. Only use this for development." )
  SET ( CN_NETLINK_USERS 11 )
ENDIF ()


# Generate `.hpp`s from header template
SET ( POSCA_HEMPLATES poscas.hpl csi/csi.hpl )
FOREACH ( POSCA_HEMPLATE ${POSCA_HEMPLATES} )
  STRING ( REPLACE .hpl .hpp POSCA_HPP ${POSCA_HEMPLATE} )
  CONFIGURE_FILE ( ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_HEMPLATE} ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_HPP} )
ENDFOREACH ( POSCA_HEMPLATE )


# Make dependency on libraries
SET ( BUILD_TESTING FALSE ) # Block json creating test binaries
FIND_PACKAGE ( Eigen3 3.3 REQUIRED NO_MODULE )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/FunctionalPlus/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/json )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/json/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/frugally-deep/include )
ADD_SUBDIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/lib/Simple-WebSocket-Server )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/lib/Simple-WebSocket-Server )

# Link libraries
LINK_LIBRARIES ( Eigen3::Eigen )
LINK_LIBRARIES ( fplus )
LINK_LIBRARIES ( nlohmann_json )
LINK_LIBRARIES ( simple-websocket-server )

# Use subdirectories
SET ( POSCA_DIRECTORIES . core csi keras ws )
FOREACH ( POSCA_DIR ${POSCA_DIRECTORIES} )
  INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_DIR} )
  AUX_SOURCE_DIRECTORY ( ${CMAKE_CURRENT_LIST_DIR}/src/${POSCA_DIR} POSCA_SOURCES )
ENDFOREACH ( POSCA_DIR )

# Executable
ADD_EXECUTABLE (
  poscas
  ${POSCA_SOURCES}
)

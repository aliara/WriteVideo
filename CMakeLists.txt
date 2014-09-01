cmake_minimum_required (VERSION 2.6)
INCLUDE(InstallRequiredSystemLibraries)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project(VidRecord CXX)
find_package( OpenCV REQUIRED )

include_directories ("${PROJECT_BINARY_DIR} /usr/local/include")
add_executable(vidrecord VidRecord.cpp Config.cpp)
target_link_libraries (vidrecord  ${OpenCV_LIBS})

install(TARGETS vidrecord DESTINATION ~/bin)

# issue: dpkg tries to remove home directory when package removed -- not good!
#SET(CPACK_GENERATOR "DEB")
#SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Vern Raben") #required
#INCLUDE(CPack)



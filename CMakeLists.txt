# CMakeLists for the temp analysis repository

# Set up
cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

# ROOT
find_package(ROOT REQUIRED)

# Project name
project (BAND-LASER)

set(INCLUDES ${ROOT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# Add executable for making his
#add_executable(laserHist LASER_his.cpp)
#target_link_libraries(laserHist ${ROOT_LIBRARIES})

#Add executable for making his
#add_executable(newHist newHist.cpp)
#target_link_libraries(newHist ${ROOT_LIBRARIES})


#Add executable for making his
add_executable(timeWalk timeWalk.cpp)
target_link_libraries(timeWalk ${ROOT_LIBRARIES})

# Add executable for making getRes
add_executable(getRes getRes.cpp)
target_link_libraries(getRes ${ROOT_LIBRARIES})

# Add executable for making getHist
add_executable(getHist getHist.cpp)
target_link_libraries(getHist ${ROOT_LIBRARIES})

# Add executable for making getGraph
add_executable(getGraph getGraph.cpp)
target_link_libraries(getGraph ${ROOT_LIBRARIES})




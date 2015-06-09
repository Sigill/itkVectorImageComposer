cmake_minimum_required(VERSION 2.8)

project(VectorImageComposer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost COMPONENTS program_options system filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Log4Cxx REQUIRED)
include_directories(${LOG4CXX_INCLUDE_DIR})

include(FindOpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(VectorImageComposer VectorImageComposer.cxx)
target_link_libraries(VectorImageComposer ${Boost_LIBRARIES} ${LOG4CXX_LIBRARIES} ${ITK_LIBRARIES})

add_executable(VectorImageNormalizer VectorImageNormalizer.cpp)
target_link_libraries(VectorImageNormalizer ${Boost_LIBRARIES} ${LOG4CXX_LIBRARIES} ${ITK_LIBRARIES})

# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.20)

# Set the name of the project and the programming language
project(Test CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STA ON)

# Set the source files for the project
set(SOURCES
    main.cpp
    mongodb-tools/mongodb.cpp
)

set(HEADERS
    mongodb-tools/mongodb.h
)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
#find_package(libmongocxx REQUIRED)
#find_package(libbsoncxx REQUIRED)

#include_directories(${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})

#include_directories(/home/samyar/Downloads/mongo-cxx-driver-r3.7.1/build/src/)
#include_directories(/home/samyar/Downloads/mongo-cxx-driver-r3.7.1/src/)
include_directories(/usr/local/include/mongocxx/v_noabi)
include_directories(/usr/local/include/bsoncxx/v_noabi)

#link_libraries(${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES})
#link_directories(${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the include directory for the project

#include(/home/samyar/Documents/mongodb-class/mongodb-tools/CMakeLists.txt)

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBMONGOCXX_INCLUDE_DIRS} ${LIBBSONCXX_INCLUDE_DIRS})
#include_directories(${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})

# Set the C++ standard to use

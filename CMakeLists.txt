message("ZeroMQ Major Domo Protocole c++")
cmake_minimum_required(VERSION 3.20)

project(
    MajordomeBroker
    VERSION 0.0.1
    DESCRIPTION "ZeroMq MDP : zeroMQ"
    LANGUAGES CXX
)

# Set the C++ standard
#set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_PREFIX_PATH "C:\\zmqLib\\CMake")
find_package(ZeroMQ REQUIRED)

set(JSONCPP_LOCAL_DIR "C:\\_Nmx_Prx\\ZeroMqFolder\\install\\x64-jsoncpp")
list(APPEND CMAKE_PREFIX_PATH "${JSONCPP_LOCAL_DIR}/lib/CMake")
find_package(jsoncpp REQUIRED)
get_target_property(JSONCPP_INCLUDE_DIR jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSONCPP_INCLUDE_DIR})


set(ZMQ_PP_LOCAL_DIR "C:\\_Nmx_Prx\\ZeroMqFolder\\install\\x64-zmqpp")
list(APPEND CMAKE_PREFIX_PATH "${ZMQ_PP_LOCAL_DIR}/lib/CMake")
find_package(zmqpp REQUIRED)
get_target_property(ZMQ_PP_INCLUDE_DIR zmqpp_lib INTERFACE_INCLUDE_DIRECTORIES)
message("Importing zmqpp lib include ${ZMQ_PP_INCLUDE_DIR}")
include_directories(${ZMQ_PP_INCLUDE_DIR})


set (serverApp_SRCS
   src/ZMQIdentity.cpp
   src/MutualHeartbeatMonitor.cpp
   src/ZMQBrokerContext.cpp
   src/Broker.cpp
)


set(entry serverApp)

message("Target : ${entry}")
add_executable (${entry} ${${entry}_SRCS}) 
target_include_directories(${entry} PRIVATE ./include PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(${entry} PUBLIC libzmq zmqpp_static jsoncpp_static)

set (clientApp_SRCS
   src/Client.cpp
   src/ZMQClientContext.cpp
   src/ZMQIdentity.cpp
   example/clientApp.cpp
)

set(entry clientApp)

message("Target : ${entry}")
add_executable (${entry} ${${entry}_SRCS}) 
target_include_directories(${entry} PRIVATE ./include PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(${entry} PUBLIC libzmq zmqpp_static jsoncpp_static)


set (echoWorkerApp_SRCS
   src/Worker.cpp
   src/ZMQWorkerContext.cpp
   src/WorkerTask.cpp
   src/ZMQIdentity.cpp
   example/echo_worker.cpp
)

set(entry echoWorkerApp)

message("Target : ${entry}")
add_executable (${entry} ${${entry}_SRCS}) 
target_include_directories(${entry} PRIVATE ./include PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(${entry} PUBLIC libzmq zmqpp_static jsoncpp_static)

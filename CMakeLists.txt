cmake_minimum_required(VERSION 3.10)

project(rocos-dbg LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

add_subdirectory(QRibbon)

############################
#####    Protocol      #####
############################

file(GLOB PROTO_FILES protos/*.proto) # protobuf
file(GLOB GRPC_FILES protos/*_service.proto) # gRPC

foreach (PFIL ${PROTO_FILES}) #protobuf gen
    # Proto file
    get_filename_component(PROTO_ABS ${PFIL} ABSOLUTE)
    get_filename_component(PROTO_PATH ${PFIL} PATH)
    get_filename_component(PROTO_NAME ${PFIL} NAME_WE)

    # Generated sources
    set(PROTO_SRC "${CMAKE_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    set(PROTO_HDR "${CMAKE_BINARY_DIR}/${PROTO_NAME}.pb.h")
    add_custom_command(
            OUTPUT "${PROTO_SRC}" "${PROTO_HDR}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --cpp_out "${CMAKE_BINARY_DIR}"
            -I "${PROTO_PATH}"
            "${PROTO_ABS}"
            DEPENDS "${PROTO_ABS}"
    )
    list(APPEND PROTO_SRCS ${PROTO_SRC})
    list(APPEND PROTO_HDRS ${PROTO_HDR})
endforeach ()

foreach (GFIL ${GRPC_FILES}) #grpc gen
    # Proto file
    get_filename_component(GRPC_ABS ${GFIL} ABSOLUTE)
    get_filename_component(GRPC_PATH ${GFIL} PATH)
    get_filename_component(GRPC_NAME ${GFIL} NAME_WE)

    # Generated sources
    set(GRPC_SRC "${CMAKE_BINARY_DIR}/${GRPC_NAME}.grpc.pb.cc")
    set(GRPC_HDR "${CMAKE_BINARY_DIR}/${GRPC_NAME}.grpc.pb.h")
    add_custom_command(
            OUTPUT "${GRPC_SRC}" "${GRPC_HDR}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
            -I "${GRPC_PATH}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${GRPC_ABS}"
            DEPENDS "${GRPC_ABS}"
    )
    list(APPEND GRPC_SRCS ${GRPC_SRC})
    list(APPEND GRPC_HDRS ${GRPC_HDR})
endforeach ()

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_library(protocol
        SHARED
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${GRPC_SRCS}
        ${GRPC_HRDS}
        src/robot_service.cpp
        )
target_link_libraries(protocol
        PUBLIC
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF}
        )
add_library(rocos::protocol ALIAS protocol)

############################
#####    Executable    #####
############################

file(GLOB SRC "src/*")

if(ANDROID)
  add_library(rocos-dbg SHARED
    ${SRC}
    res.qrc
  )
else()
  add_executable(rocos-dbg
    ${SRC}
    res.qrc
  )
endif()

target_include_directories(rocos-dbg PUBLIC src)
target_link_libraries(rocos-dbg PRIVATE Qt5::Widgets Qt5::Network QRibbon::qribbon rocos::protocol)

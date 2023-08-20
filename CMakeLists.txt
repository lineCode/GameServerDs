cmake_minimum_required(VERSION 3.1)

project(GamerServerDs)

set(PB_ROOT_PATH protobuf)
set(PB_GENERATE_PATH protobuf)

FILE(GLOB PB_SOURCE_FILE ${PB_ROOT_PATH}/*.proto)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/lib)

SET(LIB_NAME "common")
SET(PROTOBUF_GENERATE_FILE_PATH ${PROJECT_SOURCE_DIR}/common/protobuf)


add_definitions(-std=c++11)


message(STATUS "Protobuf File:${PB_SOURCE_FILE}")

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(GENERATED_SRC GENERATED_HEADER ${PB_SOURCE_FILE})

message("xxx: ${PB_SOURCE_FILE}")

SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/protobuf)

FOREACH(FIL ${PB_SOURCE_FILE})
 
    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
 
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
    string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})
 
    # string(REGEX MATCH "(/mediapipe/framework.*|/mediapipe/util.*|/mediapipe/calculators/internal/)" OUT_PATH ${FILE_PATH})
 
    set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
 
    EXECUTE_PROCESS(
            COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
    )
    message("Copying " ${PROTO_SRCS} " to " ${PROTOBUF_GENERATE_FILE_PATH}/${FILE_NAME}.pb.cc)

    file(RENAME ${PROTO_SRCS}  ${PROTOBUF_GENERATE_FILE_PATH}/${FILE_NAME}.pb.cc)
    file(RENAME ${PROTO_HDRS} ${PROTOBUF_GENERATE_FILE_PATH}/${FILE_NAME}.pb.h)
 
ENDFOREACH()

FILE(GLOB lib_source_file common/*/*.cpp common/*/*.cc common/*/*/*.cpp common/*/*.c)


add_library(${LIB_NAME} SHARED ${lib_source_file})

target_include_directories(${LIB_NAME} PUBLIC
            ext/boost_1_72_0/
            ext/spdlog-1.5.0/include/
            ext/hiredis)


# target_link_libraries(${LIB_NAME} PUBLIC 
#     -Lext/boost_1_72_0/stage/lib/*
#     /usr/local/lib64/libhiredis.so
#     /usr/lib64/mysql/libmysqlclient.so
#     /usr/local/lib/libprotobuf.so
#     -lpthread
#     )

target_link_libraries(${LIB_NAME} PUBLIC 
    -Lext/boost_1_72_0/stage/lib/*
    -lhiredis
    -lmysqlclient
    -lprotobuf
    -lpthread
)

include_directories(BEFORE 
    ${PROJECT_SOURCE_DIR}/common/datahanle/
    ${PROJECT_SOURCE_DIR}/common/include/
    ${PROJECT_SOURCE_DIR}/network/
    ext/boost_1_72_0/
    ext/spdlog-1.5.0/include/
    ext/hiredis
)


# link_libraries(-Lext/boost_1_72_0/stage/lib/*
#     /usr/local/lib64/libhiredis.so
#     /usr/lib64/mysql/libmysqlclient.so
#     /usr/local/lib/libprotobuf.so
#     -lpthread
#      ${LIBRARY_OUTPUT_PATH}/lib${LIB_NAME}.so
# )

link_libraries(-Lext/boost_1_72_0/stage/lib/*
    -lhiredis
    -lmysqlclient
    -lprotobuf
    -lpthread
     ${LIBRARY_OUTPUT_PATH}/lib${LIB_NAME}.so
)




set(LOGIN_SERVER_NAME "LoginServer")

FILE(GLOB login_server_source_file ${LOGIN_SERVER_NAME}/src/*.cpp  ${LOGIN_SERVER_NAME}/src/*/*.cpp  ${LOGIN_SERVER_NAME}/src/*/*/*.cpp)


add_executable(${LOGIN_SERVER_NAME} ${login_server_source_file})


set(GATE_SERVER_NAME "gateServer")

FILE(GLOB gate_server_source_file gateServer/src/*.cpp gateServer/src/*/*.cpp gateServer/src/*/*/*.cpp)


add_executable(${GATE_SERVER_NAME} ${gate_server_source_file})

set(DB_SERVER_NAME "dataServer")

FILE(GLOB db_server_file DBServer/src/*.cpp DBServer/src/*/*.cpp)

add_executable(${DB_SERVER_NAME} ${db_server_file})


set(GAME_SERVER_NAME "GameServer")

FILE(GLOB ${GAME_SERVER_NAME}_FILES ${GAME_SERVER_NAME}/src/*.cpp ${GAME_SERVER_NAME}/src/*/*.cpp ${GAME_SERVER_NAME}/src/*/*/*.cpp)

message(STATUS "${${GAME_SERVER_NAME}_FILES}")

add_executable(${GAME_SERVER_NAME} ${${GAME_SERVER_NAME}_FILES})


set(NODE_SERVER_NAME "NodeServer")

FILE(GLOB ${NODE_SERVER_NAME}_FILES ${NODE_SERVER_NAME}/src/*.cpp ${NODE_SERVER_NAME}/src/*/*.cpp ${NODE_SERVER_NAME}/src/*/*/*.cpp)

message(STATUS "${${NODE_SERVER_NAME}_FILES}")

add_executable(${NODE_SERVER_NAME} ${${NODE_SERVER_NAME}_FILES})



set(ROOM_SERVER_NAME "RoomServer")

FILE(GLOB ${ROOM_SERVER_NAME}_FILES ${ROOM_SERVER_NAME}/src/*.cpp ${ROOM_SERVER_NAME}/src/*/*.cpp ${ROOM_SERVER_NAME}/src/*/*/*.cpp)

message(STATUS "${${ROOM_SERVER_NAME}_FILES}")

add_executable(${ROOM_SERVER_NAME} ${${ROOM_SERVER_NAME}_FILES})


set(BATTLE_SERVER_NAME "BattleServer")

FILE(GLOB ${BATTLE_SERVER_NAME}_FILES ${BATTLE_SERVER_NAME}/src/*.cpp ${BATTLE_SERVER_NAME}/src/*/*.cpp ${BATTLE_SERVER_NAME}/src/*/*/*.cpp ${BATTLE_SERVER_NAME}/src/*/*/*/*.cpp)

message(STATUS "${${BATTLE_SERVER_NAME}_FILES}")

add_executable(${BATTLE_SERVER_NAME} ${${BATTLE_SERVER_NAME}_FILES})
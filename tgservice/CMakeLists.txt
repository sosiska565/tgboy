cmake_minimum_required(VERSION 3.10)
project(tgservice)

find_package(TgBot REQUIRED)
find_package(Threads REQUIRED)

add_executable(
    tgservice
    src/main.cpp
    src/service/service.cpp
)

target_link_libraries(
    tgservice
    TgBot::TgBot
    curl
    ssl
    crypto
)

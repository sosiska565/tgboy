cmake_minimum_required(VERSION 3.10)
project(mainservice)

find_package(PkgConfig REQUIRED)

pkg_check_modules(PQXX REQUIRED libpqxx)

add_executable(
    mainservice
    src/main.cpp
    src/service/service.cpp
    src/controller/controller.cpp
    src/repository/user/userRepository.cpp
)

target_include_directories(mainservice PRIVATE ${PQXX_INCLUDE_DIRS})

target_link_libraries(mainservice PRIVATE ${PQXX_LIBRARIES})

cmake_minimum_required(VERSION 3.12)
project(myfilesystem C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(myfilesystem
        file.c
        fs.c
        login.c
        main.c
        main.h users.c chmod.c)

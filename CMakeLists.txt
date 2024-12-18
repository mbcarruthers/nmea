cmake_minimum_required(VERSION 3.30)
project(cpl C)

set(CMAKE_C_STANDARD 23)

add_executable(cpl main.c nmea.c sentences.c)

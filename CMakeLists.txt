cmake_minimum_required(VERSION 3.12)
project(QueueBuffer)

set(CMAKE_CXX_STANDARD 11)

add_executable(QueueBuffer main.cpp Queue.c Queue.h CQueue.cpp CQueue.h)
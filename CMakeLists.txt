cmake_minimum_required(VERSION 3.17)
project(ShadowStorageManagement)

set(CMAKE_CXX_STANDARD 14)

add_executable(ShadowStorageManagement shadow_interface.cpp CanDDaGae.cpp StorageElement.cpp main.cpp)
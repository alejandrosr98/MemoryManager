cmake_minimum_required (VERSION 3.8)

project ("MemoryManager")

add_executable (memoryManager "app/main.cpp" "files/src/raw.cpp" "files/include/raw.h")
target_compile_features(memoryManager PRIVATE cxx_std_17)
target_include_directories(memoryManager PUBLIC "app")


#add_library(fileLib "files/src/raw.cpp")

#target_include_directories(fileLib PUBLIC "files/include/")


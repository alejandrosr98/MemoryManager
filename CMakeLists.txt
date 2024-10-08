cmake_minimum_required (VERSION 3.8)

project ("MemoryManager")

add_executable (memoryManager "app/main.cpp")
target_compile_features(memoryManager PRIVATE cxx_std_17)
target_include_directories(memoryManager PUBLIC "app")

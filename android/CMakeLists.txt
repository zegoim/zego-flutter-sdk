
cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
             ZegoLog

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/zego_log_jni.cpp)

add_library(
        zegoliveroom
        SHARED
        IMPORTED
)

set_target_properties(zegoliveroom
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_LIST_DIR}/libs/${ANDROID_ABI}/libzegoliveroom.so
)

target_link_libraries( # Specifies the target library.

        ZegoLog
        zegoliveroom
        )
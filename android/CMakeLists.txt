
cmake_minimum_required(VERSION 3.4.1)

add_library( # Sets the name of the library.
             ZegoLog

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/zego_log_jni.cpp)

add_library(
        ZegoLiveRoom
        SHARED
        IMPORTED
)

set_target_properties(ZegoLiveRoom
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_LIST_DIR}/libs/${ANDROID_ABI}/libZegoLiveRoom.so
)

target_link_libraries( # Specifies the target library.

        ZegoLog
        ZegoLiveRoom
        )
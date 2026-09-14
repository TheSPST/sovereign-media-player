cmake_minimum_required(VERSION 3.16)
project(SovereignMediaPlayer VERSION 2.2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization Flags & Static Runtime for Windows
if(MSVC)
    add_compile_options(/O2 /GL /W3 /utf-8)
    add_link_options(/LTCG /OPT:REF /OPT:ICF)
    # Static MSVC runtime (/MT) to avoid missing VCRUNTIME140.dll on clean Windows machines
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else()
    add_compile_options(-O3 -flto -fvisibility=hidden -Wall -Wextra)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core_engine
    ${CMAKE_CURRENT_SOURCE_DIR}/core_engine/include
)

# ------------------------------------------------------------------------------
# 1. Core Libraries (Both Shared for Developers and Static for Standalone)
# ------------------------------------------------------------------------------
if(ANDROID)
    if(NOT DEFINED ANDROID_PLATFORM)
        set(ANDROID_PLATFORM android-29)
    endif()
    add_library(sovereign_android SHARED
        core_engine/sovereign_engine.cpp
        android/sovereign_android_jni.cpp
    )
    target_link_libraries(sovereign_android PRIVATE android log)
else()
    # Shared library for external developers / integrations
    add_library(sovereign_core SHARED
        core_engine/sovereign_engine.cpp
    )

    # Static library for self-contained standalone executable
    add_library(sovereign_core_static STATIC
        core_engine/sovereign_engine.cpp
    )
    target_compile_definitions(sovereign_core_static PUBLIC SOVEREIGN_STATIC)

    if(WIN32)
        target_compile_definitions(sovereign_core PRIVATE SOVEREIGN_EXPORTS)
        target_link_libraries(sovereign_core PRIVATE ws2_32 user32 shell32)
        target_link_libraries(sovereign_core_static PRIVATE ws2_32 user32 shell32)
    elseif(UNIX AND NOT APPLE)
        target_link_libraries(sovereign_core PRIVATE pthread dl m)
        target_link_libraries(sovereign_core_static PRIVATE pthread dl m)
    endif()

    # --------------------------------------------------------------------------
    # 2. Cross-Platform Desktop Player Binary (Windows .exe / Linux ELF)
    # Statically links core engine so it has ZERO external DLL dependencies!
    # --------------------------------------------------------------------------
    add_executable(SovereignPlayer
        frontend_crossplatform/sovereign_player_main.cpp
    )
    target_link_libraries(SovereignPlayer PRIVATE sovereign_core_static)

    if(WIN32)
        set_target_properties(SovereignPlayer PROPERTIES OUTPUT_NAME "SovereignPlayer")
    endif()
endif()

# Installation Targets
if(NOT ANDROID)
    install(TARGETS sovereign_core sovereign_core_static SovereignPlayer
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    install(FILES core_engine/include/sovereign_engine.h DESTINATION include)
endif()

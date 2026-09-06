cmake_minimum_required(VERSION 3.16)
project(SovereignMediaPlayer VERSION 3.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization Flags
if(MSVC)
    add_compile_options(/O2 /GL /W3 /utf-8)
    add_link_options(/LTCG /OPT:REF /OPT:ICF)
else()
    add_compile_options(-O3 -flto -fvisibility=hidden -Wall -Wextra)
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core_engine
    ${CMAKE_CURRENT_SOURCE_DIR}/core_engine/include
)

# ------------------------------------------------------------------------------
# 1. Core Shared Library (libsovereign_core)
# ------------------------------------------------------------------------------
if(ANDROID)
    add_library(sovereign_android SHARED
        core_engine/sovereign_engine.cpp
        android/sovereign_android_jni.cpp
    )
    target_link_libraries(sovereign_android PRIVATE android log)
else()
    add_library(sovereign_core SHARED
        core_engine/sovereign_engine.cpp
    )

    if(WIN32)
        target_compile_definitions(sovereign_core PRIVATE SOVEREIGN_EXPORTS)
        target_link_libraries(sovereign_core PRIVATE ws2_32 user32 shell32)
    elseif(UNIX AND NOT APPLE)
        target_link_libraries(sovereign_core PRIVATE pthread dl m)
    endif()

    # --------------------------------------------------------------------------
    # 2. Cross-Platform Desktop Player Binary (Windows .exe / Linux ELF)
    # --------------------------------------------------------------------------
    add_executable(SovereignPlayer
        frontend_crossplatform/sovereign_player_main.cpp
    )
    target_link_libraries(SovereignPlayer PRIVATE sovereign_core)

    if(WIN32)
        set_target_properties(SovereignPlayer PROPERTIES OUTPUT_NAME "SovereignPlayer")
    endif()
endif()

# Installation Targets
if(NOT ANDROID)
    install(TARGETS sovereign_core SovereignPlayer
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    install(FILES core_engine/include/sovereign_engine.h DESTINATION include)
endif()

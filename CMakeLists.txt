cmake_minimum_required(VERSION 3.19)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
set(C74_MAX_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/c74support)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)

set(PROJECT_NAME midiio)

add_library(
    ${PROJECT_NAME}
    MODULE
    ${PROJECT_NAME}.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${C74_MAX_API_DIR}/max-includes
    ${C74_MAX_API_DIR}/msp-includes
)

target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreMIDI" "-framework CoreFoundation")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)

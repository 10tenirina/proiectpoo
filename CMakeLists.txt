cmake_minimum_required(VERSION 3.26)

set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")

project(${MAIN_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        src/punct.cpp
        src/subiect_vizual.cpp
        src/actor.cpp
        src/recuzita.cpp
        src/decor.cpp
        include/exceptii.h
        include/punct.h
        include/subiect_vizual.h
        include/actor.h
        include/recuzita.h
        include/decor.h
)

set_compiler_flags(RUN_SANITIZERS ${USE_ASAN} TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

###############################################################################

target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE
        ext
)
target_include_directories(${MAIN_EXECUTABLE_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

###############################################################################

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

# tastatura.txt - pentru std::cin (redirectat de scripturile template-ului)
# assets/       - fisiere de date citite cu ifstream (scena.txt etc.)
copy_files(FILES tastatura.txt DIRECTORY assets COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
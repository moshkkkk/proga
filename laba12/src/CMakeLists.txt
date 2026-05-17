cmake_minimum_required(VERSION 3.10)
project(Laba12 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Опция для выбора типа библиотеки 
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Пути для поиска заголовочных файлов
include_directories(include)

# Список исходных файлов
file(GLOB LIB_SOURCES "src/laba12.c")
file(GLOB MAIN_SOURCE "src/main.c")

# Статическая или динамическая библиотеки
if(BUILD_SHARED_LIBS)
    add_library(laba12 SHARED ${LIB_SOURCES})
    message(STATUS "Building SHARED library")
else()
    add_library(laba12 STATIC ${LIB_SOURCES})
    message(STATUS "Building STATIC library")
endif()

# Создаем исполняемый файл
add_executable(program ${MAIN_SOURCE})

# Линкуем библиотеку к исполняемому файлу
target_link_libraries(program laba12)

project(football_team)
cmake_minimum_required(VERSION 2.8)

# Лучше не пользоваться aux_source_directory, а явно объявлять список исходных файлов,
# иначе в проект автоматически добавляются, в том числе, нерелевантные файлы
aux_source_directory(. SRC_LIST)

# В CXX флаги не нужно добавлять
# 1) Макроопределения (-DLOCAL), для этого используется add_definitions
# 2) Опции оптимизации. CMake сам поставит нужные флаги в зависимости от конфигурации
#    сборки (Debug/Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DLOCAL -O2")
add_executable(${PROJECT_NAME} ${SRC_LIST})


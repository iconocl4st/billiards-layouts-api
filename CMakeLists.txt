cmake_minimum_required(VERSION 3.16)
project(layouts_api)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)

add_executable(
        layouts_api

		${CMAKE_CURRENT_SOURCE_DIR}/src/layouts_api.cpp
)

target_include_directories(
	layouts_api
	
	PUBLIC
	/mnt/1f0ab4b3-c472-49e1-92d8-c0b5664f7fdb/ProjectsForFun/Pool/repos/billiards-common/src)


target_link_libraries(
    	layouts_api

	    Threads::Threads
)

install(TARGETS layouts_api DESTINATION /app)


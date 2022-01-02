cmake_minimum_required(VERSION 3.13)
project(layouts_api)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE HINTS /usr/share/cmake/Modules/)

add_executable(
    layouts_api

	${CMAKE_CURRENT_SOURCE_DIR}/src/layouts_api.cpp
)

target_include_directories(
	layouts_api
	
	PUBLIC

	${PROJECT_SOURCE_DIR}/../billiards-common/src
	${PROJECT_SOURCE_DIR}/src
	$ENV{REPOS}/billiards-common/src
)


target_link_libraries(
    layouts_api

	Eigen3::Eigen
	Threads::Threads
	${Boost_SYSTEM_LIBRARY}
	unqlite
)

install(TARGETS layouts_api DESTINATION /app)


cmake_minimum_required (VERSION 3.16.0)

project ("OpenStackup" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Set prefix paths
set(CMAKE_PREFIX_PATH
	${CMAKE_PREFIX_PATH}
	${QT5_PATH}
)

# Include sub-projects.
add_subdirectory("src")

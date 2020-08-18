cmake_minimum_required( VERSION 2.8 )


file(GLOB programs_SOURCES "*.hpp" "*.cpp" "xscreensaver_video/*.hpp" "xscreensaver_video/*.cpp")

set ( CMAKE_CXX_COMPILER "g++" )

# BUG: no usar -Ofast
set( CMAKE_CXX_FLAGS_RELEASE "-Wno-unused-function -Wno-unused-variable -DNDEBUG -D__GNU_SOURCE -DUNICODE -std=c++11 -O2 -Wl,-s")

set( CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -D__GNU_SOURCE -DUNICODE -Wall -std=c++11 -O2 -ggdb -pg -lc")


include (FindPkgConfig)
pkg_search_module(FOX REQUIRED fox)


include_directories(
    ${FOX_INCLUDE_DIRS} 
    )

link_directories(
    ${FOX_LIBRARY_DIRS} 
    )

add_library(programs STATIC ${programs_SOURCES})

target_link_libraries( 
	 programs
     ${FOX_LIBRARIES} 
     ${FOX_STATIC_LIBRARIES} 
    )

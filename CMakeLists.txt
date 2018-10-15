cmake_minimum_required ( VERSION 3.8 )

set ( CMAKE_CXX_STANDARD 17 )

set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "bin/" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/" )

add_subdirectory ( "submodules/dispute/" )
add_subdirectory ( "submodules/termaux/" )

find_package ( Threads REQUIRED )

add_library ( cockpit SHARED
    "source/cockpit.cpp" )

target_include_directories ( cockpit PUBLIC
    "include/"
    "submodules/simple-terminal/include/" )
target_link_libraries ( cockpit
    ${CMAKE_THREAD_LIBS_INIT} )

add_executable ( cockpit_tool
    "source/cockpit_tool.cpp" )

target_include_directories ( cockpit_tool PRIVATE
    "include/"
    "submodules/simple-terminal/include/" )
target_link_libraries ( cockpit_tool
    cockpit
    dispute
    termaux )

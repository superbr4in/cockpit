cmake_minimum_required ( VERSION 3.8 )

set ( CMAKE_CXX_STANDARD 17 )

add_subdirectory ( "submodules/boa/" )

find_package ( Threads )

add_library ( cockpit SHARED
    "source/cockpit.cpp"
    "source/terminal.c"
    "source/terminal.cpp" )

target_include_directories ( cockpit PUBLIC
    "include/"
    ${CMAKE_THREAD_LIBS_INIT} )

add_executable ( cockpit_tool
    "source/auxiliary.cpp"
    "source/cockpit_tool.cpp" )

target_include_directories ( cockpit_tool PRIVATE
    "include/" )
target_link_libraries ( cockpit_tool
    boa
    cockpit )

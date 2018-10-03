cmake_minimum_required ( VERSION 3.8 )

set ( CMAKE_CXX_STANDARD 17 )

add_subdirectory (
    "submodules/boa/" )

find_package ( Threads )

add_executable ( cockpit
    "source/auxiliary.cpp"
    "source/cockpit.cpp"
    "source/command_monitor.cpp" )

target_include_directories ( cockpit PRIVATE
    "include/" )
target_link_libraries ( cockpit
    boa
    ${CMAKE_THREAD_LIBS_INIT} )

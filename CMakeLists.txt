cmake_minimum_required ( VERSION 3.8 )

set ( CMAKE_CXX_STANDARD 17 )

add_subdirectory (
    "submodules/boa/" )

add_executable ( cockpit
    "source/auxiliary.cpp"
    "source/cockpit.cpp"
    "source/updater.cpp" )

target_include_directories ( cockpit PRIVATE
    "include/" )
target_link_libraries ( cockpit
    boa )

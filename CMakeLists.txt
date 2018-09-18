cmake_minimum_required ( VERSION 3.8 )

add_subdirectory (
    "submodules/boa/" )

add_executable ( cockpit
    "source/cockpit.cpp" )

target_link_libraries ( cockpit PRIVATE
    boa )

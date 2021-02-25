# Specifies the CMake version we require
cmake_minimum_required (VERSION 3.8)

# Set our Includes folder as the place to look for library includes
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/Includes/)

# Set C++11 as the language standard
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/Output)

######################################### Engine ###############################################
# Store the name of our project in a variable
set(AutonomicLib "Autonomic")
project (${AutonomicLib} LANGUAGES CXX)

# Add source subdirectory which contains the source files
add_subdirectory(Source)
#add_subdirectory(Resources)

# Specify the name of the project executable and which sources should be used in the project
add_library(${AutonomicLib} STATIC
    ${LIBRARY_SOURCES}
)

source_group( Source FILES ${LIBRARY_SOURCES})

list (APPEND EXPORT_HEADERS ${LIBRARY_PUBLIC_HEADERS})
set_target_properties(${AutonomicLib} PROPERTIES PUBLIC_HEADER "${EXPORT_HEADERS}")

# Specify the libraries to use when linking the executable
target_link_libraries (${AutonomicLib} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/assimp.lib)
target_link_libraries (${AutonomicLib} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/glfw3.lib)
target_link_libraries (${AutonomicLib} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/$<CONFIG>/GDT.lib)

add_custom_command(TARGET ${AutonomicLib} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Resources/"
        ${CMAKE_SOURCE_DIR}/Build/Resources)

install(TARGETS ${AutonomicLib}
        CONFIGURATIONS Debug
        ARCHIVE DESTINATION ${OUTPUT_DIR}/lib/Debug
        PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/include
)
install(TARGETS ${AutonomicLib}
        CONFIGURATIONS Release
        ARCHIVE DESTINATION ${OUTPUT_DIR}/lib/Release
        PUBLIC_HEADER DESTINATION ${OUTPUT_DIR}/include
)

######################################## Applications #########################################
add_subdirectory(Applications/OrbitLine)
#add_dependencies(${AutonomicLib} ${OrbitLine})
add_subdirectory(Applications/Drops)
add_subdirectory(Applications/EvoTest)
add_subdirectory(Applications/EvoTest2)
add_subdirectory(Applications/CircleFluid)
add_subdirectory(Applications/Osmosis)
add_subdirectory(Applications/EMWaves)
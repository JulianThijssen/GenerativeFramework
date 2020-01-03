# Specifies the CMake version we require
cmake_minimum_required (VERSION 3.8)

# Store the name of our project in a variable
set(PROJECT "GenerativeFramework")

project (${PROJECT} LANGUAGES CXX)

# Set our Includes folder as the place to look for library includes
include_directories(${CMAKE_SOURCE_DIR}/3rdParty/Includes/)

######################################### Engine ###############################################
# Add source subdirectory which contains the source files
add_subdirectory(Source/)

# Specify the name of the project executable and which sources should be used in the project
add_executable(${PROJECT}
    ${FRAMEWORK_SOURCES}
)

source_group( Source FILES ${FRAMEWORK_SOURCES})

# Specify the libraries to use when linking the executable
target_link_libraries (${PROJECT} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/assimp.lib)
target_link_libraries (${PROJECT} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/glfw3.lib)
target_link_libraries (${PROJECT} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/Libraries/$<CONFIG>/GDT.lib)

add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Resources/"
        ${CMAKE_SOURCE_DIR}/Build/Resources)

# add_custom_command(TARGET ${PROJECT} POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E copy
        # ${CMAKE_SOURCE_DIR}/Build/$<CONFIG>/Engine.lib
        # "${PROJECT_SOURCE_DIR}/3rdParty/Libraries/$<CONFIG>/Engine.lib")

# file(GLOB H_FILES ${PROJECT_SOURCE_DIR}/Source/*.h)
# foreach(HEADER ${H_FILES})
    # get_filename_component(BARE_HEADER ${HEADER} NAME)
    # message(STATUS ${BARE_HEADER})
    # add_custom_command(TARGET ${ENGINE} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy
            # "${PROJECT_SOURCE_DIR}/Source/Engine/${BARE_HEADER}"
            # "${PROJECT_SOURCE_DIR}/3rdParty/Includes/Engine/${BARE_HEADER}")
# endforeach(HEADER)

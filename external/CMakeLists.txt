# set up the external libraries that aren't popular enough to be found on most 
# package managers are included with the project (in the 'external' folder)

add_subdirectory("nanosvg")
add_subdirectory("pugixml")
add_subdirectory("res2h")

add_custom_target(generate_resources
    ALL
    COMMAND res2h ./resources ./converted -s -h Resources.h -u ResourceUtil.cpp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
    COMMENT "Generating embedded resource files"
    DEPENDS res2h
)

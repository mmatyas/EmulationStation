# set up the external libraries that aren't popular enough to be found on most
# package managers are included with the project (in the 'external' folder)
#
# sqlite is included because although it is in package managers, it is a huge pain to set up on Windows
# (you have to generate the .lib file from the .def file for the downloaded binaries...)

macro(check_submodule_git dir)
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/.git")
    message(WARNING
    "\n=== WARNING ===\n"
    "Looks like you've forgot to clone the dependencies,"
    " as `${CMAKE_CURRENT_SOURCE_DIR}/${dir}` is not a Git repository.\n"
    "Don't worry, just run `git submodule update --init` to fix this.\n\n")
endif()
endmacro()

check_submodule_git(res2h)


add_subdirectory("nanosvg")
add_subdirectory("pugixml")
add_subdirectory("sqlite3")
add_subdirectory("res2h")


add_custom_target(generate_resources
    ALL
    COMMAND res2h ./resources ./converted -s -h Resources.h -u ResourceUtil.cpp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/data
    COMMENT "Generating embedded resource files"
    DEPENDS res2h
)

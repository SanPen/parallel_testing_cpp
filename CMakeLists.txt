#set minimum version for cmake
cmake_minimum_required(VERSION 3.15)

#-----------------------------------------------------------------------------------------------------------------------
# Project definition
#-----------------------------------------------------------------------------------------------------------------------
#set name project and languages
project(newton LANGUAGES C CXX)

#-----------------------------------------------------------------------------------------------------------------------
# Compiler directives (must be after the project definition)
#-----------------------------------------------------------------------------------------------------------------------

# define directives
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(_CRT_SECURE_NO_WARNINGS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # ON means -fPIC, OFF means -fPIE
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

# Compiler options for Visual Studio
# this is only for Visual Studio since set(CMAKE_CXX_STANDARD 17) will not work apparently :/
if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /permissive /EHsc")  # W4 is to add more warnings when compiling
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # this is to be able to use localtime_s and similar without compiler complaints
else()
    # set the debug level of optimization to the lowest
    #    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    #    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

endif()

# Compiler options for GCC
if ( CMAKE_COMPILER_IS_GNUCXX )
    message(Using GCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -g") # Wall and Wextra is to add more warnings when compiling

endif()



message(CMAKE_CXX_FLAGS: (${CMAKE_CXX_COMPILER_ID}) ${CMAKE_CXX_FLAGS})



#shows massage for actual directory
message(This_dir:${CMAKE_CURRENT_LIST_DIR})


#-----------------------------------------------------------------------------------------------------------------------
# Debug / Release configurations
#-----------------------------------------------------------------------------------------------------------------------
message("Compilation type: " ${CMAKE_BUILD_TYPE})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    if(WIN32)
        if (CMAKE_GENERATOR MATCHES "Visual Studio")
            # Do Visual Studio specific stuff
            set(CMAKE_CXX_FLAGS_DEBUG "/bigobj")
        else()
            # Other stuff
        endif()
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    endif()

    message("Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
else()
    if(WIN32)

        if (CMAKE_GENERATOR MATCHES "Visual Studio")
            # Do Visual Studio specific stuff
            set(CMAKE_CXX_FLAGS_DEBUG "/bigobj")
        else()
            # Other stuff
        endif()

    else()
        #        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -ggdb")
    endif()

    add_definitions( -DARMA_NO_DEBUG )

    message("Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})
endif()


add_executable(parallel_testing
        main.cpp
        matrix.cpp
        par_for.cpp
        input_data.cpp input_data.h output_data.cpp output_data.h random.cpp random.h)
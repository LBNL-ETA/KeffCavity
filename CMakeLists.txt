cmake_minimum_required(VERSION 3.14)

set( CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON )

set(target_name KeffCavity)

project(${target_name})

set(CMAKE_PROJECT_VERSION_MAJOR "1")
set(CMAKE_PROJECT_VERSION_MINOR "1")
set(CMAKE_PROJECT_VERSION_PATCH "5")

# Local development mode. The `local` preset sets LBNL_LOCAL_SIBLINGS=ON (a cache
# variable, so it cascades into FetchContent sub-builds). When it is on, prefer a
# sibling working copy (../<Name>) of each of THIS project's direct dependencies,
# so the whole dependency graph can be built from local disk during development.
# A dependency whose sibling is not checked out falls back to its declared remote,
# independently. Propagation: a sibling dependency that also honors this flag wires
# up its own siblings in turn. When the flag is off (e.g. the `default` preset),
# nothing here runs and every dependency comes from its remote.
# Note: each pair is "<folder>=<FETCHCONTENT var suffix>"; they differ for
# Windows-CalcEngine (folder) vs WINDOWSCALC-ENGINE (its declared name, uppercased).
if(LBNL_LOCAL_SIBLINGS)
    foreach(_sibling "Windows-CalcEngine=WINDOWSCALC-ENGINE" "googletest=GOOGLETEST")
        string(REPLACE "=" ";" _sibling "${_sibling}")
        list(GET _sibling 0 _sibling_dir)
        list(GET _sibling 1 _sibling_var)
        if(NOT FETCHCONTENT_SOURCE_DIR_${_sibling_var}
           AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../${_sibling_dir}/CMakeLists.txt")
            set(FETCHCONTENT_SOURCE_DIR_${_sibling_var}
                "${CMAKE_CURRENT_SOURCE_DIR}/../${_sibling_dir}")
            message(STATUS "[local] ${_sibling_dir} -> ${FETCHCONTENT_SOURCE_DIR_${_sibling_var}}")
        endif()
    endforeach()
endif()

include( cmake/KeffCProjectMacros.cmake )
include( cmake/KeffCinternal_utils.cmake )
include( cmake/KeffCCompilerFlags.cmake )

set(CMAKE_CXX_STANDARD 20)

# Enable MSVC parallel (multi-processor) compilation for all targets and
# dependencies defined below. Placed before add_subdirectory/FetchContent so
# every later target inherits it (add_compile_options only affects later targets).
if(MSVC)
    add_compile_options(/MP)
endif()

set(GLOBAL_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Sets global output directory for single configuration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GLOBAL_OUTPUT_PATH})

# google test will not be created by this project if this is not main project. Main project is expected to handle google test.
get_directory_property(hasParent PARENT_DIRECTORY)

if(hasParent)
  set( BUILD_KeffC_TESTING OFF )
  set( DOWNLOAD_GTEST OFF )
  if( BUILD_TESTING STREQUAL ON ) # Parent testing is ON
    set( BUILD_KeffC_TESTING ON )
  endif()
else()
	option( BUILD_KeffC_TESTING "Build testing targets" ON )
	set(DOWNLOAD_GTEST ON)
endif()

include(FetchContent)

if( DOWNLOAD_GTEST STREQUAL ON )
	# Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	if(NOT TARGET gtest)
		FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.17.0
		)
		FetchContent_MakeAvailable(googletest)
	endif()
endif ()

# Test from Windows-CalcEngine should not be visible here
set(BUILD_WCE_TESTING OFF CACHE BOOL "Build Windows-CalcEngine tests" FORCE)
set(WindowsCalcEngine_Branch "Version_1.0.75")

if(NOT TARGET Windows-CalcEngine)
    FetchContent_Declare(
        WindowsCalc-Engine
        GIT_REPOSITORY https://github.com/LBNL-ETA/Windows-CalcEngine.git
        GIT_TAG ${WindowsCalcEngine_Branch}
    )
    FetchContent_MakeAvailable(WindowsCalc-Engine)
endif()

set(KeffCavity_Version "Version_${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")
configure_file(doc/KeffCavity.md.in ${CMAKE_CURRENT_SOURCE_DIR}/doc/KeffCavity.md @ONLY)

file(GLOB SOURCES_HXX "src/*.hxx")
file(GLOB SOURCES_CXX "src/*.cxx")
LIST(APPEND SOURCES ${SOURCES_HXX} ${SOURCES_CXX})

add_library(${target_name} STATIC ${SOURCES})

target_link_libraries(${target_name} PUBLIC Windows-CalcEngine)

# Export branch info for parent projects
set_property(TARGET ${target_name} PROPERTY WindowsCalcEngine_Branch ${WindowsCalcEngine_Branch})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#add_dependencies(${target_name} Windows-CalcEngine)

if( BUILD_KeffC_TESTING )
  	include_directories(src)
	include_directories(include)
  	
  	file( GLOB all_test_src RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "tst/*.cxx" "tst/*.hxx" )
    LIST( APPEND test_src ${all_test_src} )
    
    CREATE_TEST_TARGETS_KeffC( ${target_name} "${test_src}" "" )
  endif()

config_compiler_and_linker_KeffC()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()
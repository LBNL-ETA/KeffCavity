cmake_minimum_required(VERSION 3.8)

set(target_name KeffCavity)

project(${target_name})

include( cmake/KeffCProjectMacros.cmake )
include( cmake/KeffCinternal_utils.cmake )
include( cmake/KeffCCompilerFlags.cmake )

set(CMAKE_CXX_STANDARD 11)

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

if( DOWNLOAD_GTEST STREQUAL ON )
	configure_file(CMakeLists-GTest.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

	# Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add googletest directly to our build. This adds
	# the following targets: gtest, gtest_main, gmock
	# and gmock_main
	add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
			${CMAKE_BINARY_DIR}/googletest-build)

	# Now simply link your own targets against gtest, gmock,
	# etc. as appropriate

	include_directories( ${CMAKE_BINARY_DIR}/googletest-src/include )
endif ()

include(CMakeLists-Windows-CalcEngine.txt)

include_directories( ${CMAKE_BINARY_DIR}/Windows-CalcEngine/include )

file(GLOB SOURCES_HXX "src/*.hxx")
file(GLOB SOURCES_CXX "src/*.cxx")
LIST(APPEND SOURCES ${SOURCES_HXX} ${SOURCES_CXX})

add_library(${target_name} STATIC ${SOURCES})

target_link_libraries(${target_name} Windows-CalcEngine)
add_dependencies(${target_name} Windows-CalcEngine)

if( BUILD_KeffC_TESTING )
  	include_directories(src)
  	
  	file( GLOB all_test_src RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "tst/*.cxx" )
    LIST( APPEND test_src ${all_test_src} )
    
    CREATE_TEST_TARGETS_KeffC( ${target_name} "${test_src}" "" )
  endif()

config_compiler_and_linker_KeffC()
cmake_minimum_required(VERSION 3.25)
project(bbc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

#This is required
include(CheckCXXCompilerFlag)

#A function created to perform the same as in set(CMAKE_CXX...
function(enable_cxx_compiler_flag_if_supported flag)
  string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
  if(flag_already_set EQUAL -1)
    check_cxx_compiler_flag("${flag}" flag_supported)
    if(flag_supported)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
    endif()
    unset(flag_supported CACHE)
  endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

enable_cxx_compiler_flag_if_supported("-O2")
enable_cxx_compiler_flag_if_supported("D_NDEBUG")

find_package(OpenSSL REQUIRED)

find_library(paho-mqttpp3 NAMES libpaho-mqttpp3.a REQUIRED)
find_library(paho-mqtt3a NAMES libpaho-mqtt3a.so REQUIRED)

add_library(pahottpp STATIC IMPORTED)
set_target_properties(pahottpp PROPERTIES IMPORTED_LOCATION ${paho-mqttpp3})

add_library(pahomqtt3a SHARED IMPORTED)
set_property(TARGET pahomqtt3a PROPERTY IMPORTED_LOCATION ${paho-mqtt3a})

#this method points to the include folder for your header files
include_directories(include)

#add subdirectory for the src files
add_subdirectory(src)

CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

file(GLOB SOURCE_FILES "*.cpp")

add_executable(bbc ${SOURCE_FILES} )
target_link_libraries(bbc pahottpp)
target_link_libraries(bbc pahomqtt3a)

install(TARGETS bbc RUNTIME DESTINATION bin)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

file(GLOB SOURCE_FILES "*.cpp")

add_executable(app ${SOURCE_FILES} )
target_link_libraries(app pahottpp)
target_link_libraries(app pahomqtt3a)

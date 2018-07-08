message(STATUS "Creating libs")

execute_process(COMMAND make WORKING_DIRECTORY ${libpifacedigital++_SOURCE_DIR}/3rdparty/libmcp23s17)
execute_process(COMMAND make WORKING_DIRECTORY ${libpifacedigital++_SOURCE_DIR}/3rdparty/libpifacedigital)

message(STATUS "Created libs")

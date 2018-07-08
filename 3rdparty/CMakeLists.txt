include_directories(libmcp23s17/src libpifacedigital/src)

add_library(mcp23s17 STATIC libmcp23s17/src/mcp23s17.c)
add_library(pifacedigital STATIC libpifacedigital/src/pifacedigital.c)
target_link_libraries(pifacedigital mcp23s17)

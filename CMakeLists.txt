cmake_minimum_required(VERSION 2.6)
project(g213manager)


add_executable(g213manager g213Manager.cpp G213KeybLedManager.cpp)
target_link_libraries(g213manager usb-1.0)


install(TARGETS g213manager RUNTIME DESTINATION bin)

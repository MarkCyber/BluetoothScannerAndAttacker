cmake_minimum_required(VERSION 3.13)
project(flipper_zero_bt_tool)

add_executable(flipper_zero_bt_tool
    src/main.c
    src/bt_scan.c
    src/bt_capture.c
    src/bt_exploit.c
    src/bt_command.c
)
target_include_directories(flipper_zero_bt_tool PRIVATE include)

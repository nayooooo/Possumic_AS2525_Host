file(GLOB_RECURSE HOST_DRIVER_SRCS
    "host_driver/*.c"
)

set(HOST_DRIVER_INCLUDE_DIRS
    "host_driver/cmd_api"
)

list(FILTER HOST_DRIVER_SRCS EXCLUDE REGEX "^host_driver/porting/psic/")

idf_component_register(
    SRCS
        ${HOST_DRIVER_SRCS}
    INCLUDE_DIRS
        ${HOST_DRIVER_INCLUDE_DIRS}
)

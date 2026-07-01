file(GLOB_RECURSE HOST_DRIVER_SRCS
    CONFIGURE_DEPENDS
    "host_driver/*.c"
)

set(HOST_DRIVER_INCLUDE_DIRS
    "host_driver/cmd_api"
)

list(FILTER HOST_DRIVER_SRCS EXCLUDE REGEX "^host_driver/porting/psic/")
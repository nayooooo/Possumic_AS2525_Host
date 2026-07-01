include(HostDriver.cmake)

idf_component_register(
    SRCS
        ${HOST_DRIVER_SRCS}
    INCLUDE_DIRS
        "./"
        ${HOST_DRIVER_INCLUDE_DIRS}
)

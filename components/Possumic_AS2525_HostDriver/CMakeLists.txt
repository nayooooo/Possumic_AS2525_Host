include(HostDriver.cmake)

idf_component_register(
    SRCS
        "Possumic_AS2525_HostDriver.c"
        ${HOST_DRIVER_SRCS}
    INCLUDE_DIRS
        "include"
        ${HOST_DRIVER_INCLUDE_DIRS}
)

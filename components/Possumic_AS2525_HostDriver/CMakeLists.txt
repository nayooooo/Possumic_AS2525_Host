file(GLOB_RECURSE HOST_DRIVER_SRCS
    "host_driver/*.c"
)

set(HOST_DRIVER_INCLUDE_DIRS
    "host_driver/cmd_api"
)

idf_component_register(
    SRCS
        ${HOST_DRIVER_SRCS}
    INCLUDE_DIRS
        "."
        ${HOST_DRIVER_INCLUDE_DIRS}
    PRIV_REQUIRES
        freertos esp_timer esp_driver_gpio
)

target_compile_options(
    ${COMPONENT_LIB} PRIVATE
    -Wno-implicit-fallthrough
    -Wno-unused-function
    -Wno-format
)

cmake_minimum_required(VERSION 3.4.1)

add_library(
        qrencode
        SHARED
        qrencode.c

        libqrencode/bitstream.c
        libqrencode/mask.c
        libqrencode/mmask.c
        libqrencode/mqrspec.c
        libqrencode/qrenc.c
        libqrencode/qrencode.c
        libqrencode/qrinput.c
        libqrencode/qrspec.c
        libqrencode/rsecc.c
        libqrencode/split.c

)

find_library(
        log-lib

        log)

target_link_libraries(
        qrencode
        jnigraphics
        ${log-lib})
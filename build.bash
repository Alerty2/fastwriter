gcc fastwriter.c json/cJSON.c\
    -o fastwriter \
    -I libuiohook/dist/include \
    -L libuiohook/dist/lib \
    -Wl,-rpath,'$ORIGIN/libuiohook/dist/lib' \
    -luiohook \
    -lm
#!/usr/bin/env bash

meson setup --reconfigure build
meson compile -C build
# ninja -C build
# cd build && ninja
./build/cpp_template
# meson test -C build

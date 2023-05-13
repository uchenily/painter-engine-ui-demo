#!/usr/bin/env bash

pip install -U meson ninja pre-commit
pre-commit install

clangd --version
clang-tidy --version
clang-format --version

# 集成第三方库
# mkdir subprojects
# meson wrap install fmt

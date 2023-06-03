#!/usr/bin/env bash
#
for i in ../PainterEngine/core/*.c; do
  echo $i
done

for i in ../PainterEngine/kernel/*.c; do
  echo $i
done

for i in ../PainterEngine/architecture/*.c; do
  echo $i
done

for i in project/*.c; do
  echo $i
done

for i in platform/webassembly/*.c; do
  echo $i
done

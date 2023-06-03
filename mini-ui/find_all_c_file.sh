#!/usr/bin/env bash
#
for i in core/*.c; do
  echo $i
done

for i in kernel/*.c; do
  echo $i
done

for i in architecture/*.c; do
  echo $i
done

for i in project/*.c; do
  echo $i
done

for i in platform/webassembly/*.c; do
  echo $i
done

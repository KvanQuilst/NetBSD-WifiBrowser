#!/bin/sh
#
# Surf API / surf_test.c
# Copyright (c) 2022 Dylan Eskew, Stephen Loudiana, Kevin McGrane
# * This software is under the terms of the BSD license.  * See README for more details.
#
# Test script for the Surf API

while getopts d flag
do
  case "${flag}" in
    d) debug="debug";;
  esac
done

cd ../src/surf
make
cd ../../test
make $debug

./surf
make clean
#!/bin/sh

LD_LIBRARY_PATH=./bin:${LD_LIBRARY_PATH} PATH=${PATH}:./bin runcible-lobby $*

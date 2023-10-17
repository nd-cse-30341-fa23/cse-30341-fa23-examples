#!/bin/sh

export LD_PRELOAD=$(pwd)/libmalloc1.so
exec $@

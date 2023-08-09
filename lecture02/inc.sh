#!/bin/sh

[ ! -r count ] && echo 0 > count

while true; do
    c=$(cat count)
    echo $(($c + 1)) | tee count
    sleep 1
done

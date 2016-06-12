#!/bin/bash

mount|grep -q $(pwd)/tmp || exit 0
echo "Are you sure? ENTER to proceed, CTRL-C to abort!"
read
sudo umount tmp
rm -rf tmp

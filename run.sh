#!/bin/sh

sudo ./topdeb&
TOPDEBPID=$!
watch --interval 1 cat topdeb.out
sudo kill $TOPDEBPID

#!/bin/sh

sudo ./topdeb -o topdeb.out -t 50&
TOPDEBPID=$!
watch --interval 1 cat topdeb.out
sudo kill $TOPDEBPID

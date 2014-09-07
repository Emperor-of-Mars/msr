#!/bin/sh

rm -rf "msr_save";
tar cf "msr_save" libs res src msr_test.h main.cpp msr.cbp save.sh

# date +'%m-%d-%Y//%r'
#!/bin/bash

gcc -o hw4 hw4.c -lblas -llapack && ./hw4 "$1"
python3 hw4_plot.py
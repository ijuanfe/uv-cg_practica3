#!/bin/bash
rm main 2>/dev/null
g++ *.cpp -o main -lGL -lGLU -lglut -lm
./main

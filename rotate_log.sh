#!/bin/bash
timestamp=$(date +%d-%m-%Y_%H:%M:%S)
zip $timestamp logs.txt
rm logs.txt 
touch logs.txt
#!/bin/bash
timestamp=$(date +%d-%m-%Y_%H:%M:%S)
zip $timestamp /tmp/lab6_logs.log
rm /tmp/lab6_logs.log
touch /tmp/lab6_logs.log
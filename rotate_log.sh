#!/bin/bash
timestamp=$(date +%d-%m-%Y_%H:%M:%S)
zip $timestamp /tmp/my_services_log.txt
rm /tmp/logs.txt 
touch /tmp/my_services_log.txt
#!/bin/bash

TRUE=/usr/bin/true
LOGFILE=/Volumes/BigTemp/btlog/thelog.txt
SLEEP_INTERVAL_IN_SECONDS=40

touch  "${LOGFILE}"

while $TRUE ; do
    TIMESTAMP="$(date)"
    echo "Trying at ${TIMESTAMP}"
    echo "\n== ${TIMESTAMP}">> "${LOGFILE}"
    curl   --connect-timeout 40 http://10.0.0.35/bluetooth-device-report >> "${LOGFILE}"
    sleep $SLEEP_INTERVAL_IN_SECONDS
done    

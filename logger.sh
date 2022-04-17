#!/bin/bash

TRUE=/usr/bin/true
LOGFILE=/Volumes/BigTemp/btlog/thelog.txt
SLEEP_INTERVAL_IN_SECONDS=10

DEVICES="10.0.0.36 10.0.0.35"

touch  "${LOGFILE}"

while $TRUE ; do

    for DEVICE in $DEVICES ; do
       QUERY_URL="http://${DEVICE}/bluetooth-device-report"	
       TIMESTAMP="$(date)"
       echo "Trying '$QUERY_URL' at ${TIMESTAMP}"
       echo >> "${LOGFILE}"    
       echo "== $DEVICE  ${TIMESTAMP}">> "${LOGFILE}"
       curl   --connect-timeout 30 ${QUERY_URL} >> "${LOGFILE}"
    done
    sleep $SLEEP_INTERVAL_IN_SECONDS
done    

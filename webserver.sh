#!/bin/bash

# A very simple webserver, used only to fake a real webserver for testing purposes.

while true; do echo -e "HTTP/1.1 200 OK\n\n $(date)" | nc -l 0.0.0.0 3000; done

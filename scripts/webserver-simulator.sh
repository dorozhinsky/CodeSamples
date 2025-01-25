#!/bin/bash

#This command will:
#- Use `nc` to listen on port 8080.
#- Serve the contents of `response.html` to any incoming HTTP request.
#- The `-q 1` option ensures that `nc` will close the connection 1 second after the response is sent.
#You can now open a web browser and navigate to `http://localhost:8080` to see the "Hello, World!" message served by your simulated web server.

set -euo pipefail

LISTEN_PORT="8080"

if [ $# -lt 1 ]; then
    echo "WebServer simulator will use default port: "$LISTEN_PORT
else
    LISTEN_PORT=$1
    echo "WebServer simulator will use port: "$LISTEN_PORT
fi

while true; do
    echo "Generating simple response HTML file..."
    echo -e "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Welcome to $(uname --all)</h1><h2>Previous s erver connection date: $(date)</h2></body></html>" > response.html
    echo "Waiting for the connection..."
nc -l -p $LISTEN_PORT -q 1 < response.html  
    echo "The response was sent"
    rm response.html
done

#!/bin/bash

#This command will:
#- Use `nc` to listen on port 8080.
#- Serve the contents of `response.html` to any incoming HTTP request.
#- The `-q 1` option ensures that `nc` will close the connection 1 second after the response is sent.
#You can now open a web browser and navigate to `http://localhost:8080` to see the "Hello, World!" message served by your simulated web server.

set -euo pipefail

LISTEN_PORT="8080"
readonly RESPONSE_FILE="/tmp/webserver-simulator-response.html"

trap "echo 'Got SIGINT, terminating...'; rm ${RESPONSE_FILE}; exit" SIGINT
trap "echo 'Got SIGTERM, terminating...'; rm ${RESPONSE_FILE}; exit" SIGTERM

echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Starting WebServer simulator..."

if [ $# -lt 1 ]; then
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - WebServer simulator will use default port: "$LISTEN_PORT
else
    LISTEN_PORT=$1
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - WebServer simulator will use port: "$LISTEN_PORT
fi

while true; do
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Generating simple response HTML file at ${RESPONSE_FILE} ..."
    echo -e "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\
        <html><head>\
            <meta http-equiv=\"refresh\" content=\"300\">\
        </head><body>\
            <h1>Welcome to $(uname --all)</h1>\
            <h2>Last data update time: $(date +"%Y-%m-%d %H:%M:%S.%3N %Z")</h2>\
            <p>Server uptime: $(uptime)</p>\
            <h2>Server load:</h2>\
            <pre>$(top -b -n 1)</pre>\
        </body></html>" > $RESPONSE_FILE
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Waiting for the connection..."
    set +e
timeout 30 nc -l -p $LISTEN_PORT -q 1 < $RESPONSE_FILE
    if [ $? -eq 0 ]; then
        echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - The response was sent"
    else
        echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - The connection timed out or failed"
    fi
set -e          
done

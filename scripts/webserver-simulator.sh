#!/bin/bash

set -euo pipefail

LISTEN_PORT="8080"
RESPONSE_FILE=$(mktemp /tmp/webserver-simulator-response.XXXXXX.html)

cleanup() {
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Cleaning up..."
    rm -f "$RESPONSE_FILE"
}
trap cleanup EXIT
trap "echo 'Got SIGINT, terminating...'; exit" SIGINT
trap "echo 'Got SIGTERM, terminating...'; exit" SIGTERM

echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Starting WebServer simulator..."

if [ $# -lt 1 ]; then
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - WebServer simulator will use default port: $LISTEN_PORT"
else
    LISTEN_PORT=$1
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - WebServer simulator will use port: $LISTEN_PORT"
fi

# Check if the port is available
if lsof -iTCP:$LISTEN_PORT -sTCP:LISTEN >/dev/null 2>&1; then
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Error: Port $LISTEN_PORT is already in use."
    exit 1
fi

while true; do
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Generating simple response HTML file at $RESPONSE_FILE ..."
    echo -e "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n\
        <html><head>\
            <meta http-equiv=\"refresh\" content=\"300\">\
        </head><body>\
            <h1>Welcome to $(uname --all)</h1>\
            <h2>Last data update time: $(date +"%Y-%m-%d %H:%M:%S.%3N %Z")</h2>\
            <p>Server uptime: $(uptime)</p>\
            <h2>Server load:</h2>\
            <pre>$(cat /proc/loadavg)</pre>\
        </body></html>" > "$RESPONSE_FILE"
    echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - Waiting for the connection..."
    set +e
    timeout 30 nc -l -p "$LISTEN_PORT" -q 1 < "$RESPONSE_FILE"
    if [ $? -eq 0 ]; then
        echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - The response was sent"
    else
        echo "$(date +"%Y-%m-%d %H:%M:%S.%3N %Z") - The connection timed out or failed"
    fi
    set -e
done

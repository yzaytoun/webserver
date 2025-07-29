#!/bin/bash

function test_get1() {
    file_name=$1
    echo "open localhost 8080"
    sleep 1
    echo "GET $file_name HTTP/1.1"
    echo "Host: localhost"
    echo "Connection: close"
    echo
    echo
    sleep 1
}

function test_request_template() {
    echo "open localhost 8080"
    # here echo's for request message
    echo
    echo
    sleep 1
}

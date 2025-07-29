#!/bin/bash

path=$1

function test_configuration() {
    echo $path
    docker run --rm -t -a stdout --name test_config \
     -v $path:/etc/nginx/:ro \
     nginx:latest \
     nginx -c /etc/nginx/config.conf -t
}

test_configuration
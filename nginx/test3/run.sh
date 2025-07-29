#!/bin/bash

action=$1

case "$action" in
    build)
    docker run -d --name test3 \
        -p 8080:80 \
        -v ./test3/default:/etc/nginx/sites-available/default:ro \
        -v ./test3/start.sh:/start.sh:ro \
        --entrypoint "/start.sh" nginx:latest > /dev/null 2>/dev/null
    sleep 1
        ;;
    clean)
    docker stop test3  > /dev/null 2>/dev/null
	docker rm test3  > /dev/null 2>/dev/null
    docker rmi nginx  > /dev/null 2>/dev/null
        ;;
    *)
    echo ""
        ;;
esac
#!/bin/bash

if [ ! -f /etc/nginx/sites-available/default ]; then
  echo "NOT FOUND: sites-available!"
  exit 1
fi
# Configure container
sed -i "s/nginx/webserver/g" /usr/share/nginx/html/index.html

# Run nginx

nginx -g "daemon off;"

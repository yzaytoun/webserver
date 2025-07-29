#!/bin/bash

if [ ! -f /etc/nginx/sites-available/default ]; then
  echo "NOT FOUND: sites-available!"
  exit 1
fi
# Configure container

# Run nginx

nginx -g "daemon off;"

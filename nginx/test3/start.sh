#!/bin/bash

# Create the YoupiBanane directory
mkdir -p ~/YoupiBanane/nop
mkdir -p ~/YoupiBanane/Yeah

# Create the files in the main directory
touch ~/YoupiBanane/youpi.bad_extension
touch ~/YoupiBanane/youpi.bla

# Create files in the 'nop' subdirectory
touch ~/YoupiBanane/nop/youpi.bad_extension
touch ~/YoupiBanane/nop/other.pouic

# Create files in the 'Yeah' subdirectory
touch ~/YoupiBanane/Yeah/not_happy.bad_extension

# Example command for downloading
wget http://example.com/cgi_test -O ~/cgi_test
chmod +x ~/cgi_test
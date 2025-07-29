#!/bin/bash

source requests.sh

test1()  {
    ./test1/run.sh build
    echo "Build Test1"
    test_get1 /index.html | telnet
    sleep 1
    # Other test here
    ./test1/run.sh clean
}

test2() {
    ./test2/run.sh build
    echo "Build Test2"
    test_get1 /index.html | telnet
    sleep 1
    # Other test here
    ./test2/run.sh clean
}

test3() {
    
}

#test1
#test2
test3
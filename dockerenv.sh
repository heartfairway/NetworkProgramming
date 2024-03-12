#!/bin/bash

echo_command() {
    echo "Usage: [sudo] ${0} {command} [parameter ...]"
    echo "  "
    echo "    <command>"
    echo "      setup                     Setup build environment image"
    echo "      new [container_name]      Create new build working container"
    echo "      resume [container_name]   Resume a previous created build working container"
}

if [ $# -lt 1 ]; then
    echo_command
    exit 0
fi

if [ "$1" = "setup" ]; then
    docker build -t np_buildenv -f buildenv.dockerfile . --no-cache

elif [ "$1" = "new" ]; then
    if [ $# -lt 2 ]; then
        docker run --name np_practice -v $(pwd):/root/host -it np_buildenv bash
    else
        docker run --name $2 -v $(pwd):/root/host -it iii5gc_buildenv bash
    fi

elif [ "$1" = "resume" ]; then
    if [ $# -lt 2 ]; then
        docker start -i np_practice
    else
        docker start -i $2
    fi

else
    echo_command
    exit 0
fi

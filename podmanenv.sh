#!/bin/bash

echo_command() {
    echo "Usage: ${0} {command} [parameter ...]"
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
    podman build -t np_buildenv -f buildenv.containerfile . --no-cache

elif [ "$1" = "new" ]; then
    if [ $# -lt 2 ]; then
        podman run --name np_practice --cap-add=NET_ADMIN --cap-add=NET_RAW -v $(pwd):/root/host -it np_buildenv bash
    else
        podman run --name $2 --cap-add=NET_ADMIN --cap-add=NET_RAW -v $(pwd):/root/host -it np_buildenv bash
    fi

elif [ "$1" = "resume" ]; then
    if [ $# -lt 2 ]; then
        podman start -ai np_practice
    else
        podman start -ai $2
    fi

else
    echo_command
    exit 0
fi

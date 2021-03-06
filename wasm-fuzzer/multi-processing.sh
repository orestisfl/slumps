#!/bin/bash

NUM_INSTANCES=$1
shift;  # Pop $1 from argument list to pass all arguments "$@" to slumps/wafl:latest

if [ -z "$NUM_INSTANCES" ]; then
    echo "Specifiy the number of AFL instances as argument!"
    exit 1
fi

echo "Running $NUM_INSTANCES AFL instance(s)."

CURRENT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

set -a
source $CURRENT_DIR/.env
set +a

# Make sure we're starting a fresh test here
rm -R $CURRENT_DIR/wafl-temp

echo "Running #1"
docker run --env-file=./.env \
    -e MASTER_AFL_NODE=True \
    -e WAFL_INSTANCE_ID=1 \
    -v maven_data:/root/.cache/coursier/v1/https/repo1.maven.org/maven2 \
    -v compiled_sources:/home/server/src/out/ \
    -v ${LOCAL_WASM_DIR:?err}:/home/server/wasm/ \
    -v ${PWD}/wafl-temp/afl-out:/home/client/out/ \
    -v ${PWD}/wafl-temp/logs/1:/home/shared/logs/ \
    -d wafl:latest $@

if [ $NUM_INSTANCES -lt 2 ]; then
    exit 0
fi

for i in $(seq 2 $NUM_INSTANCES); do
    echo "Waiting for previous mill server to compile..."
    sleep 30s
    echo "Running #${i}"
    docker run --env-file=./.env \
        -e MASTER_AFL_NODE=False \
        -e WAFL_INSTANCE_ID=${i} \
        -v maven_data:/root/.cache/coursier/v1/https/repo1.maven.org/maven2 \
        -v compiled_sources:/home/server/src/out/ \
        -v ${LOCAL_WASM_DIR:?err}:/home/server/wasm/ \
        -v ${PWD}/wafl-temp/afl-out:/home/client/out/ \
        -v ${PWD}/wafl-temp/logs/${i}:/home/shared/logs/ \
        -d wafl:latest $@
done
exit 0

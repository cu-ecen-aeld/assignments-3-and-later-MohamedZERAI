#!/bin/sh
# Tester script for assignment 1 and assignment 2
# Author: Siddhant Jajoo

set -e
set -u

NUMFILES=10
WRITESTR=AELD_IS_FUN
WRITEDIR=/tmp/aeld-data
username=$(cat /etc/finder-app/conf/username.txt)

# Optional args
if [ $# -lt 3 ]; then
    echo "Using default values for number of files and string to write"
    if [ $# -ge 1 ]; then
        NUMFILES=$1
    fi
else
    NUMFILES=$1
    WRITESTR=$2
    WRITEDIR=/tmp/aeld-data/$3
fi

MATCHSTR="The number of files are ${NUMFILES} and the number of matching lines are ${NUMFILES}"

echo "Writing ${NUMFILES} files containing string '${WRITESTR}' to '${WRITEDIR}'"

rm -rf "${WRITEDIR}"
mkdir -p "${WRITEDIR}"

if [ ! -d "${WRITEDIR}" ]; then
    echo "Failed to create directory: ${WRITEDIR}"
    exit 1
fi

# Write files
for i in $(seq 1 "$NUMFILES"); do
    writer "${WRITEDIR}/${username}$i.txt" "${WRITESTR}"
done

# Run finder and store output
OUTPUTSTRING=$(finder.sh "$WRITEDIR" "$WRITESTR")

echo "$OUTPUTSTRING" > /tmp/assignment4-result.txt

# Clean up
rm -rf /tmp/aeld-data

# Validate result
set +e
echo "$OUTPUTSTRING" | grep "$MATCHSTR" > /dev/null
if [ $? -eq 0 ]; then
    echo "success"
    exit 0
else
    echo "failed: expected '${MATCHSTR}' in output but got:"
    echo "$OUTPUTSTRING"
    exit 1
fi

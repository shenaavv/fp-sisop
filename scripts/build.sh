#!/bin/bash

echo "=== Named Pipes Communication System ==="
echo "Building the programs..."

# Compile server and client
gcc -o server src/server.c
gcc -o client src/client.c

if [ $? -eq 0 ]; then
    echo "✓ Programs compiled successfully!"
    echo ""
    echo "Usage:"
    echo "1. Run './scripts/run_server.sh' in one terminal"
    echo "2. Run './scripts/run_client.sh' in another terminal"
    echo "3. Start communicating!"
    echo ""
    echo "Files created:"
    echo "- server (executable)"
    echo "- client (executable)"
    echo "- history.log (will be created when programs run)"
else
    echo "✗ Compilation failed!"
    exit 1
fi

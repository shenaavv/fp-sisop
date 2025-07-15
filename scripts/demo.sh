#!/bin/bash

echo "=== Demo Named Pipes Communication System ==="
echo ""

# Clean up any existing files
rm -f server client history.log

echo "1. Building programs..."
gcc -o server src/server.c
gcc -o client src/client.c

if [ $? -ne 0 ]; then
    echo "✗ Build failed!"
    exit 1
fi

echo "✓ Programs compiled successfully!"
echo ""

echo "2. Demo will show automated communication..."
echo "   Starting server in background..."

# Start server in background and redirect output
./server > server_output.log 2>&1 &
SERVER_PID=$!

# Wait a moment for server to start
sleep 1

echo "✓ Server started (PID: $SERVER_PID)"
echo ""

echo "3. Starting client with automated messages..."

# Create automated client input
echo -e "Hello Server!\nHow are you?\nThis is a test message\nexit" | ./client > client_output.log 2>&1

echo "✓ Client communication completed"
echo ""

echo "4. Stopping server..."
kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

echo "✓ Server stopped"
echo ""

echo "5. Communication log (history.log):"
echo "=================================="
if [ -f history.log ]; then
    cat history.log
else
    echo "No history.log file found!"
fi

echo ""
echo "=== Demo completed ==="

# Cleanup
rm -f server client server_output.log client_output.log

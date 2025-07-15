#!/bin/bash

echo "=== Named Pipes Communication System Test ==="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_TOTAL=0

run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_result="$3"
    
    ((TESTS_TOTAL++))
    echo -n "Test $TESTS_TOTAL: $test_name... "
    
    if eval "$test_command"; then
        echo -e "${GREEN}PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}FAILED${NC}"
    fi
}

# Cleanup function
cleanup() {
    pkill -f "./server" 2>/dev/null
    rm -f server client history.log /tmp/namedpipe_a19
    sleep 1
}

# Initial cleanup
cleanup

echo "Setting up test environment..."

# Test 1: Compilation
run_test "Compilation" "gcc -o server src/server.c && gcc -o client src/client.c" 0

# Test 2: Server executable exists
run_test "Server executable" "[ -f server ]" 0

# Test 3: Client executable exists  
run_test "Client executable" "[ -f client ]" 0

# Test 4: Server starts successfully
echo ""
echo "Starting integration tests..."

# Start server in background
./server > /dev/null 2>&1 &
SERVER_PID=$!
sleep 2

run_test "Server startup" "ps -p $SERVER_PID > /dev/null" 0

# Test 5: Named pipe creation
run_test "Named pipe creation" "[ -p /tmp/namedpipe_a19 ]" 0

# Test 6: Client can connect and communicate
echo -e "test message\nexit" | timeout 5 ./client > /dev/null 2>&1
run_test "Client communication" "[ $? -eq 0 ]" 0

# Test 7: History log creation
run_test "History log creation" "[ -f history.log ]" 0

# Test 8: History log contains client messages
run_test "Client logging" "grep -q 'CLIENT-SENT' history.log" 0

# Wait a moment and cleanup
sleep 1
cleanup

echo ""
echo "=== Test Results ==="
echo -e "Tests passed: ${GREEN}$TESTS_PASSED${NC}/$TESTS_TOTAL"

if [ $TESTS_PASSED -eq $TESTS_TOTAL ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed! ✗${NC}"
    exit 1
fi

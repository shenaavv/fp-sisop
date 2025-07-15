CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=server client
SRC_DIR=src
SCRIPTS_DIR=scripts

.PHONY: all build clean demo install help test

all: $(TARGETS)

build: $(TARGETS)
	@mkdir -p src/output
	@echo "Build completed successfully!"

server: $(SRC_DIR)/server.c
	$(CC) $(CFLAGS) -o server $(SRC_DIR)/server.c

client: $(SRC_DIR)/client.c
	$(CC) $(CFLAGS) -o client $(SRC_DIR)/client.c

clean:
	rm -f server client history.log src/output/history.log /tmp/namedpipe_c2s /tmp/namedpipe_s2c
	@echo "Cleaned"

demo: all
	@echo "Running demo..."
	@chmod +x $(SCRIPTS_DIR)/demo.sh
	@./$(SCRIPTS_DIR)/demo.sh

test: all
	@echo "Running tests..."
	@chmod +x $(SCRIPTS_DIR)/test.sh
	@./$(SCRIPTS_DIR)/test.sh

install: all
	@echo "Making scripts executable..."
	@chmod +x $(SCRIPTS_DIR)/*.sh

help:
	@echo "Available targets:"
	@echo "  all     - Build server and client"
	@echo "  build   - Same as 'all'"
	@echo "  clean   - Remove built files and logs"
	@echo "  demo    - Run automated demo"
	@echo "  test    - Run test suite"
	@echo "  install - Make scripts executable"
	@echo "  help    - Show this help"

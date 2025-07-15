#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define PIPE_C2S "/tmp/namedpipe_c2s"
#define PIPE_S2C "/tmp/namedpipe_s2c"
#define BUFFER_SIZE 256
#define LOG_FILE "src/output/history.log"

int pipe_write_fd = -1;
int pipe_read_fd = -1;
FILE *log_file;

void cleanup(int sig)
{
    (void)sig; // Suppress unused parameter warning
    printf("\nClient shutting down...\n");
    if (pipe_write_fd != -1)
    {
        close(pipe_write_fd);
    }
    if (pipe_read_fd != -1)
    {
        close(pipe_read_fd);
    }
    if (log_file)
    {
        fclose(log_file);
    }
    exit(0);
}

void log_message(const char *side, const char *message)
{
    if (log_file)
    {
        fprintf(log_file, "[%s] %s\n", side, message);
        fflush(log_file);
    }
}

int main()
{
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Set up signal handler for graceful shutdown
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    // Open log file
    log_file = fopen(LOG_FILE, "a");
    if (!log_file)
    {
        perror("Failed to open log file");
        exit(1);
    }

    printf("Client: Connecting to server...\n");

    // Open pipes
    pipe_write_fd = open(PIPE_C2S, O_WRONLY);
    if (pipe_write_fd == -1)
    {
        perror("Failed to open write pipe. Make sure server is running.");
        fclose(log_file);
        exit(1);
    }

    pipe_read_fd = open(PIPE_S2C, O_RDONLY);
    if (pipe_read_fd == -1)
    {
        perror("Failed to open read pipe. Make sure server is running.");
        close(pipe_write_fd);
        fclose(log_file);
        exit(1);
    }

    printf("Client: Connected to server!\n");
    printf("Client: Type 'exit' to quit\n");
    log_message("CLIENT", "Connected to server");

    while (1)
    {
        // Get message from user
        printf("Client: Enter message: ");
        fflush(stdout);

        if (fgets(message, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }

        // Remove newline character
        message[strcspn(message, "\n")] = 0;

        // Send message to server
        if (write(pipe_write_fd, message, strlen(message)) == -1)
        {
            perror("Failed to write to pipe");
            break;
        }

        log_message("CLIENT-SENT", message);

        // Check for exit command
        if (strcmp(message, "exit") == 0)
        {
            printf("Client: Exiting...\n");
            log_message("CLIENT", "Exiting");
            break;
        }

        // Read response from server
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(pipe_read_fd, buffer, BUFFER_SIZE - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';
            printf("Server replied: %s\n", buffer);
            log_message("CLIENT-RECEIVED", buffer);
        }
        else if (bytes_read == 0)
        {
            printf("Client: Server disconnected\n");
            log_message("CLIENT", "Server disconnected");
            break;
        }
        else
        {
            perror("Failed to read from pipe");
            break;
        }
    }

    cleanup(0);
    return 0;
}

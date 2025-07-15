#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define PIPE_C2S "/tmp/namedpipe_c2s"
#define PIPE_S2C "/tmp/namedpipe_s2c"
#define BUFFER_SIZE 256
#define LOG_FILE "src/output/history.log"

int pipe_read_fd = -1;
int pipe_write_fd = -1;
FILE *log_file;

void cleanup(int sig)
{
    (void)sig; // Suppress unused parameter warning
    printf("\nServer shutting down...\n");
    if (pipe_read_fd != -1)
    {
        close(pipe_read_fd);
        unlink(PIPE_C2S);
    }
    if (pipe_write_fd != -1)
    {
        close(pipe_write_fd);
        unlink(PIPE_S2C);
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

    // Remove any existing pipes
    unlink(PIPE_C2S);
    unlink(PIPE_S2C);

    // Create named pipes
    if (mkfifo(PIPE_C2S, 0666) == -1)
    {
        perror("mkfifo PIPE_C2S failed");
        fclose(log_file);
        exit(1);
    }

    if (mkfifo(PIPE_S2C, 0666) == -1)
    {
        perror("mkfifo PIPE_S2C failed");
        unlink(PIPE_C2S);
        fclose(log_file);
        exit(1);
    }

    printf("Server: Named pipes created.\n");
    printf("Server: Waiting for client connection...\n");

    // Open pipes
    pipe_read_fd = open(PIPE_C2S, O_RDONLY);
    if (pipe_read_fd == -1)
    {
        perror("Failed to open read pipe");
        cleanup(0);
    }

    pipe_write_fd = open(PIPE_S2C, O_WRONLY);
    if (pipe_write_fd == -1)
    {
        perror("Failed to open write pipe");
        cleanup(0);
    }

    printf("Server: Client connected!\n");
    log_message("SERVER", "Client connected");

    while (1)
    {
        // Read message from client
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(pipe_read_fd, buffer, BUFFER_SIZE - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';

            // Check for exit command
            if (strcmp(buffer, "exit") == 0)
            {
                printf("Server: Client requested exit\n");
                log_message("SERVER", "Client requested exit");
                break;
            }

            printf("Server received: %s\n", buffer);
            log_message("SERVER-RECEIVED", buffer);

            // Echo back to client
            if (write(pipe_write_fd, buffer, strlen(buffer)) == -1)
            {
                perror("Failed to write to pipe");
                break;
            }

            printf("Server sent: %s\n", buffer);
            log_message("SERVER-SENT", buffer);
        }
        else if (bytes_read == 0)
        {
            printf("Server: Client disconnected\n");
            log_message("SERVER", "Client disconnected");
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

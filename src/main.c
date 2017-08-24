/*
 *
 *  CMSC 23300 / 33300 - Networks and Distributed Systems
 *
 *  main() code for chirc project
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "log.h"

typedef struct user {
    char *nick;
    char *username;
    char *full_name;
} user;

struct sockaddr_in init_socket(int port) {
    struct sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return addr;
}

void handle_nick_msg(char *msg) {
    printf("Received nick command!\n");
}

void handle_user_msg(char *msg) {
    printf("Received user command!\n");
}

void accept_user(int port) {
    int sockfd, replysockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = init_socket(port);
    // TODO: error checking?
    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sockfd, 5);

    struct sockaddr_in cli_addr;
    socklen_t client_addr_len = sizeof(cli_addr);

    char buffer[256];
    while (1) {
        replysockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client_addr_len);
        read(replysockfd, buffer, 255);
        printf("Received message: %s\n", buffer);
        char *command = strtok(buffer, " ");
        if (strcmp(command, "NICK") == 0) {
            handle_nick_msg(strtok(NULL, " "));
        } else if (strcmp(command, "USER") == 0) {
            handle_user_msg(strtok(NULL, " "));
        } else if (strcmp(command, "EXIT") == 0) { // for debugging only
            break;
        } else {
            printf("Received unknown command %s\n", command);
        }
        close(replysockfd);
    }
    close(sockfd);
}

int main(int argc, char *argv[]) {
    int opt;
    char *port = "6667", *passwd = NULL;
    int verbosity = 0;

    while ((opt = getopt(argc, argv, "p:o:vqh")) != -1)
        switch (opt)
        {
        case 'p':
            port = strdup(optarg);
            break;
        case 'o':
            passwd = strdup(optarg);
            break;
        case 'v':
            verbosity++;
            break;
        case 'q':
            verbosity = -1;
            break;
        case 'h':
            fprintf(stderr, "Usage: chirc -o PASSWD [-p PORT] [(-q|-v|-vv)]\n");
            exit(0);
            break;
        default:
            fprintf(stderr, "ERROR: Unknown option -%c\n", opt);
            exit(-1);
        }

    if (!passwd)
    {
        fprintf(stderr, "ERROR: You must specify an operator password\n");
        exit(-1);
    }

    /* Set logging level based on verbosity */
    switch(verbosity)
    {
    case -1:
        chirc_setloglevel(QUIET);
        break;
    case 0:
        chirc_setloglevel(INFO);
        break;
    case 1:
        chirc_setloglevel(DEBUG);
        break;
    case 2:
        chirc_setloglevel(TRACE);
        break;
    default:
        chirc_setloglevel(INFO);
        break;
    }

	/* Your code goes here */
    accept_user(atoi(port));
	return 0;
}


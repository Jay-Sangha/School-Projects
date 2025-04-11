/**
 * @File httpserver.c
 *
 * This file contains the main function for the HTTP server.
 *
 * @author [Jayden Sangha]
 */

#include "asgn2_helper_funcs.h"
#include "debug.h"
#include "protocol.h"

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 4096




struct Response {
	char version[16];
	int status_code;
	char status_phrase[32];
	char header[19];
	long len;
	char message[64];
};


struct Request {
	char method[10];
	char uri[64];
	char version[16];
	char header[50];
	char value[50];
	int off_set;
	int err_flag;
	int len;
	int size;
};


uint16_t strtouint16(char number[]) {
    char *last;
    long num = strtol(number, &last, 10);
    if (num <= 0 || num > UINT16_MAX || *last != '\0') {
        return 0;
    }
    return num;
}



struct Request process(char req_buf[]) {
	struct Request req;

	char buf[1024] = "";

	strcpy(req.method, "");
	strcpy(req.uri, "");
	strcpy(req.version, "");
	strcpy(req.header, "");
	strcpy(req.value, "");


	const char dl[2] = "\n";

	char *token;

	int total = 0;

	char phead[25] = "";

	char pval[24] = "";

	int check = 0;

	req.err_flag = 0;

	int ctr = 0;

	req.size = strlen(req_buf);


	token = strtok(req_buf, dl);

	strcpy(buf, token);


	if (token == NULL) {
		req.err_flag = 5;

		return req;
	}


	if ((check = sscanf(buf, "%s /%s %s %n", req.method, req.uri, req.version, &req.off_set)) != 3) {


		strcpy(req.method, "");
		strcpy(req.version, "");
		strcpy(req.uri, "");

		sscanf(buf, "%s %s %s", req.method, req.uri, req.version);

		if (strcmp(req.version, "HTTP/1.1") != 0) {
			dprintf(STDERR_FILENO, "Wrong version rightly indentified\n");
			req.err_flag = 5;
			return req;

		}


		if (strcmp(req.uri, "/") == 0 && strcmp(req.version, "HTTP/1.1") == 0) {

			req.err_flag = 2;

			strcpy(buf, "");
			dprintf(STDERR_FILENO, "At flag 2 in process request\n");

			return req;
		}

		req.err_flag = 1;

		strcpy(buf, "");

		return req;
	}

	if (strcmp(req.version, "HTTP/1.1") != 0) {
		dprintf(STDERR_FILENO, "Wrong version rightly indentified new\n");
		req.err_flag = 5;

		return req;
	}


	if (!((strcmp(req.method, "GET") == 0 | strcmp(req.method, "get") == 0) || (strcmp(req.method, "PUT") == 0 | strcmp(req.method, "put") == 0))) {

		req.err_flag = 501;
		return req;

	}


	token = strtok(NULL, dl);

	total = req.off_set;

	while (token != NULL) {

		strcpy(buf, token);

		sscanf(buf, "%s %s %n", req.header, req.value, &req.off_set);


		char temp_1[25] = "";
		char temp_2[25] = "";
		char temp_3[25] = "";


		if (sscanf(buf, "%s %s %s", temp_1, temp_2, temp_3) == 3) {

			req.err_flag = 1;

			strcpy(buf, "");
			strcpy(temp_1, "");
			strcpy(temp_2, "");
			strcpy(temp_3, "");

			return req;
		}


		if (strcmp(req.header, "") == 0 && strcmp(req.value, "") == 0) {

			if (ctr == 0 && (strcmp(req.method, "GET") != 0 | strcmp(req.method, "get") != 0)) {
				req.err_flag = 1;

				strcpy(buf, "");

				return req;
			}


			break;
		}

		if (strcmp(req.header, "Content-Length:") == 0) {

			strcpy(phead, req.header);

			char temp1[25] = "";
			char temp2[25] = "";
			char temp3[25] = "";

			int bad = 0;


			if (sscanf(buf, "%s %s %s", temp1, temp2, temp3) == 3) {
				bad  = 1;

				strcpy(temp1, "");
				strcpy(temp2, "");
				strcpy(temp3, "");
			}


			int val_ln = strlen(req.value);

			int i;

			for (i = 0; i < val_ln; i++) {

				if (!isdigit(req.value[i])) {
					bad = 1;

					break;

				}
			}

			
			if (strcmp(req.value, "") == 0 | bad == 1) {
				req.err_flag = 1;

				strcpy(buf, "");

				return req;
			}

			strcpy(pval, req.value);
		}


		if (strcmp(req.header, "") == 0) {

			req.err_flag = 1;

			strcpy(buf, "");

			return req;
		}


		strcpy(req.header, "");
		strcpy(req.value, "");

		token = strtok(NULL, dl);

		total += req.off_set;

		ctr += 1;

	}



	strcpy(req.header, phead);
	strcpy(req.value, pval);

	req.off_set = total + 8;


	if (ctr == 1) {

		req.off_set -= 4;
	}


	req.off_set -= 1;

	if (req.off_set > 2048) {
		req.err_flag = 7;
	}


	req.len = atoi(pval);

	return req;

}




int get(int connfd, char file[]) {

	
	struct Response res;
	struct stat st = {0};

	stat(file, &st);

	int size = st.st_size;

	strcpy(res.status_phrase, "");

	strcpy(res.message, "");
	strcpy(res.header, "");
	strcpy(res.version, "");

	if (access(file, F_OK) != 0) {
		return 3;
	}

	if (access(file, R_OK) != 0) {
                return 4;
        }

	if (S_ISDIR(st.st_mode) != 0) {
                return 4;
        }




	int fd = open(file, O_RDONLY);

	if (fd < 0) {
		return 4;
	}

	res.status_code = 200;

	strcpy(res.version, "HTTP/1.1");
	strcpy(res.status_phrase, "OK");

	res.len = size;

	strcpy(res.header, "Content-Length");


	dprintf(connfd, "%s %d %s\r\n%s: %ld\r\n\r\n", res.version, res.status_code, res.status_phrase, res.header, res.len);

	strcpy(res.status_phrase, "");
	strcpy(res.message, "");


	int passed = pass_n_bytes(fd, connfd, size);

	if (passed < 0) {
		errx(EXIT_FAILURE, "pass bytes returned -1");
	}


	close(fd);
	return 0;

}


int put(int connfd, char file[], struct Request req, char buff[], int bread) {

	struct Response res;
	int fd;
	struct stat ln = { 0 };

	stat(file, &ln);


	if (S_ISDIR(ln.st_mode)) {
		return 4;
	}

	strcpy(res.header, "");
    	strcpy(res.message, "");
    	strcpy(res.status_phrase, "");
    	strcpy(res.version, "");

    	strcpy(res.version, "HTTP/1.1");


	if (access(file, F_OK) == 0) {
		
		if (access(file, W_OK) != 0) {
			return 4;
		}


		fd = open(file, O_WRONLY | O_TRUNC);
		res.status_code = 200;

		strcpy(res.status_phrase, "OK");

		res.len = 3;

		strcpy(res.message, "OK\nn");
		strcpy(res.header, "Content-Length");
	}

	else {

		fd = open(file, O_WRONLY | O_CREAT, 0777);
		res.status_code = 201;

		strcpy(res.status_phrase, "Created\n");

		res.len = 8;

		strcpy(res.message, "Created\n");
		strcpy(res.header, "Content-Length");
	}


	if (req.len <= (bread - req.off_set)) {
		int ctr = 0;
		char pbuf[4096] = "";

		while (ctr <= (bread - req.off_set)) {
			pbuf[ctr] = buff[(req.off_set) + ctr];
			ctr += 1;

		}

		if ((write_n_bytes(fd, pbuf, req.len)) < 0) {
			errx(1, "fail in writing\n");
		}


		close(fd);


		dprintf(connfd, "%s %d %s\r\n%s: %ld\r\n\r\n%s", res.version, res.status_code, res.status_phrase, res.header, res.len, res.message);

	}


	else {
		int ctr = 0;

		char pbuf[4096] = "";

		while (ctr <= (bread - req.off_set)) {
			pbuf[ctr] = buff[(req.off_set) + ctr];

			ctr += 1;

		}


		int written = write_n_bytes(fd, pbuf, (bread - req.off_set));

		pass_n_bytes(connfd, fd, req.len - written);


		close(fd);

		dprintf(connfd, "%s %d %s\r\n%s: %ld\r\n\r\n%s\n", res.version, res.status_code, res.status_phrase, res.header, res.len, res.message);

	}

	return 0;



}





/** @brief Handles a connection from a client.
 *
 *  @param connfd The file descriptor for the connection.
 *
 *  @return void
 */
void handle_connection(int connfd) {
    /* Your code here */

	char buff[BUFFER_SIZE] = "";

	
	int bread = read(connfd, buff, BUFFER_SIZE);


	struct Request req;

	req = process(buff);

	
	if (req.err_flag == 1) {
		dprintf(connfd, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");

		return;
	}

	if (req.err_flag == 5) {
		int num1;
		int num2;

		sscanf(req.version, "HTTP/%d.%d", &num1, &num2);


		if (num1 >= 10 | num2 >= 10) {

			dprintf(connfd, "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n");

			return;
		}


		dprintf(connfd, "HTTP/1.1 505 Version Not Supported\r\nContent-Length: 22\r\n\r\nVersion Not Supported\n");

		return;
	}

	if (req.err_flag == 501) {

		dprintf(connfd, "HTTP/1.11 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n");

		return;
	}

	if ((strcmp(req.method, "GET") == 0) | (strcmp(req.method, "get") == 0)) {

       		int num;
        	num = get(connfd, req.uri);
        	if (num == 3) {
            		dprintf(connfd, "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n");
        	}
        	if (num == 4) {
            		dprintf(connfd, "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n");
        	}
    	}


	if (strcmp(req.method, "PUT") == 0 | strcmp(req.method, "put") == 0) {

        	int num;

        	num = put(connfd, req.uri, req, buff, bread);

        	if (num == 3) {
            		dprintf(connfd, "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n");
        	}
        	if (num == 4) {
            		dprintf(connfd, "HTTP/1.1 403 Forbidden\r\nContent-Length: 9\r\n\r\nForbidden\n");
        	}
    	}




    close(connfd);
    return;
}

/** @brief Main function for the HTTP server.
 *
 *  @param argc The number of arguments.
 *  @param argv The arguments.
 *
 *  @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        warnx("wrong arguments: %s port_num", argv[0]);
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr = NULL;
    size_t port = (size_t) strtoull(argv[1], &endptr, 10);

    /* Add error checking for the port number */

    if (port < 0 | port > 65535) {
	    fprintf(stderr, "Invalid Port\n");
    }



    signal(SIGPIPE, SIG_IGN);
    Listener_Socket sock;
    listener_init(&sock, port);

    while (1) {
        int connfd = listener_accept(&sock);
        handle_connection(connfd);
    }

    return EXIT_SUCCESS;
}

#include "HTTPRequest.h"
#include "../DataStructures/Lists/Queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int method_select(char *method) {
	if (strcmp(method, "GET") == 0) {
		return GET;
	} else if (strcmp(method, "POST") == 0) {
		return POST;
	} else if (strcmp(method, "PUT") == 0) {
		return PUT;
	} else if (strcmp(method, "PATCH") == 0) {
		return PATCH;
	} else if (strcmp(method, "DELETE") == 0) {
		return DELETE;
	} else if (strcmp(method, "HEAD") == 0) {
		return HEAD;
	} else if (strcmp(method, "TRACE") == 0) {
		return TRACE;
	} else if (strcmp(method, "CONNECT") == 0) {
		return CONNECT;
	} else if (strcmp(method, "OPTIONS") == 0) {
		return OPTIONS;
	} else {
		return -1;
	}
}

struct HTTPRequest http_request_constructor(char *request_string) {
	struct HTTPRequest request;
	char *request_line = request_string;
	char *header_fields = NULL;
	char *body = NULL;

	char *header_end = strstr(request_string, "\r\n\r\n");
	if (header_end) {
		body = header_end + 4;
		*header_end = '\0';
	}

	header_fields = strstr(request_string, "\r\n");
	if (header_fields) {
		*header_fields = '\0';
		header_fields += 2;
	}

	char *method = strtok(request_line, " ");
	request.Method = method_select(method);
	char *URI = strtok(NULL, " ");
	request.URI = URI;
	char *HTTPVersion = strtok(NULL, "/");
	HTTPVersion = strtok(NULL, "/");
	request.HTTPVersion = (float)atof(HTTPVersion);
	request.header_fields = dictionary_constructor(compare_string_keys);

	struct Queue headers = queue_constructor();
	char *token = strtok(header_fields, "\r\n");
	while (token) {
		headers.push(&headers, token, strlen(token) + 1);
		token = strtok(NULL, "\r\n");
	}

	char *header = (char *)headers.peek(&headers);
	while (header) {
		char *key = strtok(header, ":");
		char *value = strtok(NULL, "|");
		request.header_fields.insert(&request.header_fields, key,
									 strlen(key) + 1, value, strlen(value) + 1);
		headers.pop(&headers);
		header = (char *)headers.peek(&headers);
	}

	printf(
		"\nRequest line\nReq type : %s\nReq path : %s\nhttp version : %.1f\n\n",
		method, request.URI, request.HTTPVersion);
	printf("Headers:\n");
	// printf("%d", request.header_fields.keys.length);
	for(int i=0 ; i<request.header_fields.keys.length ; i++){
		printf("%s\n", request.header_fields.keys.retrieve(&request.header_fields.keys, i));
	}
	if (body)
		printf("Body:\n%s\n\n", body);
	return request;
}
#ifndef GLOBAL_H_DEF
#define GLOBAL_H_DEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include <unistd.h>

// const char* URL = "https://192.168.2.4:4567/checkForAuthToken?level=1";
char *URL;
static size_t foundBT = 3;
size_t numHeaders = 1;
char *headers;

int bluescan(char *host, char *level, char *hmac, char *certlocation);

void setFoundBT(size_t _foundBT);
size_t getFoundBT(void);

void updateHeaderSize(const size_t size);
void increaseNumHeaders();
size_t getNumHeaders();

char* getHeaders();
void setHeader(const char *header, size_t index);
void appendHeader(const char *header);

static void checkForBTResult();
static size_t header_handler(void *buffer, size_t size, size_t nmemb, void *userdata);

void setURL(char *hostname, char *level, char *hmac);
char* getURL();

#endif

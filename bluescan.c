#include "bluescan.h"

/* SKIP hostname authentication due to local enviroment - and self-signed Certificates */
#define SKIP_HOSTNAME_VERIFICATION

// int main(int argc, char **argv) {
int bluescan(char *host, char *level, char *hmac, char *certlocation) {

	CURL *curl;
  CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if(curl) {

		setURL(host, level, hmac);

		// char *localUrl = (char *) malloc(1 + strlen(getURL()));
		printf("URL curl: %s\n", getURL());

		curl_easy_setopt(curl, CURLOPT_URL, URL);

		/* Certificate check */
		curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
		curl_easy_setopt(curl, CURLOPT_CAINFO, certlocation);

		/* Connection will will terminate by certificate miss-match  */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

		/* SKIP hostname authentication due to local enviroment - and self-signed Certificates */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		/* get headers */
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_handler);

	    /* Perform the request, res will get the return code */
	    res = curl_easy_perform(curl);

		/* Check for errors */
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			return 0;     // curl was unsuccessful -> deny access
		} else {
			checkForBTResult();
    }

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return 1;        // curl was sucessful
}

static size_t header_handler(void *buffer, size_t size, size_t nmemb, void *userdata) {

	const size_t realsize = size * nmemb;
	char *header = malloc(size+1);
	memcpy(header, buffer, realsize);

	printf("from buffer: %s\n", buffer);
	printf("from header: %s\n", header);
	appendHeader(header);

	return realsize;
}

void appendHeader(const char *header) {
	if(getNumHeaders() != 1) {
		increaseNumHeaders();
		updateHeaderSize(getNumHeaders());	// preserve old headers, add one more field
		setHeader(header, getNumHeaders() - 1);
	} else {
		setHeader(header, 0);
	}

	printf("proof of append: %s", header);
}

size_t getNumHeaders() {	return numHeaders;	}

void increaseNumHeaders() {	numHeaders += 1;	}

void updateHeaderSize(const size_t size) {
	char *oldHeaders;
	memcpy(oldHeaders, getHeaders(), strlen(getHeaders()));

	free(headers);
	headers = (char *) malloc(size * sizeof(char));

	setHeader(oldHeaders, size - 1);
	free(oldHeaders);
}

char* getHeaders() {	return headers;	}

void setHeader(const char *header, size_t index) {
	memcpy(&headers[index], header, strlen(header));
}

static void checkForBTResult() {
	int i = 0;
	const char *tempHeader = getHeaders();
	while(i < strlen(tempHeader)) { // loop through array by vector size
		printf(" %s\n", tempHeader[i]);
		if(&tempHeader[i] == "foundBT") { // search for foundBT
			// check for next field and set it either false
			if(&tempHeader[i + 1] == "true") {	// next word equals true - found
				printf("bt true\n");
				setFoundBT(1);
				return;
			} else if(&tempHeader[i + 1] == "false") {// next word equals false - not found
				printf("bt false\n");
				setFoundBT(0);
				return;
			} else {  		// anything else than true|false found
				printf("something weird happend");
				setFoundBT(3);
				return;
			}
		}

		// couldn't find foundBT (yet) in the whole vector
		printf("nope not yet found");
		setFoundBT(3);
		i += 1;
	}
}

void setFoundBT(size_t _foundBT) {	foundBT = _foundBT;	}

size_t getFoundBT(void) {	return foundBT;	}

char* getURL() { return URL; }

void setURL(char *host, char *level, char *hmac) {

	char *https = "https://";
	char *ref = "/checkForAuthToken";
	char *refLevel = "?level=";
	char *refHMAC = "?hmac=";

	char *temp;

	if(strcmp(level, "3") == 0) {
		if(hmac == NULL) {
			return;
		}

		temp = (char *) malloc(1 + strlen(host) + strlen(ref) + strlen(refLevel) + strlen(level) + strlen(refHMAC) + strlen(hmac));

	} else {
		temp = (char *) malloc(1 + strlen(host) + strlen(ref) + strlen(refLevel) + strlen(level));

	}

	strcpy(temp, https);
	strcat(temp, host);
	strcat(temp, ref);
	strcat(temp, refLevel);
	strcat(temp, level);

	if(strcmp(level, "3") == 0) {
		strcat(temp, refHMAC);
		strcat(temp, hmac);
	}
	// temp[strlen(temp)] = '\0';

	printf("temp: %s\n", temp);

	URL = (char *) malloc( sizeof(char) + strlen(temp));
	// printf("len URL: %s\n", strlen(URL));


	memcpy(URL, temp, strlen(temp));
	printf("URL: %s\n", URL);
}

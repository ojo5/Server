#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback function to handle response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    printf("Response data: %.*s\n", (int)total_size, (char *)contents);
    return total_size;
}

// Function to perform the GET request
void perform_get_request(const char *url) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the GET request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    // Cleanup global libcurl resources
    curl_global_cleanup();
}

int main(void) {
    // URL for the GET request
    const char *url = "http://127.0.0.1:8080";

    // Perform the GET request
    perform_get_request(url);

    return 0;
}
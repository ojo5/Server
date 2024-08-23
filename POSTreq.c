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

// Function to perform the POST request
void perform_post_request(const char *url, const char *post_data) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Set the URL for the POST request
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the POST request
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
    const char *url = "http://127.0.0.1:12345";
    const char *post_data = "param1=value1&param2=value2";

    // Perform the POST request
    perform_post_request(url, post_data);

    return 0;
}
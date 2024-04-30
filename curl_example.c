#include <curl/curl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct curl_res_memory {
    char *memory;
    size_t size;
};

static size_t _curl_fetch_result_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct curl_res_memory *mem = (struct curl_res_memory *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) { // out of memory! 
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

int main(void)
{
    struct curl_res_memory curl_data;
    CURL *curl_handle;
    CURLcode res;

    curl_data.memory=NULL;
    curl_data.size=0;

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPALIVE, 1L); // set keep alive for persistence connection
    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPIDLE, 50L);
    curl_easy_setopt(curl_handle, CURLOPT_TCP_KEEPINTVL, 30L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, _curl_fetch_result_cb);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&curl_data);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl/agent");

    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://www.tku.edu.tw/");
    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else {
        // do something with curl_data
        printf("Got %d byte(s) of response.\n================================\n", curl_data.size);
        printf("\n%s\n", curl_data.memory);
    }

    curl_easy_cleanup(curl_handle);

    return 0;
}

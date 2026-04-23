#include <fastcgi/fcgi_stdio.h>
#include <stdlib.h>

int main (void) {
    char *str, buf[512];
    int len;

    while (FCGI_Accept() >= 0) {
        printf("Status: 200 OK\r\n");
        printf("Content-type: text/plain\r\n\r\n");
        printf("FastCGI Works (simple version)\n");

        // --
	printf("------\n");

        str=getenv("REQUEST_URI");
        if(str) printf("REQUEST_URI : %s\n", str);
        else printf("REQUEST_URI is null\n", str);

        str=getenv("QUERY_STRING");
        if(str) printf("QUERY_STRING : %s\n", str);
        else printf("QUERY_STRING is null\n", str);

        str=getenv("SCRIPT_NAME");
        if(str) printf("SCRIPT_NAME : %s\n", str);
        else printf("SCRIPT_NAME is null\n", str);

        str=getenv("SCRIPT_FILENAME");
        if(str) printf("SCRIPT_FILENAME : %s\n", str);
        else printf("SCRIPT_FILENAME is null\n", str);

        str=getenv("REQUEST_METHOD");
        if(str) printf("REQUEST_METHOD : %s\n", str);
        else printf("REQUEST_METHOD is null\n", str);

        str=getenv("CONTENT_TYPE");
        if(str) printf("CONTENT_TYPE : %s\n", str);
        else printf("CONTENT_TYPE is null\n", str);

        str=getenv("CONTENT_LENGTH");
        if(str) {
            printf("CONTENT_LENGTH : %s\n", str);
            len=atoi(str);
        }
        else {
            printf("CONTENT_LENGTH is null\n", str);
            len=atoi(str);
        }

        // --
	printf("\n");

        str=getenv("SERVER_NAME");
        if(str) printf("SERVER_NAME : %s\n", str);
        else printf("SERVER_NAME is null\n", str);

        str=getenv("SERVER_ADDR");
        if(str) printf("SERVER_ADDR : %s\n", str);
        else printf("SERVER_ADDR is null\n", str);

	str=getenv("SERVER_PORT");
        if(str) printf("SERVER_PORT : %s\n", str);
        else printf("SERVER_PORT is null\n", str);

        // --
        printf("\n");

        str=getenv("REMOTE_ADDR");
        if(str) printf("REMOTE_ADDR : %s\n", str);
        else printf("REMOTE_ADDR is null\n", str);

        str=getenv("REMOTE_PORT");
        if(str) printf("REMOTE_PORT : %s\n", str);
        else printf("REMOTE_PORT is null\n", str);

        // --
        printf("------\n");

        if(len > 0) {
            FCGI_fread(buf, sizeof(char), len, FCGI_stdin);
            buf[len]='\0';
    
	    printf("%s\n", buf);
        }
        else {
            printf("(no content body)\n");
        }
    }

    return EXIT_SUCCESS;
}

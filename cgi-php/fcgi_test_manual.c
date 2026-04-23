//#include <fastcgi/fcgi_stdio.h>
#include <fastcgi/fcgiapp.h>
#include <stdio.h>
#include <stdlib.h>

int main (void) {
    char *str, rx_buf[512];
    int sock, len;
    FCGX_Request request;

    FCGX_Init();
    sock=FCGX_OpenSocket("/tmp/fcgi_test_manual.sock", 16);

    FCGX_InitRequest(&request, sock, 0);


    while(1) {
        FCGX_Accept_r(&request);
	
        /* 
        // Warp stdin, stdout, stderr for FCGI_stdio, a simple implement method.
        // Check source of fcgi_stdio and fcgiapp for more details.
        FCGI_stdin->stdio_stream = NULL; // for conventional CGI
        FCGI_stdin->fcgx_stream = request.in; // for FastCGI
        FCGI_stdout->stdio_stream = NULL; // for conventional CGI
        FCGI_stdout->fcgx_stream = request.out; // for FastCGI
        FCGI_stderr->stdio_stream = NULL; // for conventional CGI
        FCGI_stderr->fcgx_stream = request.err; // for FastCGI
        */

	printf("Processing request (ID=%d) ...\n", request.requestId);

        FCGX_FPrintF(request.out, "Status: 200 OK\r\n");
        FCGX_FPrintF(request.out, "Content-type: text/plain\r\n\r\n");
        FCGX_FPrintF(request.out, "FastCGI Works (manual version)\n");

        // --
	FCGX_FPrintF(request.out,"------\n");

        FCGX_FPrintF(request.out,"#Request ID : %d\n\n", request.requestId);

        str=FCGX_GetParam("REQUEST_URI", request.envp);
        if(str) FCGX_FPrintF(request.out,"REQUEST_URI : %s\n", str);
        else FCGX_FPrintF(request.out,"REQUEST_URI is null\n", str);

        str=FCGX_GetParam("QUERY_STRING", request.envp);
        if(str) FCGX_FPrintF(request.out, "QUERY_STRING : %s\n", str);
        else FCGX_FPrintF(request.out, "QUERY_STRING is null\n", str);

        str=FCGX_GetParam("SCRIPT_NAME", request.envp);
        if(str) FCGX_FPrintF(request.out, "SCRIPT_NAME : %s\n", str);
        else FCGX_FPrintF(request.out, "SCRIPT_NAME is null\n", str);

        str=FCGX_GetParam("SCRIPT_FILENAME", request.envp);
        if(str) FCGX_FPrintF(request.out, "SCRIPT_FILENAME : %s\n", str);
        else FCGX_FPrintF(request.out, "SCRIPT_FILENAME is null\n", str);

        str=FCGX_GetParam("REQUEST_METHOD", request.envp);
        if(str) FCGX_FPrintF(request.out, "REQUEST_METHOD : %s\n", str);
        else FCGX_FPrintF(request.out, "REQUEST_METHOD is null\n", str);

        str=FCGX_GetParam("CONTENT_TYPE", request.envp);
        if(str) FCGX_FPrintF(request.out, "CONTENT_TYPE : %s\n", str);
        else FCGX_FPrintF(request.out, "CONTENT_TYPE is null\n", str);

        str=FCGX_GetParam("CONTENT_LENGTH", request.envp);
        if(str) {
            FCGX_FPrintF(request.out, "CONTENT_LENGTH : %s\n", str);
            len=atoi(str);
        }
        else {
            FCGX_FPrintF(request.out, "CONTENT_LENGTH is null\n", str);
            len=atoi(str);
        }

        // --
	FCGX_FPrintF(request.out, "\n");

        str=FCGX_GetParam("SERVER_NAME", request.envp);
        if(str) FCGX_FPrintF(request.out, "SERVER_NAME : %s\n", str);
        else FCGX_FPrintF(request.out, "SERVER_NAME is null\n", str);

        str=FCGX_GetParam("SERVER_ADDR", request.envp);
        if(str) FCGX_FPrintF(request.out, "SERVER_ADDR : %s\n", str);
        else FCGX_FPrintF(request.out, "SERVER_ADDR is null\n", str);

	str=FCGX_GetParam("SERVER_PORT", request.envp);
        if(str) FCGX_FPrintF(request.out, "SERVER_PORT : %s\n", str);
        else FCGX_FPrintF(request.out, "SERVER_PORT is null\n", str);

        // --
        FCGX_FPrintF(request.out,"\n");

        str=FCGX_GetParam("REMOTE_ADDR", request.envp);
        if(str) FCGX_FPrintF(request.out, "REMOTE_ADDR : %s\n", str);
        else FCGX_FPrintF(request.out, "REMOTE_ADDR is null\n", str);

        str=FCGX_GetParam("REMOTE_PORT", request.envp);
        if(str) FCGX_FPrintF(request.out, "REMOTE_PORT : %s\n", str);
        else FCGX_FPrintF(request.out, "REMOTE_PORT is null\n", str);

        // --
        FCGX_FPrintF(request.out, "------\n");

        if(len > 0) {
            //FCGI_fread(buf, sizeof(char), len, FCGI_stdin);
            FCGX_GetStr(rx_buf, len, request.in);
            rx_buf[len]='\0';
    
	    FCGX_FPrintF(request.out,"%s\n", rx_buf);
        }
        else {
            FCGX_FPrintF(request.out, "(no content body)\n");
        }

        FCGX_Finish_r(&request);
    }

    return EXIT_SUCCESS;
}

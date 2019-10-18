// there are several many libraries for the job

// 2015510012 Furkan Ayas
//Note: The WS can't open the images, i ask my friends the problem and they say they had too
//sorry for the problem
// Also i looked different sources and internet, this variables are generally using ones
// also i make mini chat and OK server-client (not for web) system in begining the assigment

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>
#include <time.h>
#include <sys/sendfile.h>


//it's a simple technic of define a little html page in a char. These tpyes project it help us
//to skip the file check part for external file operations 
char webpage[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
"<title>AyasWS</title>\r\n"
"</head>\r\n"
"<body>\r\n"
"<h3><font color =\"blue\">DEU CENG 2015510012 Furkan AYAS</font></h3><br>\r\n"
"<img src=\"deu.jpg\" alt = \"Deu Ceng\">\r\n"
"</body>\r\n"
"</html>\r\n";


#define MYPORT 9999
//port never change after steps, we shold define first stpe
int main(int argc, char *argv[]) //standart main
{


	pthread_t threads[10];
	int thread_count = 0;


    //sock define
    struct sockaddr_in server_addr,client_addr;
    socklen_t sin_len = sizeof(client_addr);
    int fd_server, fd_client;
    char buf[2048]; //can change with different size
    int fdimg;
    int on = 1;

    fd_server = socket(AF_INET,SOCK_STREAM, 0);
    if(fd_server < 0) // less than zero situations says it's a error.Exit end the program
    {
        perror("socket");
        exit(1);
    }

    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	//like oop system, server_addr has these attribuates. Equals has means.
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(MYPORT);

    if(bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind"); // perror for errors, different than printf
        close(fd_server);
        exit(1);
    }



    if(listen(fd_server, 10) == -1)
    {
        perror("listen");
        close(fd_server);
        exit(1);
    }
	

/*
Little part of i tried external before interval html, i put this part for a reason
it show the 404 NOT FOUND

htmlfile = fopen("/Desktop/h2/index.html","r");
fputs(htmlpage,htmlfile);
fwrite(htmlpage,sizeof(htmlpage),10,htmlfile);

if(htmlfile == NULL)
{
	send_buf = (char *)malloc(24);
	strcpy(send_buf, "HTTP/1.1 404 Not Found\n\n");
	send(*connfd_thread,send_buf,24,0);
	perror("htmlfile");
}
else
{
	time(&local);
}


*/

    while(1)
    {
	// i tried the add threads after localserver opens but it always
	// looks 1, it didnt increase, i cant make the thread part.
	// it make server system stop when thread count 10, because i can't make
	// client number over 10, it always safety number when i tried
	// and i add this type manuel system
	thread_count++;

	printf("<-----%d server_count----->\n",fd_server);
	printf("<-----%d client_count----->\n",fd_client);
	printf("<-----%d thread_count----->\n",thread_count);
	if(thread_count >= 10)
	{
		perror("Safety");
		printf("<-Safety vailation..!->");
		exit(1);
	}


        fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

        if(fd_client == -1)
        {
            perror("<-Connetion fail->\n");
            continue;
        }
        printf("<- Client connect successfully ->\n");

        if(!fork())
        {
            //child process
            close(fd_server);
            memset(buf, 0, 2048);
            read(fd_client, buf, 2047);

            printf("%s\n", buf);


		//i tried different ways on the web but they didnt working to open images
		//you can change easly the image's names and you should look ls-l the size of
	   	 //image and change the fourth size variable.
            if(!strncmp(buf, "GET /deu.ico", 16))
            {
                fdimg = open("deu.ico", O_RDONLY);
                sendfile(fd_client, fdimg, NULL, 60000);
                close(fdimg);
            }
	    else if(!strncmp(buf, "GET /deu.jpg", 16))
            {
                fdimg = open("deu.jpg", O_RDONLY);
                sendfile(fd_client, fdimg, NULL, 12000);
                close(fdimg);
            }
            else
                write(fd_client, webpage, sizeof(webpage) -1);

            close(fd_client);
            printf("Closing\n");
            exit(0);

        }
        //parent process
        close(fd_client);
    }


    return 0;
}

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace httpxx {
    namespace client {
        /// Make a GET request to provided hn:port
        char get(char server, char path) {
            // server is an hostname:port combo
            // right now this is only HTTP/0.9, expansion soon.
            if (server) {
                // here we know that server isn't null (likely)
                // now to verify path isn't null
                if (path) {
                    // here we know that path isn't null
                    // begin attempting a connection to the provided Hostname:Port combo via TCP
                    int sockfd, portno, n;
                    struct sockaddr_in serv_addr;
                    struct hostent *server;
                    string serverHostname;
                    serverHostname = server; // hmm, how to split...
                    char buffer[256]; // We should make this dynamic, according to Content-Length header

                    portno = 1234; // replace with the correct port soon

                    /* Create a socket point */
                    sockfd = socket(AF_INET, SOCK_STREAM, 0);

                    if (sockfd < 0) {
                        perror("ERROR Opening socket");
                        exit(1);
                    }
                    
                    // Resolve hostname to IP  
                    server = gethostbyname(serverHostname.c_str()); 
                    if (server == NULL) {
                        printf("ERROR, No such host!\n");
                        exit(0);
                    }
                    
                    bzero((char *) &serv_addr, sizeof(serv_addr));
                    // This is IPv4, TO-DO: Switch to both v4 and v6. For chief: look at your notes on how to do it.
                    serv_addr.sin_family = AF_INET;
                    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
                    serv_addr.sin_port = htons(portno);
                    
                    /* Now connect to the server */
                    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
                        perror("ERROR Connecting");
                        exit(1);
                    }

                    /* Now ask for a message from the user, this message
                    * will be read by server
                    */
	
                    //    printf("Please enter the message: ");
                    bzero(buffer,256);
                    //    fgets(buffer,255,stdin);

   
                    /* Send message to the server */
                    string msg = "GET " + path; 
                    // This is gonna be composed from path, HTTP/0.9 doesn't include a version number
                    n = write(sockfd, msg.c_str(), strlen(msg.c_str())); 
                    // There's gonna be problems having some things work on C aswell
                    if (n < 0) {
                        perror("ERROR Writing to socket");
                        exit(1);
                    }
   
                    /* Now read server response */
                    bzero(buffer,256);
                    n = read(sockfd, buffer, 255);
                    
                    if (n < 0) {
                        perror("ERROR Reading from socket");
                        exit(1);
                    }
                    
                    /* 
                        When we upgrade to HTTP/1.1
                        we need to change this to async recieve & send data,
                        + use a single connection for further communtication with this hostname:port combo 
                    */
                    // Return results to program
                    // printf("%s\n",buffer);

                    close(sockfd); // Close socket when finished.
                }
            } else {
                // return error
            }
        }
    }
}

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif
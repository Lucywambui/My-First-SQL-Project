/**
 * @file ftp_client_connection.cpp
 * @author Jasmine Banga/Lucy Mbugua, 578483786/655076669, NO2, CSCI 460, VIU
 * @version 1.0.0
 * @date October 16, 2021
 *
 * Describe the major functionalities that are performed by the code in this file.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "../include/ftp_client_connection.hpp"
#include"../include/ftp_client_session.hpp"
using namespace std;

char* getHostIPAddress(){

  //Determine the IP address of the local host and return it.
  struct hostent *host_entry;
  char host[MAX_IP_ADDRESS_LENGTH];
  char *IP;
  int hostname;
  
  hostname = gethostname(host,sizeof(host));
  
  if(hostname== -1){
    perror("gethostname");
    exit(1);
  }
  
  host_entry=gethostbyname(host);
  if(host_entry == NULL){
    perror("gethostbyname");
    exit(1);
  }
  
  IP=inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
  printf("Local Host Name: %s\n", host);
  printf("Local IP: %s \n", IP);
  
  return IP;
  
}


void connectToServer(int& socketDescriptor, bool& isConnected, const char* serverIP, int serverPort){

  // Sends a connection request on severPort of a sever whose IP address is equal to serverIP.
  struct sockaddr_in server;
  server.sin_family=AF_INET;
  server.sin_port = htons(serverPort);
  server.sin_addr.s_addr = inet_addr(serverIP);

  //create socket descriptor
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if(socketDescriptor < 0){
    cout << "ERROR: Could not create socket." << endl;
  }
  
  //all the socket stuff needs to be dealt with in here
  // If a connection has been established as a result of the request sets the connection descriptor value  // to reference 'socketDescriptor' and sets reference 'isConnected' to 'true'.
  
  if((connect(socketDescriptor, (struct sockaddr*) &server, (socklen_t)sizeof(server))) == -1){
    cout<<"Cannot connect to server will change tp default"<<endl;
    server.sin_port = FTP_CLIENT_DEFAULT_SERVER_PORT;
    server.sin_addr.s_addr=inet_addr(FTP_CLIENT_DEFAULT_SERVER_IP);
    cout<<"NEW SERVER IP: "<<FTP_CLIENT_DEFAULT_SERVER_IP<<endl;
    cout<< "NEW SERVER PORT: "<<FTP_CLIENT_DEFAULT_SERVER_PORT<<endl;
  }else{
    cout<<"IP ADDRESS OF SERVER: "<< serverIP <<endl;
    cout<<"PORT: "<<serverPort<<endl;
    cout<<"SOCKET: "<<socketDescriptor<<endl;
    isConnected=true;
    cout<<"successfully connected"<<endl;
  }
  return;
}

void disconnectFromServer(int& socketDescriptor, bool& isConnected){
  
  // Closes network connection represented by reference 'socketDescriptor' and
  close(socketDescriptor);

  // sets reference 'isConnected' to 'false'.
  isConnected=false;
  return;
  
}

int sendToServer(int sockDescriptor, const char* message, int messageLength){

  // Sends 'message' of length 'messageLength' bytes to the server 
  // on the network connection represented by 'sockDescriptor'.
  int mLength=0;
  mLength=send(sockDescriptor, message, messageLength,0);
  return mLength;
}

int receiveFromServer(int sockDescriptor, char* message, int messageLength){

  // Receives 'message' of length 'messageLength' bytes from the server 
  // on the network connection represented by 'sockDescriptor'.
  int mLength=0;
  mLength=recv(sockDescriptor, message, messageLength, 0);
  return mLength;
  
}

/**
 * @file ftp_client_session.cpp
 * @author Jasmine Banga/Lucy Mbugua, 578483786/655076669, NO2, CSCI 460, VIU
 * @version 1.0.0
 * @date October 6, 2021
 *
 * Describe the major functionalities that are performed by the code in this file.
 * This file connects and disconnects the FTP server.
 */



//Include required library and custom header files.
#include "../include/ftp_client_session.hpp"
#include "../include/ftp_client_connection.hpp"
#include "../include/ftp_client_command.hpp"
#include "../include/ftp_client_ui.hpp"
#include "../include/ftp_server_response.hpp"
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <iostream>
using namespace std;

//Implement all the functions prototyped in the header file ftp_client_session.hpp

void startClientFTPSession(const char* serverIP, int serverPort, ClientFtpSession& clientFtpSession){

  /*int clientSocket=-1;
  if((clientSocket=socket(AF_INET,SOCK_STREAM,0)) == -1){
    cout<<"Cannot connect to the server:"<<endl;
    exit(1);
    }

  //call connect
  connectToServer(clientSocket, clientFtpSession.isControlConnected, serverIP, serverPort);
  return;
  */

  connectToServer(clientFtpSession.controlConnection, clientFtpSession.isControlConnected, serverIP, serverPort);

  //after connection, the communication with server to get appropriate response
  char buffer[BUFFER_SIZE];
  int numBytes=receiveFromServer(clientFtpSession.controlConnection,buffer, BUFFER_SIZE); 
  cout<< "NUMBER RETURNED "<< numBytes<<endl;
}

void stopClientFTPSession(ClientFtpSession& clientFtpSession){
  /*
  //disconnect from server
  disconnectFromServer(clientFtpSession.controlConnection, clientFtpSession.isControlConnected);
  return;
  */

  disconnectFromServer(clientFtpSession.controlConnection, clientFtpSession.isControlConnected);
  
}


// not going to do this here, the function will be called fro conent

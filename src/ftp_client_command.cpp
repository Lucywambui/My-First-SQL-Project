/**
 * @file ftp_client_command.cpp
 * @author Jasmine Banga/Lucy Mbugua, 578483786/655076669, NO2, CSCI 460, VIU
 * @version 1.0.0
 * @date October 6th, 2021
 *
 * Describe the major functionalities that are performed by the code in this file
 * This file interprets and handles the commands given by the user. There are several functions in this
 * file that handles each command the user is able to handle. At this point (Version 1.0.0), the program is
 * able to handle the following user commands: help, user, pass, and quit. This file also handles simple
 * commnads from the user.
 */



//Include required library and custom header files.
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "../include/ftp_client_command.hpp"
#include "../include/ftp_client_ui.hpp"
#include "../include/ftp_client_connection.hpp"
#include "../include/ftp_client_session.hpp"
#include "../include/ftp_server_response.hpp"
using namespace std;


void interpretAndHandleUserCommand(string command, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
  
  // Interprets and handles user command
  // Calls appropriate handleCommandXXXX() function from 'ftp_client_command.hpp'.

  stringstream ss(command);
  string responseFromServer;
  int value=0;
  string commandValue[5];

  while(ss>>responseFromServer){
    commandValue[value]=responseFromServer;
    value++;
  }

  
  if (commandValue[0] == FTP_CLIENT_USER_COMMAND_HELP){
    handleCommandHelp();
    
  }else if (commandValue[0]== FTP_CLIENT_USER_COMMAND_USER){
    string username=commandValue[1];
    handleCommandUser(username, clientFtpSession, serverResponse);
    
  }else if (commandValue[0]== "pass"){
    string password=commandValue[1];  
    handleCommandPassword(password, clientFtpSession, serverResponse);
    
  }else if (commandValue[0] == "quit"){
    handleCommandQuit(clientFtpSession, serverResponse);

  }else if (commandValue[0] == "pwd"){
    handleCommandPrintDirectory(clientFtpSession, serverResponse);

  }else if (commandValue[0] == "cdup"){
    handleCommandChangeDirectoryUp(clientFtpSession, serverResponse);

  }else if (commandValue[0] == "cwd"){
    string path = commandValue[1];
    handleCommandChangeDirectory(path, clientFtpSession, serverResponse);
    
  }else{
    cout<<"Unknown command"<<endl;
  }
    
  // Displays all FTP server responses calling showFtpResponse() function (this is in ftp_client_ui.cpp)
  for(int i = 0; i < MAX_NUMBER_OF_RESPONSES; i++){
    showFtpResponse(serverResponse.responses[i]);
  }  
}

void handleCommandHelp(){
  // Displays following help text on the user terminal
  
  cout << "Usage: csci460Ftp>> [ help | user | pass | pwd | dir | cwd | cdup | get | quit ]" << endl;
  cout << "help                    Gives the list of FTP commands available and how to use them." << endl;
  cout << "user    <username>      Sumbits the <username> to FTP server for authentication." << endl;
  cout << "pass    <password>      Sumbits the <password> to FTP server for authentication." << endl;
  cout << "pwd                     Requests FTP server to print current directory." << endl;
  cout << "dir                     Requests FTP server to list the entries of the current directory." << endl;
  cout << "cwd     <dirname>       Requests FTP server to change current working directory." << endl;
  cout << "cdup                    Requests FTP server to change current directory to parent directory." << endl;
  cout << "get     <filename>      Requests FTP server to send the file with <filename>." << endl;
  cout << "quit                    Requests to end FTP session and quit." << endl;
  
// Usage: csci460Ftp>> [ help | user | pass | pwd | dir | cwd | cdup | get | quit ]
//          help                    Gives the list of FTP commands available and how to use them.
//          user    <username>      Sumbits the <username> to FTP server for authentication.
//          pass    <password>      Sumbits the <password> to FTP server for authentication.
//          pwd                     Requests FTP server to print current directory.
//          dir                     Requests FTP server to list the entries of the current directory.
//          cwd     <dirname>       Requests FTP server to change current working directory.
//          cdup                    Requests FTP server to change current directory to parent directory.
//          get     <filename>      Requests FTP server to send the file with <filename>.
//          quit                    Requests to end FTP session and quit.

  }

void handleSimpleCommand(string ftpCommand, bool checkAuthentication, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
  
  // Sends 'ftpCommand' request message to FTP server on the control connection.

  if(clientFtpSession.isControlConnected == true){
 
    sendToServer(clientFtpSession.controlConnection, ftpCommand.c_str(), ftpCommand.length());
    
    cout << "Sent command '" << ftpCommand << "' to the server" << endl;
    sleep(1);
    
    if(serverResponse.count == MAX_NUMBER_OF_RESPONSES){
      cout << "Responses can not be recieved at this time." << endl;
      return;
    }
    serverResponse.responses[serverResponse.count].clear();
    serverResponse.count = 0;
    
    char buffer[FTP_RESPONSE_MAX_LENGTH];
    
  // Receives the response from the server against the request.
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively.
  // If the response is unsuccessful and checkAuthentication parameter value is true, quits the application.
 
    
    int receivedResponseLength = receiveFromServer(clientFtpSession.controlConnection, buffer, FTP_RESPONSE_MAX_LENGTH);

    for(int i = 0; i < receivedResponseLength; i++){
      serverResponse.responses[serverResponse.count] += buffer[i];
    }
    
    serverResponse.count++;
    
  
    if(checkAuthentication){
      cout << "Right after check authenitcation = true"<< endl;
      cout<<"SERVER RESPONSE COUNT "<<serverResponse.count<<endl;
      if(strncmp(buffer, INVALID_USERNAME_RESPONSE, receivedResponseLength) == 0 || strncmp(buffer, NOT_LOGGED_IN_RESPONSE, receivedResponseLength) == 0){
	cout << "ERROR: Could not recieve message from the server" << endl;
	clientFtpSession.isControlConnected=false;
	//stopClientFTPSession(clientFtpSession);
      }
    }
  }
  return;
  
}

void handleCommandQuit(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){

  // Sends a 'QUIT' request message to FTP Server.
  const char* quit = "QUIT";
  
  // Calls function handleSimpleCommand() with "checkAuthentication" parameter "false" 
  // to send 'QUIT' request message.
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively.
  handleSimpleCommand(quit, false, clientFtpSession, serverResponse); 
  clientFtpSession.controlConnection = -1;
  stopClientFTPSession(clientFtpSession);

  // Quits from the application.
  return;
  //exit(1);
  
}

void handleCommandUser(string username, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){

  // Sends a 'USER <username>' request meesage to the FTP Server.
  username = "USER " + username;
  
  // Receives response from FTP Server.
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively. 
  // Calls function handleSimpleCommand() with "checkAuthentication" parameter value "true"
  handleSimpleCommand(username, true, clientFtpSession, serverResponse);

  return;
}

void handleCommandPassword(string password, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){

  // Sends a 'PASS <password>' request message to the FTP Server.
  password = "PASS " + password;  
  
  // Receives response from FTP Server.
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively. 
  // Calls function handleSimpleCommand() with "checkAuthentication" parameter "true"
  handleSimpleCommand(password, true, clientFtpSession, serverResponse);
  return;
}

void handleCommandDirectory(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
// Calls handlePassive() function to do the followings:
  //handlePassive(clienFtpSesssion, serverResponse);
// 	-send a 'PASV' request message to the FTP server.
// 	-receive the response against PASV request message from the server.
// 	-update 'serverResponses' and 'serverResponseCount' parameters based on PASV responses.
// 	-retrieve data-connection listener port number from the successful response.
// 	-requests a data connection to the server at the listener port at the server.
// If the data connection is successful, calls handleNLIST() function to do the followings:
// 	-send a 'NLST' request message to the server on the control connection.
// 	-receive the response against NLST request from the server on the control connection.
// 	-retrieve the list of entries of the current directory at the server on the data connection from the successful response..
// 	-update 'serverResponses' and 'serverResponseCount' parameters based on NLST responses.
}


void handleCommandPrintDirectory(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
// Sends a 'PWD' request message to FTP Server.
  const char* pwd = "PWD";
  
// Receives the response from the server.
// Successful server response contains the current FTP directory at the server.
  
// Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively.
// Calls function handleSimpleCommand() with "checkAuthentication" parameter "false"
// to perform the activities mentioned above.
  handleSimpleCommand(pwd, false, clientFtpSession, serverResponse);
  
}

void handleCommandChangeDirectory(string path, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){

  // Sends a 'CWD <path>' request message to the FTP Server.
  path = "CWD " + path;
  
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively.
  // Calls function handleSimpleCommand() with "checkAuthentication" parameter "false" 
  // to perform the activities mentioned above.

  handleSimpleCommand(path, false, clientFtpSession, serverResponse);
  return;
}

void handleCommandChangeDirectoryUp(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
  // Sends a 'CDUP' request message to FTP Server.
  const char* changedup = "CDUP";
  
  // Receives the response from the server.
  // Returns server responses and response count through 'serverResponses' and 'serverResponseCount' parameters respectively.
  
  // Calls function handleSimpleCommand() with "checkAuthentication" parameter "false" 
  // to perform the activities mentioned above.
  handleSimpleCommand(changedup, false, clientFtpSession, serverResponse);
}

void handleCommandGetFile(string filename, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
// Calls handlePassive() function to do the followings:
// 	-send a 'PASV' request message to the FTP server.
// 	-receive the response against PASV request message from the server.
// 	-update 'serverResponses' and 'serverResponseCount' parameters based on PASV responses.
// 	-retrieve data-connection listener port number from the successful response.
// 	-requests a data connection to the server at the listener port at the server.

  handlePassive(clientFtpSession, serverResponse);
// If the data connection is successful, calls handleRETR() function to do the followings:
  if(clientFtpSession.isDataConnected == true){
// 	-send a 'RETR <filename>' request message to the server on the control connection.
// 	-receive the response against RETR request from the server on the control connection.
// 	-retrieve the content of the file on the data connection from the successful response.
// 	-save the content of the file with <filename> at local folder.
// 	- NEED TO DO THIS update 'serverResponses' and 'serverResponseCount' parameters based on RETR responses.
    handleRETR(filename, clientFtpSession, serverResponse);
  }
}
void handlePassive(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse ){
// Sends a 'PASV' request message to the FTP server.
  string pasv= ("PASV");
  sendToServer(clientFtpSession.controlConnection, pasv.c_str(),pasv.length());
// Receives the response against PASV request message from the server.
  char buffer [FTP_RESPONSE_MAX_LENGTH];
  int receive= receiveFromServer(clientFtpSession.controlConnection,buffer, FTP_RESPONSE_MAX_LENGTH);
// Updates 'serverResponses' and 'serverResponseCount' parameters based on PASV responses.
  for(int i=0; i<receive; i++){
    serverResponse.responses[serverResponse.count]+=buffer[i];
  }
  
  serverResponse.count++;
  for(int i= 0; i<receive; i++){
  cout<<buffer[i];
  }
// If the response is a successful one, retrives data-connection listener port number form the response.
  char host[MAX_IP_ADDRESS_LENGTH];
  int port;
  if(strncmp(buffer, DATA_CONNECTION_SUCCESS_RESPONSE, receive) ==0){
// Retrives data-connection listener port number form the response
  getHostIPAndPortFromPassiveSuccessResponse(buffer,host, port);
// using function getHostIPAndPortFromPassiveSuccessResponse().
// Requests a data connection to the server on the listener port at the server.
  }
}

void handleNLIST(ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
// Sends a 'NLST' request message to the server on the control connection.
// Receives the response against NLST request from the server on the control connection.
// Updates 'serverResponses' and 'serverResponseCount' parameters based on NLST responses.
// If the response is successful, retrieves the list of entries in server's current directory 
// on the data connection.
// Adds the list of directory entries to 'serverResponses' and updates 'serverResponseCount'.
}

void handleRETR(string filename, ClientFtpSession& clientFtpSession, ServerResponse& serverResponse){
  // Sends a 'RETR <filename>' request message to the server on the control connection.
  filename = "RETR " + filename;
  
  // Receives the response against RETR request from the server on the control connection.
  // Updates 'serverResponses' and 'serverResponseCount' parameters based on RETR responses
  char* namefile = strdup(filename.c_str());
  
  sendToServer(clientFtpSession.controlConnection, namefile, filename.length());
  serverResponse.responses[serverResponse.count].clear();
  serverResponse.count = 0;

  int responseLength = receiveFromServer(clientFtpSession.controlConnection, namefile, filename.length());

  for(int i = 0; i < responseLength; i++){
    serverResponse.responses[serverResponse.count] += namefile[i];
    cout << serverResponse.responses[i] << endl;
  }
  serverResponse.count++;
  
  cout << "responselength: " << responseLength;
 
// If the response is successful, retrieves the content of the file on the data connection.
  
// Saves the file in the local directory at the client computer.
}

void getHostIPAndPortFromPassiveSuccessResponse(char* response, char* hostIP, int& hostPort){
// Retrieves IP address of FTP Server from the response string into reference 'hostIP'.
  int digit1,digit2,digit3,digit4, digit5,digit6;
  sscanf(response," 227 Entering Passive Mode (%d,%d,%d,%d,%d,%d ).", &digit1, &digit2, &digit3, &digit4,&digit5, &digit6);
 
  sprintf(hostIP, "This is the host IP %d.%d.%d.%d \n", digit1, digit2, digit3, digit4);
 
// Retrives data-connection listening port number of FTP server from the response string into reference 'hostPort'.
  hostPort=(digit5*256) + digit6;

// The example of a successful response message is "227 Entering Passive Mode (192,168,1,65,202,143)."
// From the above response message 'hostIP' will be 192.168.1.65 and 'hostPort' will be
// (202x256 + 143) or 51855.
}

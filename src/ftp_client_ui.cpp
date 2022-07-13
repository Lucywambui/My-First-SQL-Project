/**
 *  @file ftp_client_ui.hpp
 * @author Jasmine Banga/Lucy Mbugua, 578483786/655076669, NO2, CSCI 460, VIU
 * @version 1.0.0
 * @date October 6, 2021
 *
 * Describe the major functionalities that are performed by the code in this file.
 * This file prompts the user to enter a command and saves the command the user enters 
 * into a variable that is passed in from the main that is called userCommand. 
 * It the displays the server response to the user's commands.
 */



//Include required library and custom header files.
#include "../include/ftp_client_ui.hpp"
#include <iostream>
#include <string>
using namespace std;


void getUserCommand(std::string& userCommand){

  // Lets user to type command and reads the user command from the prompt.
  cout << "CSCI460FTP ";
  getline(cin, userCommand);
  cout << endl;
}


void showFtpResponse(std::string response){ 

  cout << response << endl;

}

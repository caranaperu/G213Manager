/**
  *  The MIT License (MIT)
  *
  *  g231Manager v0.1 Copyright (c) 2017 Carlos Arana
  *  This software is a migration to c++ from G213Colors all the rights to SebiTimeWaster
  *  for his investigation on the commands.
  *
  *  Permission is hereby granted, free of charge, to any person obtaining a copy
  *  of this software and associated documentation files (the "Software"), to deal
  *  in the Software without restriction, including without limitation the rights
  *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  *  copies of the Software, and to permit persons to whom the Software is
  *  furnished to do so, subject to the following conditions:
  *
  *  The above copyright notice and this permission notice shall be included in all
  *  copies or substantial portions of the Software.
  *
  *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  *  SOFTWARE.
**/

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "G213KeybLedManager.h"

#define CMD_SET_COLOR 0x01
#define CMD_SET_BREATHE  0x02
#define CMD_SET_CYCLE  0x03

int main(int argc,char *argv[]) {

    G213KeybLedManager *g213Mngr = new G213KeybLedManager();

    u_short command = 0x00;
    u_short field = 0;

    char * color=NULL;
    long timeInMills=0;
    int exitStatus = EXIT_SUCCESS;
    bool showInfo = false;


    if (argc == 3) {
        if (std::string(argv[1]) == "-c1" || std::string(argv[1]) == "-c2" ||
                std::string(argv[1]) == "-c3" || std::string(argv[1]) == "-c4"  || std::string(argv[1]) == "-c5" ||
                std::string(argv[1]) == "-c") {
            command = CMD_SET_COLOR;
            field = std::string(argv[1]) == "-c" ? 0 : argv[1][2]-'0';
            color = argv[2];

            if (argv[2] == NULL || strlen(argv[2]) != 6) {
                std::cout<<"Color is in hex format rrggbb"<<std::endl;
		exit(EXIT_FAILURE);
            }

        } else if(std::string(argv[1]) == "-x") {
            command = CMD_SET_CYCLE;
            timeInMills = atol(argv[2]);

	    if (timeInMills < 32 || timeInMills > 65535) {	      
		std::cout<<"Time is in milliseconds, range: 32 - 65535"<<std::endl;
		exit(EXIT_FAILURE);
	    }

        } else {
            showInfo = true;
        }
    } else if (argc == 4) {
        if (std::string(argv[1])  == "-b") {

            command = CMD_SET_BREATHE;
            color = argv[2];
            timeInMills = atol(argv[3]);
	    
	    if (strlen(argv[2]) != 6) {
                std::cout<<"Color is in hex format rrggbb"<<std::endl;
		exit(EXIT_FAILURE);
            }
            
	    if (timeInMills < 32 || timeInMills > 65535) {	      
		std::cout<<"Time is in milliseconds, range: 32 - 65535"<<std::endl;
		exit(EXIT_FAILURE);
	    }
        }
    } else {
        showInfo = true;
    }

    if (showInfo == true) {
        std::cout<<std::endl<<"Options :"<<std::endl;
        std::cout<<"-c <color>\t\tSet selected color for 5 areas"<<std::endl;
        std::cout<<"-c<n> <color>\t\tSet selected color for n area can be any number between 1 and 5 (i.e -c1)"<<std::endl;
        std::cout<<"-x <speed>\t\tSet cycle animation with a speed indicated in milliseconds"<<std::endl;
        std::cout<<"-b <color> <speed>\tSet breathe animation with the specified color and the speed in milliseconds"<<std::endl;
        std::cout<<std::endl;
        std::cout<<"* Color is a hex encoded color in the format RRGGBB"<<std::endl;
        std::cout<<"  i.e. ff0000 is red, 00ff00 is green and so on"<<std::endl;
        std::cout<<"* Speed is in milliseconds, range: 32 - 65535"<<std::endl;
        exit(EXIT_FAILURE);
    }

    if (g213Mngr->attach() == true) {
        if (command == CMD_SET_COLOR) {
            g213Mngr->g213SetColor(color,field);
        } else if (command == CMD_SET_BREATHE) {
            g213Mngr->g213SetBreathe(color,timeInMills);
        } else if (command == CMD_SET_CYCLE) {
            g213Mngr->g213SetCycle(timeInMills);
        } else {
            exitStatus = EXIT_FAILURE;
        }
    }

    g213Mngr->detach();
    exit(exitStatus);
}


/**
 * I will choose to switch doors every time. Because after a lot of simulations, the percentage of winning
 * by switching doors is around 66.67% while the percentage of winning by not switching doors is around 33.33%.
 */
/**
@file
@author Justin Nguyen <jn867@drexel.edu>
@date September 28, 2025
@section DESCRIPTION
This file contains a program that examine the percentage of winning the "Let's Make a Deal" game show.
It simulates the game multiple times and calculates the winning percentage based on two strategies: switching doors and not switching doors.

*/
#include <iostream>
#include <random>
#include <string>

/**
@mainpage CS 361 - Homework 1
@section Description
The program simulates the "Let's Make a Deal" game show, specifically the Monty Hall problem. It runs multiple simulations to determine the winning percentages for two strategies: switching doors and not switching doors. The results are displayed as percentages of wins for each strategy.
*/

/**
    Main function runs the Monty Hall simulation.
    @param argc Argument count
    @param argv Argument vector
    @return 0 for sucess and failure
*/
int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cout << "Monty Hall Problem Simulator"<<std::endl<<"Usage: ./monty <number_of_simulations>" << std::endl;
        return 0;
    }
    for (char* c = argv[1]; *c != '\0'; c++) {
        if (!isdigit(*c)) {
        std::cout << "Monty Hall Problem Simulator"<<std::endl<<"Number of test is not a number" << std::endl;
        return 0;
        }
    }
    float num_simulations = std::stoi(argv[1]);
    float switch_wins = 0;
    float stay_wins = 0;
    for ( int i = 0; i < num_simulations;i++){
        int car = std::rand() % 3; // Randomly place the car behind one of the three doors
        int player_choice = std::rand() % 3; // Player makes an initial choice
        int monty_choice; //not the car or the player's choice 
        for ( int j = 0; j < 3; j++){
            if ( j != car && j != player_choice){
                monty_choice = j;
            }
        }
        //find the switch choice
        float switch_choice = 0;
        for ( int j = 0; j < 3; j++){
            if ( j != monty_choice && j != player_choice){
                switch_choice = j;
            }
        }
        
        if (switch_choice == car){
            switch_wins++;
        }
        if ( player_choice == car){
            stay_wins++;
        }
        
        

    }
    std::cout<<"Monty Hall Problem Simulator"<< std::endl;
    double percentage = (switch_wins * 100) / num_simulations;
    double stay_percentage = 100 - percentage;
    std::printf("Switch would win %.2f percent of experiments", percentage);
    std::cout<< std::endl;
    std::printf("Switch would win %.2f percent of experiments", stay_percentage);
    std::cout<< std::endl;
    
    return 0;
}
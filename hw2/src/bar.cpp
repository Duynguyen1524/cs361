/**
    @file
    @author Justin Nguyen <jn867@drexel.edu>
    @date 2025-10-08
    @section Description

    This file gives the implementation details of homework 2. It simulates how a noodle bar works. 
    Customers will attempt to enter the noodle bar, eat, and then leave.
 */
/**
@mainpage CS 361 - Homework 2
@section Description
This project simulates a noodle bar with a limited number of seats using semaphores to manage customer access.
Customers (threads) will attempt to enter the noodle bar, eat, and then leave.
*/
#include "semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <string>

/**
function to simulate action of a customer when enter a noodle bar. Customers will try to get a seat, eat, and then leave. After leaving, they will signal the semaphore to indicate that a seat is now available.
@param id Customer ID
@param sem Semaphore to manage access to the noodle bar
@return void
*/

void customer(int id, Semaphore& sem);
/**
function to safely print the status of a customer.
@param str Status message
@param id Customer ID
@return void
*/
void printStatus(std::string& str, int id);


/**
Main function to simulate the noodle bar scenario.
@param argc Argument count
@param argv Argument vector (expects number of customers as the first argument)
@return Exit status
*/
int main(int argc, char* argv[]){
    int numCustomers; // Default number of customers
    if (argc > 1) {
        numCustomers = std::atoi(argv[1]);
    } 
    const int seatCount = 5; // Number of seats in the noodle bar
    Semaphore sem = Semaphore(seatCount); // Initialize semaphore with number of seats
    std::vector<std::thread> customers; // Vector to hold customer threads
    for(int i = 0; i < numCustomers; ++i){
        customers.push_back(std::thread(customer,i+1, std::ref(sem))); // Create customer threads
    }
    for(auto& t : customers){
        t.join(); // Wait for all customers to finish
    }
    return 0;

}


void printStatus(std::string& str, int id){
    static std::mutex ioLock;
    std::lock_guard<std::mutex> guard(ioLock);
    std::cout << "Person " << id << str << std::endl;
}

void customer(int id, Semaphore& sem){
    std::string str = " woke up from their nap.";
    printStatus(str, id);
    sem.wait() ;// Try to get a seat
    str = " got to the noodle bar.";
    printStatus(str, id);
    std::this_thread::sleep_for(std::chrono::seconds(rand()%3)); // Simulate walking time
    str = " started eating.";
    printStatus(str, id);
    std::this_thread::sleep_for(std::chrono::seconds(rand()%3)); // Simulate eating time
    str = " finished eating and is heading home.";
    printStatus(str, id);
    sem.signal(); // Leave the seat
    
}
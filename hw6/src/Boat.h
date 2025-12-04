#ifndef BOAT_H
#define BOAT_H

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <chrono>
#include "semaphore.h"

class Human;   
/**
 * @class Boat
 * A class representing the shared boat used in the Island Crossing simulation.
 *
 * The Boat class stores global shared state used by all threads in the program. The boat
 * enforces the rule that one driver and up to one passenger can be in the boat
 * at a time. 
 *
 * The monitor and human threads interact with this structure to safely update
 * boat occupancy, switch seats, clear the boat, and track completed trip counts.
 */

class Boat {
public:
    // Constructor
    Boat(int numAdults, int numChildren);
    int ferry;// Keep track of the ferry boat(not used)
    int adults_island_;/**< The number of adult in island.*/
    int children_island_;/**< The number of children in island.*/
    /**< print summary.*/
    void print_summary();
    
    /**< Human pointer keep track of driver*/
    Human* mydriver = nullptr;
    /**< Human pointer keep track of passenger*/
    Human* mypassenger = nullptr;
    /**< function to add a human to driver seat*/
    void addriver(Human* driver) {
        mydriver = driver;
    }
 /**< function to add a human to passenger seat*/
    void addpassenger(Human* passenger) {
        mypassenger = passenger;
    }
     /**< function to  change  seat*/
    void changeseat() {
        Human* p = mydriver;
        mydriver = mypassenger;
        mypassenger = p;
    }
     /**< function to go to mainland*/
    void exit() {
        mydriver = nullptr;
        mypassenger = nullptr;
    }
    

    int travel_to_mainland_count_;  /**< # of trips from island to mainland. */
    int travel_to_island_count_;    /**< # of return trips to the island.     */
    int boats_two_children_;        /**< # of two-child crossings.            */
    int boats_child_adult_;         /**< # of adult + child crossings.        */
    int boats_one_person_;          /**< # of solo crossings (return trips).  */
    int times_adult_driver_;        /**< # of times an adult drove.           */
    int times_child_driver_;        /**< # of times a child drove.            */

    int peopleleft;/**< Total people remaining on the island (set in constructor). */
};

#endif

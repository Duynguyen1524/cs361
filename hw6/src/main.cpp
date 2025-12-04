/**
@mainpage CS 361 - Homework 6
@section Description
This program simulates the classic "Island Boat" concurrency problem using
multiple threads, semaphores, and shared synchronization mechanisms.
Adults and children must safely cross from the island to the mainland 
following specific constraints on boat occupancy and driver/passenger roles.
The simulation uses a dedicated ferry child, driver/passenger coordination 
semaphores, and a monitor thread to orchestrate crossings. The design ensures 
that no race conditions occur, prevents deadlocks, and guarantees that all 
individuals eventually reach the mainland.
*/
/**
 * @file main.cpp
 * @author 
 *   Justin Nguyen <jn867@drexel.edu>
 * @date November 17, 2025
 *
 * @section DESCRIPTION
 * This file implements the main control logic for the CS 361 Homework 6
 * Island Boat simulation. It initializes all semaphores, threads, and shared
 * data structures needed to coordinate boat crossings among adults, children,
 * and a designated ferry child.
 *
 * The program ensures proper synchronization using semaphores and mutexes to
 * prevent race conditions and deadlocks. It enforces the problem constraints
 * by controlling driver/passenger assignments, managing return trips, and
 * guaranteeing that all individuals eventually reach the mainland.
 */
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>

#include "Human.h"
#include "Boat.h"

std::mutex print_lock;// for printing purpose. 
/**
 * Thread function for the special ferry child.
 *
 * This thread represents the designated child responsible for sitting in the
 * passenger seat when a person drives the boat to the mainland, and then
 * returning the boat to the island as the driver. It coordinates with the
 * driver and the monitor thread using semaphores to ensure correct
 * sequencing of boarding, traveling, and exiting.
 *
 * @param boat Pointer to the shared Boat object.
 * @param id Unique identifier of the child thread (always child #1).
 * @param cv_board Semaphore that signals the boat is ready for the next cycle.
 * @param travel_sem Semaphore used to synchronize travel completion.
 * @param exit_sem Semaphore used to signal person exit.
 * @param childferry_sem Semaphore that wakes the ferry child for boarding.
 * @param tripdone Semaphore that notifies the monitor a full cycle is complete.
 * @param driver_sem Semaphore signaling that the driver is seated and ready.
 * @param passenger_sem Semaphore signaling that the passenger is seated.
*/

void childferryThread1(Boat* boat, int id,
                       Semaphore& cv_board,
                       Semaphore& travel_sem,
                       Semaphore& exit_sem,
                       Semaphore& childferry_sem,
                       Semaphore& tripdone,
                       Semaphore& driver_sem,
                       Semaphore& passenger_sem);

/**
 * This thread represents one adult who waits to be signaled by the monitor
 * thread before attempting to board the boat. When selected, the adult becomes
 * the driver if the driver seat is available. The thread synchronizes with the
 * ferry child (passenger) using driver and passenger semaphores to ensure that
 * both seats are properly filled before travel. After reaching the mainland,
 * the adult exits the boat, signals completion, and the thread terminates.
 *
 * @param boat Pointer to the shared Boat object containing system state.
 * @param id Unique identifier for this adult thread.
 * @param cv_board Semaphore signaled when the boat is ready for the next cycle.
 * @param travel_sem Semaphore used to synchronize travel completion.
 * @param exit_sem Semaphore signaling that a passenger or driver has exited.
 * @param mtx Mutex protecting boat access during boarding operations.
 * @param adult_sem Semaphore that wakes an adult when the monitor selects one.
 * @param driver_sem Semaphore signaling when the driver is seated.
 * @param passenger_sem Semaphore signaling when the passenger is seated.
 */
void adultThread(Boat* boat, int id,
                 Semaphore& cv_board,
                 Semaphore& travel_sem,
                 Semaphore& exit_sem,
                 std::mutex& mtx,
                 Semaphore& adult_sem,
                 Semaphore& driver_sem,
                 Semaphore& passenger_sem);
/**
 *
 * This thread represents one of the children who are not the designated ferry
 * child. When signaled by the monitor using  child_sem, a child attempts to
 * board the boat and becomes the driver if the driver seat is available.
 * The child synchronizes with the passenger using driver and passenger semaphores to ensure both
 * seats are properly occupied before the boat travels. After reaching the
 * mainland, the child exits, signals completion to the monitor, and ends.
 *
 * @param boat Pointer to the shared Boat object.
 * @param id Unique identifier for this child thread.
 * @param cv_board Semaphore used to signal that the boat may begin a new cycle.
 * @param travel_sem Semaphore signaling that the travel phase is complete.
 * @param exit_sem Semaphore used when a rider exits the boat.
 * @param mtx Mutex protecting access to shared boat state during boarding.
 * @param child_sem Semaphore that wakes a child when the monitor selects one.
 * @param driver_sem Semaphore signaling that the driver is ready.
 * @param passenger_sem Semaphore signaling that the passenger is ready.
  */

void childThread(Boat* boat, int id,
                 Semaphore& cv_board,
                 Semaphore& travel_sem,
                 Semaphore& exit_sem,
                 std::mutex& mtx,
                 Semaphore& child_sem,
                 Semaphore& driver_sem,
                 Semaphore& passenger_sem);
/**
 * Monitor thread responsible for orchestrating all boat crossings.
 *
 * This thread coordinates the sequence in which adults and children board and
 * operate the boat. In the first phase, the monitor ensures that each adult is
 * paired with the special ferry child, waking the appropriate semaphores to
 * form a valid adult–child crossing. In the second phase, once all adults have
 * crossed, the monitor signals the remaining children to operate the boat
 * with the ferry child. The monitor waits on  tripdone to ensure each full trip cycle
 * completes before scheduling the next crossing, and uses  cv_board to reset
 * the boat between cycles.
 *
 * @param boat Pointer to the shared Boat object holding global state.
 * @param cv_board Semaphore signaling that the boat is ready for the next trip.
 * @param adult_sem Semaphore used to wake an adult for boarding.
 * @param child_sem Semaphore used to wake a normal child for boarding.
 * @param tripdone Semaphore signaled when a round-trip cycle is complete.
 * @param ferry_sem Semaphore used to wake the special ferry child passenger.
 
 */
void start(Boat* boat,
           Semaphore& cv_board,
           Semaphore& adult_sem,
           Semaphore& child_sem,
           Semaphore& tripdone,
           Semaphore& ferry_sem);


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./island <num_adults> <num_children>\n";
        return 1;
    }

    int num_adults   = std::stoi(argv[1]);
    int num_children = std::stoi(argv[2]);

    // ---------------- Semaphores ----------------
    Semaphore cv_board(0);
    Semaphore travel_sem(0);
    Semaphore exit_sem(0);
    Semaphore child_sem(0);
    Semaphore adult_sem(0);
    Semaphore tripdone(0);
    Semaphore ferry_sem(0);
    Semaphore driver_sem(0);
    Semaphore passenger_sem(0);

    std::mutex mtx;

    // ---------------- Boat ----------------
    Boat boat(num_adults, num_children);

    // ---------------- Threads ----------------
    std::vector<std::thread> threads;

    // Monitor thread
    std::thread boat_monitor(
        start,
        &boat,
        std::ref(cv_board),
        std::ref(adult_sem),
        std::ref(child_sem),
        std::ref(tripdone),
        std::ref(ferry_sem)
    );

    // Special ferry child thread
    std::thread childferry(
        childferryThread1,
        &boat,
        1,
        std::ref(cv_board),
        std::ref(travel_sem),
        std::ref(exit_sem),
        std::ref(ferry_sem),
        std::ref(tripdone),
        std::ref(driver_sem),
        std::ref(passenger_sem)
    );

    // Adult threads
    for (int i = 0; i < num_adults; i++) {
        threads.emplace_back(
            adultThread,
            &boat,
            i + 1,
            std::ref(cv_board),
            std::ref(travel_sem),
            std::ref(exit_sem),
            std::ref(mtx),
            std::ref(adult_sem),
            std::ref(driver_sem),
            std::ref(passenger_sem)
        );
    }

    // Child threads 
    for (int i = 1; i < num_children; i++) {
        threads.emplace_back(
            childThread,
            &boat,
            i + 1,
            std::ref(cv_board),
            std::ref(travel_sem),
            std::ref(exit_sem),
            std::ref(mtx),
            std::ref(child_sem),
            std::ref(driver_sem),
            std::ref(passenger_sem)
        );
    }

    // Join all human threads
    for (auto& t : threads)
        t.join();

    childferry.join();
    boat_monitor.join();

    boat.print_summary();

    return 0;
}



void start(Boat* boat,
           Semaphore& cv_board,
           Semaphore& adult_sem,
           Semaphore& child_sem,
           Semaphore& tripdone,
           Semaphore& ferry_sem)
{
    // Adult phase
    while (boat->adults_island_ > 0) {

        if (boat->mydriver == nullptr && boat->mypassenger == nullptr) {
            adult_sem.signal();
            ferry_sem.signal();
        } else {
            adult_sem.signal();
        }

        if (boat->peopleleft == 2) break;

        tripdone.wait();
        cv_board.signal();
    }

    // Child-only phase
    while (boat->children_island_ > 1) {

        child_sem.signal();

        if (boat->peopleleft == 2) break;

        tripdone.wait();
        cv_board.signal();
    }
}


void adultThread(Boat* boat, int id,
                 Semaphore& cv_board,
                 Semaphore& travel_sem,
                 Semaphore& exit_sem,
                 std::mutex& mtx,
                 Semaphore& adult_sem,
                 Semaphore& driver_sem,
                 Semaphore& passenger_sem)
{
    Human adult(id, 0);

    while (true) {

        adult_sem.wait();

        std::lock_guard<std::mutex> lock(mtx);

        if (boat->adults_island_ <= 0)
            return;

        adult.addBoat(boat);

        if (boat->mydriver == nullptr) {

            boat->addriver(&adult);
            boat->times_adult_driver_++;

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cout << "Adult " << adult.get_id()
                          << " got into driver's seat of the boat.\n";
            }

            driver_sem.signal();
            passenger_sem.wait();

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cerr << "Boat is traveling island to mainland\n";
            }

            int t = (std::rand() % 4) + 1;
            std::this_thread::sleep_for(std::chrono::seconds(t));

            boat->travel_to_mainland_count_++;
            boat->adults_island_--;
            boat->peopleleft--;
            boat->boats_child_adult_++;

            boat->exit();

            travel_sem.signal();

            

            exit_sem.signal();
            return;
        }
    }
}


void childferryThread1(Boat* boat, int id,
                       Semaphore& cv_board,
                       Semaphore& travel_sem,
                       Semaphore& exit_sem,
                       Semaphore& childferry_sem,
                       Semaphore& tripdone,
                       Semaphore& driver_sem,
                       Semaphore& passenger_sem)
{
    Human child(id, 1);

    while (true) {

        childferry_sem.wait();

        if (boat->children_island_ <= 0)
            return;

        child.addBoat(boat);
        boat->addpassenger(&child);

        while (boat->peopleleft > 0 && boat->mypassenger != nullptr) {

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cout << "Child " << child.get_id()
                          << " got into  passenger's seat of the boat.\n";
            }

            driver_sem.wait();
            passenger_sem.signal();

            travel_sem.wait();
            exit_sem.wait();

            if (boat->peopleleft == 1) {
                
                boat->peopleleft--;
                return;
            }

            boat->changeseat();

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cerr << "Boat is traveling mainland to island\n";
            }

            boat->boats_one_person_++;
            boat->times_child_driver_++;

            int t = (std::rand() % 4) + 1;
            std::this_thread::sleep_for(std::chrono::seconds(t));

            boat->travel_to_island_count_++;
            boat->addpassenger(&child);

            tripdone.signal();
            cv_board.wait();
        }
    }
}


void childThread(Boat* boat, int id,
                 Semaphore& cv_board,
                 Semaphore& travel_sem,
                 Semaphore& exit_sem,
                 std::mutex& mtx,
                 Semaphore& child_sem,
                 Semaphore& driver_sem,
                 Semaphore& passenger_sem)
{
    Human child(id, 1);

    while (true) {

        child_sem.wait();

        std::lock_guard<std::mutex> lock(mtx);

        if (boat->children_island_ <= 0)
            return;

        child.addBoat(boat);

        if (boat->mydriver == nullptr) {

            boat->addriver(&child);
            boat->boats_two_children_++;
            boat->times_child_driver_++;

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cout << "Child "
                          << child.get_id()
                          << " got into driver's seat of the boat.\n";
            }

            driver_sem.signal();
            passenger_sem.wait();

            {
                std::lock_guard<std::mutex> pl(print_lock);
                std::cerr << "Boat traveling island to mainland\n";
            }

            int t = (std::rand() % 4) + 1;
            std::this_thread::sleep_for(std::chrono::seconds(t));

            boat->travel_to_mainland_count_++;
            boat->children_island_--;
            boat->peopleleft--;

            boat->exit();

            

            travel_sem.signal();
            exit_sem.signal();
            return;
        }
    }
}

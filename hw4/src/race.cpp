/**
@mainpage CS 361 - Homework 4
@section Description
This program simulates a thread race where multiple player threads compete to reach a score of 20 by taking moves provided by a game master thread. The game master generates random moves and signals players when moves are available. Players take turns moving based on the available moves in a shared queue, and the order in which they finish is recorded and displayed at the end of the race
*/
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <random>
#include <queue>
#include <string>
#include "semaphore.h"
/**
 @file
 @author Justin Nguyen <jn867@drexel.edu>
 @date 10-24-2025
 @section Description
 This file contains the implementation of a multithreaded race simulation. 
 */

/**
Player thread function simulates a player in the race where each player moves based on available moves provided by the game master by taking moves from a shared queue.
 @param id is the player id
 @param results is the vector to store finishing order
 @param sem is the semaphore to signal available moves
 @param q is the queue of available moves
 @param finished_count is the count of finished players
 @param print_lk is the mutex to protect printing
 @param q_lk is the mutex to protect the queue
  @param finish_lock is the mutex to protect the count of finish players. 
 */
void player_thread(int id, std::vector<std::thread::id> &results, Semaphore &sem, std::queue<int> &q, int &finished_count, std::mutex &print_lk, std::mutex &q_lk, std::mutex &finish_lock) ;
/**
 Game Master function generates moves for players and adds them to a shared queue, signaling players when new moves are available.
 @param sem is the semaphore to signal available moves
 @param q is the queue of available moves
 @param num_threads is the total number of player threads
 @param finished_count is the count of finished players
 @param q_lock is the mutex to protect the queue
 @param finish_lock is the mutex to protect the count of finish players. 
 */

void Game_Master(Semaphore &sem, std::queue<int> &q, int num_threads, int &finished_count, std::mutex &q_lock, std::mutex &finish_lock);


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_threads>\n";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    if (num_threads <= 0) {
        std::cerr << "Number of threads must be positive.\n";
        return 1;
    }
    int finished_count = 0;
    std::mutex finished_lock;
    Semaphore sem(0);
    std::vector<std::thread::id> results;
    std::queue<int> q;
    std::mutex q_lock;
    std::mutex print_lock;
    std::thread gm(Game_Master, std::ref(sem), std::ref(q), num_threads, std::ref(finished_count), std::ref(q_lock),std::ref(finished_lock));
    std::vector<std::thread> players;
    
    for (int i = 0; i < num_threads; ++i) {
        players.emplace_back(player_thread, i, std::ref(results), std::ref(sem), std::ref(q), std::ref(finished_count), std::ref(print_lock),std::ref(q_lock),std::ref(finished_lock));
    }

    for (auto &t : players) t.join();
    gm.join();
    for (int i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << ":   " << results[i] << "\n";
    }
}



void Game_Master(Semaphore &sem, std::queue<int> &q, int num_threads, int &finished_count, std::mutex &q_lock, std::mutex &finish_lock) {
    while ( true) {
        {
            
            std::lock_guard<std::mutex> guard(finish_lock);  
            if (finished_count >= num_threads) break;
            

        }
        int k = rand() % 6;
        if (k == 0){
            std::this_thread::sleep_for(std::chrono::seconds(rand() % 3));
            continue;
        }
        {
        std::lock_guard<std::mutex> guard(q_lock);
        for (int i = 0; i < k; i++) {
            q.push(rand() % 6 + 1);  // push random moves 1–6
            // Notify one player that a move is available
            sem.signal();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3));
    }
}

void player_thread(int id, std::vector<std::thread::id> &results, Semaphore &sem, std::queue<int> &q, int &finished_count, std::mutex &print_lk, std::mutex &q_lk, std::mutex &finish_lock) {
    int score = 0;
    std::thread::id current_id = std::this_thread::get_id();
    {std::lock_guard<std::mutex> guard(print_lk);
        std::cout << "Thread " << current_id << " has left the gates\n";
    }

    while (score < 20) {
        sem.wait();

        int move = 0;
        {
            std::lock_guard<std::mutex> guard(q_lk);
            if (!q.empty()) {
                move = q.front();
                q.pop();
            } else {
                continue;
            }
        }
        score += move;
        if (score >= 20){
            {
            std::lock_guard<std::mutex> guard(print_lk);
            std::cout << "Thread " << current_id << " moved forward " << move  << " spaces\n";
            std::cout << "Thread " << current_id << " has crossed the finish line!\n";
            }
            {
            std::lock_guard<std::mutex> guard(finish_lock); 
            finished_count++;
            results.push_back(current_id);
            }
            return;
        }
        else{
        {std::lock_guard<std::mutex> guard(print_lk);
        std::cout << "Thread " << current_id << " moved forward " << move<< " spaces\n";
        }
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 3));
        
    }
    
}

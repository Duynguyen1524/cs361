# Thread Race 2000  
**CS 361 – Homework 04**  
**Author:** Justin Nguyen  
**Drexel ID:** jn867@drexel.edu_

---

## How to Run

### 1. Build the Program
make
### 2. Run
make test1: run with 3 threads
make test2: run with 5 threads
make test3: run with 10 threads
or manually:
./bin/race n where n is number of threads you want to compete in the race. 
### 3. Clean Executables
make clean
### 4. Short question
Question 1:
I use semaphore to control communication between threads and game master. I use signal to announce to all the player whenever dice are added.
Each racer calls wait() before trying to pop from the queue. This prevents racers from trying to read when the queue is empty.

Question 2
Three mutex locks are used:
    One to protect shared access to the dice queue, because both Game Master and racers modify it.
    One to protect the players vector where finishing order is recorded.
    One to protect the number of players that finished the race. 
Using locks is to prevent threads from running in the wrong logic and keep critical section safe. 


Question 3:
I am confident because locks are acquired in consistence orders and semphore are always go outside lock guard scope. Furthermore, all lock are managed with lock guard. The Master also exits once all players is finished so no thread run indefinitely. 
Question 4
The Game Master continuously produces dice rolls until every racer is finished. The racers wait fairly on the same semaphore and eventually receives dice to move to the finish line. Moreover, no players is blocked infinitely. 

Question 5
The most challenging part of this assigment is how to protect data and print out the statement safely. I ran into a lot of problems while try to figure out a way to protect data and run the logic consistently. 
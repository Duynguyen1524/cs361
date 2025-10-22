#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>


void message(const std::string& threadName, const std::string& status, int task){
    std::cout<<"Thread"<<threadName<<": "<<status<<"Task"<<<<std::endl;
}
void doWork(){
    int k = rand()%3+1;
    auto t = std::chrono::seconds(k);
    std::this_thread::sleep_for(t);

};
void threadA(){
    message("A", "Starting", 1);
    doWork();
    message("A", "Finishing", 1);
    lockA.unlock();
    lockB.lock();
    message("A", "starting", 2);
    doWork();
    message("A", "Finishing", 2);
}
void threadB(){
    message("B", "Starting", 1);
    doWork();
    message("B", "Finishing", 2);
}
int main(void){
    srand(time(NULL));
    lockA.lock();
    lockB.lock();
    std::thread A = std::thread(threadA);
    std::thread B = std::thread(threadB);
    A.join();
    B.join();
    return 0;
}
#include "semaphore.h"

semaphore::semaphore() {
    counter = 0;
}
semaphore::Semaphore(int c) {
    counter = c;
}
void semaphore::signal(){
    std::lock_guard<std::mutex> lk(myLock);
    counter++;
    cv.notify_one();
}
void semaphore::wait(){
    std::unique_lock<std::mutex> lk(myLock);
    //await (counter > 0)
    cv.wait(lk, [this] { return counter > 0; });
    counter--;
}
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <mutex>

std::mutex mtx;

std::ostream& operator<< (std::ostream& os, const std::vector<int>& vec) {
    os<<"[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i != vec.size() - 1) {
            os << ", ";
        }
    }
    os<<"]";
    return os;
}
void sleepyThread(int x, int& pos, std::vector<int>& vec) {
    auto t = std::chrono::seconds(x);
    std::this_thread::sleep_for(t);
    mtx.lock();
    std::cout
    <<"thread "<<std::this_thread::get_id()
    <<x<<" ";
    mtx.unlock();
}
void sleepsort(std::vector<int>& vec) {
    auto t = std::vector<std::thread>();
    for (auto& x : vec){
        t.push_back(std::thread(sleepyThread,x)); 

    }
    for (auto& th : t){
        th.join();
    }
        
}

int main() {
    std::vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::cout << "Original vector: " << vec << std::endl;
    sleepsort(vec);
    std::cout << "\nSorted vector: " << vec << std::endl;
    return 0;
}
#include <iostream>
#include <thread>
#include <template>
#include <vector>
#include <queue>
class Node{
    public:
    int val;
    std::shared_ptr<Node> next;
    Node(int v){
        val = v;
        next = nullptr;
    }
    template <typename T>
    class queue{
        private:
        std::shared_ptr<Node<T>> head;
        std::shared_ptr<Node<T>> tail;
        public:
        queue(){
            head = nullptr;
            tail = nullptr;
        }
        bool isEmpty(){
            return head == nullptr;
        }
        T front(){
            if isEmpty(){
                throw std::runtime_error("Queue is empty");
            }
            return head.val;
    
        }

        
};

int main (void){
    Node n(7);
    std::cout << n.val << std::endl;
    std::cout << n.next << std::endl;
    queue q;
    for (int i = 0; i < 10 ; i++){
        std::cout < q.front()<<std::endl;
        q.queue();


    }

    
    return 0;
}

#include <atomic>
#include <memory>


template <typename T>
class LockFreeQueue {
private:
    struct Node {
        T data;
        std::atomic<Node*> next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue() : head(new Node({})), tail(head.load()) {}

    ~LockFreeQueue() {
        T val;
        while (pop_front(val));
        delete head.load(std::memory_order_relaxed);
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        Node* prevTail = tail.load(std::memory_order_relaxed);

        do {
            Node* currentTail = prevTail;
            Node* currentNext = currentTail->next.load(std::memory_order_relaxed);
            if (currentNext == nullptr) {
                if (currentTail->next.compare_exchange_strong(currentNext, newNode)) {
                    tail.compare_exchange_strong(prevTail, newNode);
                    return;
                }
            }
            else {
                prevTail = currentNext;
            }
        } while (true);
    }

    bool pop_front(T& value) {
        Node* prevHead = head.load(std::memory_order_relaxed);
        Node* currentHead = prevHead;
        Node* currentTail = tail.load(std::memory_order_relaxed);
        Node* currentNext = currentHead->next.load(std::memory_order_relaxed);

        if (currentHead == head.load()) {
            if (currentHead == currentTail) {
                if (currentNext == nullptr) {
                    return false;
                }
                else {
                    tail.compare_exchange_strong(currentTail, currentNext);
                }
            }
            else {
                auto val = currentNext->data;
                if (head.compare_exchange_strong(prevHead, currentNext)) {
                    delete currentHead;
                    value = std::move(val);
                    return true;
                }
            }
        }
        return false;
    }
};

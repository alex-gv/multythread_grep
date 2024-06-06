#pragma once
#include <atomic>
#include <memory>

/**
 * @brief This is a lock-free queue implementation.
 *
 * The queue is based on a linked list where each node contains a data
 * element and a pointer to the next node. Access to the queue is thread-safe
 * due to the use of atomic operations. The queue has a push_back method to
 * add elements to the queue and a pop_front method to remove elements from
 * the front of the queue.
 *
 * @tparam T The type of data stored in the queue.
 */
template <typename T> class LockFreeQueue {
private:
  struct Node {
    T data;
    std::atomic<Node *> next;

    Node(const T &value) : data(value), next(nullptr) {}
  };

  std::atomic<Node *> head;
  std::atomic<Node *> tail;

public:
  LockFreeQueue() : head(new Node({})), tail(head.load()) {}

  ~LockFreeQueue() {
    T val;
    while (pop_front(val))
      ;
    delete head.load(std::memory_order_relaxed);
  }

  /**
   * @brief Adds an element to the back of the queue.
   *
   * @param value The value to add to the queue.
   */
  void push_back(const T &value) {
    Node *newNode = new Node(value);
    Node *prevTail = tail.load(std::memory_order_relaxed);

    do {
      Node *currentTail = prevTail;
      Node *currentNext = currentTail->next.load(std::memory_order_relaxed);
      if (currentNext == nullptr) {
        if (currentTail->next.compare_exchange_strong(currentNext, newNode)) {
          tail.compare_exchange_strong(prevTail, newNode);
          return;
        }
      } else {
        prevTail = currentNext;
      }
    } while (true);
  }

  /**
   * @brief Removes an element from the front of the queue.
   *
   * @param value The removed element.
   * @return true If an element was removed, false if the queue was empty.
   */
  bool pop_front(T &value) {
    Node *prevHead = head.load(std::memory_order_relaxed);
    Node *currentHead = prevHead;
    Node *currentTail = tail.load(std::memory_order_relaxed);
    Node *currentNext = currentHead->next.load(std::memory_order_relaxed);

    if (currentHead == head.load()) {
      if (currentHead == currentTail) {
        if (currentNext == nullptr) {
          return false;
        } else {
          tail.compare_exchange_strong(currentTail, currentNext);
        }
      } else {
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

  /**
   * @brief Checks if the queue is empty.
   *
   * @return true If the queue is empty, false otherwise.
   */
  bool is_empty() {
    Node *currentHead = head.load(std::memory_order_relaxed);
    Node *currentNext = currentHead->next.load(std::memory_order_relaxed);
    return currentNext == nullptr ? true : false;
  }
};

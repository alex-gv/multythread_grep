#pragma once
#include <atomic>
#include <memory>
#include <thread>


#include <gtest/gtest.h>
#include "../lock_free_queue.h"

template <typename T>
class LockFreeQueueTest : public ::testing::Test {
protected:
  LockFreeQueue<std::string> queue;

  void SetUp() override {}

  void TearDown() override {}
};

typedef ::testing::Types<int, std::string> TestTypes;

TYPED_TEST_SUITE(LockFreeQueueTest, TestTypes);

TYPED_TEST(LockFreeQueueTest, PushAndPop) {
  std::string value1 = "fdgfdsg sdfkghdkshgkdfjhg kdfshgdkshgkdjsf hg";
  this->queue.push_back(value1);
  std::string value2;
  ASSERT_TRUE(this->queue.pop_front(value2));
  ASSERT_EQ(value1, value2);
  ASSERT_TRUE(this->queue.is_empty());
}

TYPED_TEST(LockFreeQueueTest, MultipleElements) {
  this->queue.push_back("AAA");
  this->queue.push_back("BBB");
  this->queue.push_back("+-+");
  std::string value;
  ASSERT_TRUE(this->queue.pop_front(value));
  ASSERT_EQ("AAA", value);
  ASSERT_TRUE(this->queue.pop_front(value));
  ASSERT_EQ("BBB", value);
  ASSERT_TRUE(this->queue.pop_front(value));
  ASSERT_EQ("+-+", value);
  ASSERT_TRUE(this->queue.is_empty());
}

TYPED_TEST(LockFreeQueueTest, MultipleThreads) {
  std::vector<std::thread> threads;
  std::vector<std::string> values{"dfgdfgdf", "0000", "РУСКИЕ БУКВЫ", "()?#!#$", ""};
  for (auto value : values) {
    threads.push_back(std::thread([this, value]() { this->queue.push_back(value); }));
  }
  for (std::thread &thread : threads) {
    thread.join();
  }
  std::string popped_value;
  for (auto value : values) {  
    ASSERT_TRUE(this->queue.pop_front(popped_value));    
  }
  ASSERT_TRUE(this->queue.is_empty());
}


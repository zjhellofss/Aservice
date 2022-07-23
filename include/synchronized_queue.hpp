//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_SYNCHRONIZED_QUEUE_HPP_
#define TRAFFICPROCESS_INCLUDE_SYNCHRONIZED_QUEUE_HPP_
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>
template<typename T, size_t capacity = 1024, int wait_time_out = 5>
class SynchronizedQueue {
 public:
  bool push(const T &value) {
	using namespace std::chrono_literals;
	std::unique_lock<std::mutex> lk(this->mutex_);
	auto now = std::chrono::system_clock::now();
	if (!cond_.wait_until(lk, now + wait_time_out_ * 100ms, [&]() {
	  bool is_full = this->elem_queue_.size() >= this->capacity_;
	  return !is_full;
	})) {
	  return false;
	} else {
	  this->elem_queue_.push_back(value);
	  cond_.notify_one();
	  return true;
	}
  }

  bool pop(T &value) {
	using namespace std::chrono_literals;
	std::unique_lock<std::mutex> lk(this->mutex_);
	auto now = std::chrono::system_clock::now();
	if (!cond_.wait_until(lk, now + wait_time_out_ * 100ms, [&]() {
	  bool is_empty = this->elem_queue_.empty();
	  return !is_empty;
	})) {
	  return false;
	} else {
	  value = this->elem_queue_.front();
	  this->elem_queue_.pop_front();
	  cond_.notify_one();
	  return true;
	}
  }

  size_t write_available() {
	std::unique_lock<std::mutex> lk(this->mutex_);
	return this->capacity_ - this->elem_queue_.size();
  }

  size_t read_available() {
	std::unique_lock<std::mutex> lk(this->mutex_);
	return this->elem_queue_.size();
  }

 private:
  int wait_time_out_ = wait_time_out;// 500ms time out
  size_t capacity_ = capacity;
  std::deque<T> elem_queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
#endif //TRAFFICPROCESS_INCLUDE_SYNCHRONIZED_QUEUE_HPP_

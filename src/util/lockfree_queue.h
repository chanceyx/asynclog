#pragma once

#include <atomic>
#include <memory>
#include <vector>

namespace lockfreebuf {

template <typename T, size_t N = 1024>
class LockFreeQueue {
 public:
  struct Element {
    Element() : filled_(false) {}
    std::atomic<bool> filled_;
    T data_;
  };

  LockFreeQueue();
  ~LockFreeQueue();
  bool initialize();
  bool Enqueue(T value);
  bool Dequeue(T& value);

 private:
  Element* queue_ = nullptr;
  size_t size_ = 0;
  bool initialized_ = false;

  std::atomic<size_t> read_idx_;
  std::atomic<size_t> write_idx_;
};

template <typename T, size_t N>
bool LockFreeQueue<T, N>::initialize() {
  if (initialized_ == true) return true;

  queue_ = new Element[N];
  if (queue_) {
    size_ = N;
    initialized_ = true;
    return true;
  }
  return false;
}

template <typename T, size_t N>
LockFreeQueue<T, N>::LockFreeQueue()
    : queue_(nullptr),
      size_(0),
      initialized_(false),
      read_idx_(0),
      write_idx_(0) {}

template <typename T, size_t N>
LockFreeQueue<T, N>::~LockFreeQueue() {
  if (initialized_) delete[] queue_;
}

template <typename T, size_t N>
bool LockFreeQueue<T, N>::Enqueue(T value) {
  size_t write_index = 0;
  Element* e;
  bool succeed = false;
  while (!succeed) {
    write_index = write_idx_.load(std::memory_order_relaxed);
    if (write_index >= read_idx_.load(std::memory_order_relaxed) + size_)
      return false;
    size_t index = write_index % size_;
    e = &queue_[index];
    if (e->filled_.load(std::memory_order_relaxed)) {
      return false;
    }
    succeed = write_idx_.compare_exchange_weak(write_index, write_index + 1,
                                               std::memory_order_release,
                                               std::memory_order_relaxed);
  }
  e->data_ = std::move(value);
  e->filled_.store(true, std::memory_order_relaxed);
  return true;
}

template <typename T, size_t N>
bool LockFreeQueue<T, N>::Dequeue(T& value) {
  size_t read_index = 0;
  Element* e = NULL;
  bool succeed = false;
  while (!succeed) {
    read_index = read_idx_.load(std::memory_order_relaxed);
    if (read_index >= write_idx_.load(std::memory_order_relaxed)) {
      return false;
    }
    size_t index = read_index % size_;
    e = &queue_[index];
    if (!e->filled_.load(std::memory_order_relaxed)) {
      return false;
    }
    succeed = read_idx_.compare_exchange_weak(read_index, read_index + 1,
                                              std::memory_order_release,
                                              std::memory_order_relaxed);
  }
  value = std::move(e->data_);
  e->filled_.store(false, std::memory_order_release);

  return true;
}

}  // namespace lockfreebuf

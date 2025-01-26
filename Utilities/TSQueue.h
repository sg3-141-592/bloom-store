#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class TSQueue {
private:
  std::queue<T> m_queue;
  mutable std::mutex m_mutex;
  std::condition_variable m_not_full;
  std::condition_variable m_not_empty;
  size_t m_max_size;

public:
  // Constructor with explicit max size
  explicit TSQueue(size_t max_size) : m_max_size(max_size) {}

  // Push with blocking when queue is full
  void push(T item) {
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait until queue has space
    m_not_full.wait(lock, [this]() { return m_queue.size() < m_max_size; });

    // Add item
    m_queue.push(std::move(item));

    // Notify waiting pop threads
    lock.unlock();
    m_not_empty.notify_one();
  }

  // Non-blocking push with return status
  bool try_push(T item) {
    std::unique_lock<std::mutex> lock(m_mutex);

    // Check if queue is full
    if (m_queue.size() >= m_max_size) {
      return false;
    }

    m_queue.push(std::move(item));
    lock.unlock();
    m_not_empty.notify_one();
    return true;
  }

  // Pop with blocking wait
  T pop() {
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait until queue is not empty
    m_not_empty.wait(lock, [this]() { return !m_queue.empty(); });

    // Retrieve and remove item
    T item = std::move(m_queue.front());
    m_queue.pop();

    // Notify waiting push threads
    lock.unlock();
    m_not_full.notify_one();

    return item;
  }

  // Non-blocking pop
  std::optional<T> try_pop() {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_queue.empty()) {
      return std::nullopt;
    }

    T item = std::move(m_queue.front());
    m_queue.pop();

    lock.unlock();
    m_not_full.notify_one();

    return item;
  }

  // Get current queue size
  size_t size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queue.size();
  }
};
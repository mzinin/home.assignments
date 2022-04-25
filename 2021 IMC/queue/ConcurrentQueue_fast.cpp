#include <atomic>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

template<typename T, uint64_t SIZE = 4096, uint64_t MAX_PUSH_ATTEMPTS = 40000000>
class ConcurrentQueue {
private:
    static constexpr unsigned Log2(unsigned n, unsigned p = 0) {
        return (n <= 1) ? p : Log2(n / 2, p + 1);
    }

    static constexpr uint64_t closestExponentOf2(uint64_t x) {
        return (1UL << ((uint64_t)(Log2(SIZE - 1)) + 1));
    }

    static constexpr uint64_t mRingModMask = closestExponentOf2(SIZE) - 1;
    static constexpr uint64_t mSize = closestExponentOf2(SIZE);

    std::pair<std::atomic_flag, T> mMem[mSize];
    std::atomic_uint64_t mReadPtr{0};
    std::atomic_uint64_t mWritePtr{0};

public:
    bool try_pop(T& aResult) {
        auto& pair = mMem[mReadPtr & mRingModMask];
        if (!pair.first.test()) {
            return false;
        }

        aResult = pair.second;
        pair.first.clear();
        ++mReadPtr;
        return true;
    }

    bool try_push(const T& aItem) {
        const auto writePos = mWritePtr++;

        for (uint64_t attempt = 0; attempt < MAX_PUSH_ATTEMPTS; ++attempt) {
            if (writePos >= mReadPtr + mSize) {
                continue;
            }
            auto& pair = mMem[writePos & mRingModMask];
            pair.second = aItem;
            pair.first.test_and_set();
            return true;
        }

        return false;
    }

    bool try_push(T&& aItem) {
        const auto writePos = mWritePtr++;

        for (uint64_t attempt = 0; attempt < MAX_PUSH_ATTEMPTS; ++attempt) {
            if (writePos >= mReadPtr + mSize) {
                continue;
            }
            auto& pair = mMem[writePos & mRingModMask];
            pair.second = std::move(aItem);
            pair.first.test_and_set();
            return true;
        }

        return false;
    }
};

int main(int, char**) {
    using Functor = std::function<void()>;

    ConcurrentQueue<Functor*> queue;

    std::thread consumer([&] {
        Functor* task = nullptr;
        while (true) {
            if (queue.try_pop(task)) {
                (*task)();
                delete task;
            }
        }
    });

    std::thread producer([&] {
        uint64_t counter = 0;
        while (true) {
            auto taskId = counter++;
            auto newTask = new Functor([ = ] {
                std::cout << "Running task " << taskId << std::endl << std::flush;
            });
            if (!queue.try_push(newTask)) {
                throw std::runtime_error("Concurrent queue full cannot write to it!");
            }
        }
    });

    consumer.join();
    producer.join();
    return 0;
}

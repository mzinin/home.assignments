#include <functional>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

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

    T mMem[mSize];
    mutable std::mutex mLock;
    uint64_t mReadPtr = 0;
    uint64_t mWritePtr = 0;

public:
    bool try_pop(T& aResult) {
        std::lock_guard<std::mutex> lock(mLock);
        if (!peek()) {
            return false;
        }

        aResult = mMem[mReadPtr & mRingModMask];
        mReadPtr++;
        return true;
    }

    bool try_push(const T& aItem) {
        for (uint64_t attempt = 0; attempt < MAX_PUSH_ATTEMPTS; ++attempt) {
            std::lock_guard<std::mutex> lock(mLock);
            if (getCount() == mSize) {
                continue;
            }

            mMem[mWritePtr & mRingModMask] = aItem;
            mWritePtr++;
            return true;
        }
        return false;
    }

    bool try_push(T&& aItem) {
        for (uint64_t attempt = 0; attempt < MAX_PUSH_ATTEMPTS; ++attempt) {
            std::lock_guard<std::mutex> lock(mLock);
            if (getCount() == mSize) {
                continue;
            }

            mMem[mWritePtr & mRingModMask] = std::move(aItem);
            mWritePtr++;
            return true;
        }
        return false;
    }

private:
    bool peek() const {
        return (mWritePtr != mReadPtr);
    }

    uint64_t getCount() const {
        return mWritePtr - mReadPtr;
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

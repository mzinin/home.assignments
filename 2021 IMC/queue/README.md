# Concurrent Queue Code Review

## `Q: Would you check in your code like this?`

A: No, as I find some lines of the code quite suspicious and some - redundant (see details below). Also more or less through testing can easily reproduce crashes, especially in the case of slow consumer and multiple producers.

## `Q: Do you think the code is clean?`

A: No, I as can see:
* Redundant `#include <cmath>` directive.
* Questionable `getCount` method implementation. According to my understanding `mWritePtr` can never be less then `mReadPtr`, so no need to compare them. Current code looks like a leftover from the previous version, when both pointers were indices of the `mMem` buffer.
* User shouldn't have access to some of the methods, so they should be private (`getCount` and `busyWaitForPush`).

## `Q: What changes would you suggest in a code review?`

A: This definitely depends on the way the queue is expected to be used. Still I could point at the following issues:

* `pop()` methods returns reference to the inner buffer, so consumer gets an unprotected by mutex reference, while producer can update the same item. In the provided use case that leads to skipping of one task and double free of another.

* `peek()` is called twice within `pop()` - before and after the mutex is locked. Is it an optimization? If so, it is not obvious to me. In general calling `peek()` under no mutex protection is wrong, as `mWritePtr` can be changed (increased) in another thread. So `peek()` can return false-negative result, which does no harm is this particular case, but also isn't useful.

* `pop()` returns a default empty value if the queue is empty. That means producers shouldn't be allowed to push such values in the queue. I would propose to combine `peek` and `pop` operations in to one atomic operation, something like `bool try_pop(T& output)`.

* `getCount()` also operates with non-synchronized variables under no mutex protection. So it can produce false-positive results (I mean `getCount() != mSize` while it is actually `mSize`). As a result producers may update elements of the `mMem` buffer which are not yet processed by consumer.

* I would also propose to make push operation return a boolean value (to distinguish successful pushes from failed ones) instead of throwing an exception. Let the user decide what to do in the case of the full queue - wait or stop with error.

Please check the result of applying my suggestions in the `ConcurrentQueue_clean.cpp` file.

## `Q: Do you see room for improving the performance without breaking thread safety?`

The very first idea is to get rid of the mutex. That will prevent producers from blocking each other, and more generally will prevent threads from switching to kernel space and back.

The result can be checked in the `ConcurrentQueue_fast.cpp` file (compilable with C++20). A thorough benchmarking is required, but the quick one confirms that lock-free variant of the queue is faster.

## `Q: What steps would you take to find the bug?`

1. Localize it, make a minimum reproducing use case.
2. Analyze the code, find and eliminate suspicious things (like accessing non-synchronized data from different threads), try to understand how the data is read and written.
3. Run the problem code in debugger and/or tools like `valgrind`.
4. Analyze some compiler's intermediate results (AST for example) to get some clues or to justify a hypotheses.
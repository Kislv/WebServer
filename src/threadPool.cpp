#include "../include/threadPool.h"
#include "../include/job.h"
#include <utility>

#define CORESQUANTITY 8

ThreadPool::ThreadPool(unsigned int threadNumber, std::string documentRoot) : documentRoot(std::move(documentRoot)) {
    threadNumber = threadNumber;
    if (threadNumber < 1) {
        threadNumber = 1;
    }
    if (threadNumber > CORESQUANTITY) {
        threadNumber = CORESQUANTITY;
    }

    for (int i = 0; i < threadNumber; i++) {
        threads.emplace_back(&ThreadPool::Work, this);
    }
}

void ThreadPool::AddToQueue(int socket, const std::string &requestString) {
    std::lock_guard<std::mutex> g(queueMutex);
    queue.push(std::pair<int, std::string>(socket, requestString));
    queueCV.notify_one();
}

void ThreadPool::Work() {
    while (true) {
        std::pair<int, std::string> request;

        std::unique_lock<std::mutex> g(queueMutex);
        queueCV.wait(g, [&]{
            return !queue.empty();
        });

        request = queue.front();
        queue.pop();

        handle(request.first, request.second);
    }
}

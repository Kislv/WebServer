#pragma once

#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>

class ThreadPool {
public:
    void AddToQueue(int socket, const std::string& requestString);
    ThreadPool(unsigned int threadNumber, std::string documentRoot);

private:
    unsigned int threadNumber;
    std::condition_variable_any queueCV;
    std::queue<std::pair<int, std::string>> queue;
    std::mutex queueMutex;
    std::string documentRoot;
    std::vector<std::thread> threads;
    void Work();
};

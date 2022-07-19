#include <mutex>

std::mutex coutAccess;

void coutLock() {
    coutAccess.lock();
}

void coutUnlock() {
    coutAccess.unlock();
}
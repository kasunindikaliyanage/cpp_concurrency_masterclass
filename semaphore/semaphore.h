#pragma once
#include <mutex>
#include <condition_variable>

/** General semaphore with N permissions **/
class Semaphore {
	const size_t num_permissions;
	size_t avail;
	std::mutex m;
	std::condition_variable cv;
public:
	/** Default constructor. Default semaphore is a binary semaphore **/
	explicit Semaphore(const size_t& num_permissions = 1) : num_permissions(num_permissions), avail(num_permissions) { }

	/** Copy constructor. Does not copy state of mutex or condition variable,
		only the number of permissions and number of available permissions **/
	Semaphore(const Semaphore& s) : num_permissions(s.num_permissions), avail(s.avail) { }

	void acquire() {
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [this] { return avail > 0; });
		avail--;
	}

	void release() {
		//TODO : here also use unique_locks
		m.lock();
		avail++;
		m.unlock();
		cv.notify_one();
	}

	size_t available() const {
		return avail;
	}
};
#pragma once

#include <mutex>
#include <memory>
#include <atomic>

//http://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html

// Double lock broken in C++, it depends on the memory model of the processor,
//the reorderings performed by the compiler and the interaction between the compiler and the synchronization library.
//Explicit memory barriers can be used to make it work in C++
class Singleton
{
	static Singleton* instance;
	static std::mutex mu1;
	static std::mutex mu2;
	Singleton() {}
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	friend class Singleton_destroyer;
	static Singleton* getSingleton()
	{		
		if (instance == nullptr)
		{
			std::unique_lock<std::mutex>lock(mu1);
			if (instance == nullptr)
			{
				//std::unique_lock<std::mutex>lock(mu2);
				instance = new Singleton();
			}
		}

		return instance;
	}
};
Singleton* Singleton::instance;

class Singleton_destroyer {
public:
	~Singleton_destroyer()
	{ 
		delete Singleton::instance;
	}
};

// Creation is also thread safe
//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class Singleton_1
{
	Singleton_1() {}
	// Note: Scott Meyers mentions in his Effective Modern
	//  C++ book, that deleted functions should generally
	//  be public as it results in better error messages
	//  due to the compilers behavior to check accessibility
	//  before deleted status
public:
	Singleton_1(const Singleton_1&) = delete;
	void operator=(const Singleton_1 &) = delete;

	Singleton_1& getSingleton()
	{
		static Singleton_1 instance;
		return instance;
	}
};

class Singleton_2
{
public:
	Singleton_2(Singleton const&) = delete;
	Singleton_2& operator=(Singleton const&) = delete;

	static std::shared_ptr<Singleton_2> instance()
	{
		static std::shared_ptr<Singleton_2> s{ new Singleton_2() };
		return s;
	}

private:
	Singleton_2() {}
};

class Singleton_3
{
	static std::once_flag onceflag;
	static Singleton_3* instance;
	Singleton_3() {}
	Singleton_3() = default;
	~Singleton_3() = default;
	Singleton_3(const Singleton_3&) = delete;
	Singleton_3& operator=(const Singleton_3&) = delete;
public:
	static Singleton_3& getInstance()
	{
		std::call_once(onceflag, []() { instance = new Singleton_3(); });
		return *instance;
	}
};
Singleton_3* Singleton_3::instance = nullptr;
std::once_flag Singleton_3::onceflag;

class xyz {
};
template <class X>
X& singleton_4()
{
	static X x;
	return x;
}
xyz obj = singleton_4<xyz>();



//The beauty of the Meyers Singleton in C++11 is that it's automatically thread safe. 
//That is guaranteed by the standard: Static variables with block scope.
//The Meyers Singleton is a static variable with block scope, so we are done. 
//It's still left to rewrite the program for four threads.

class MySingleton {
public:
	static MySingleton& getInstance() {
		static MySingleton instance; // object creation always perform in critical section so OS itself block other thread/process to access this critical section.
		return instance;
	}
private:
	MySingleton() = default;
	~MySingleton() = default;
	MySingleton(const MySingleton&) = delete;
	MySingleton& operator=(const MySingleton&) = delete;
};

class MySingleton {
public:

	std::atomic<MySingleton*> m_instance;
	std::mutex m_mutex;

	MySingleton* getInstance() {
		MySingleton* tmp = m_instance.load(std::memory_order_relaxed);
		std::atomic_thread_fence(std::memory_order_acquire);
		if (tmp == nullptr) {
			std::lock_guard<std::mutex> lock(m_mutex);
			tmp = m_instance.load(std::memory_order_relaxed);
			if (tmp == nullptr) {
				tmp = new MySingleton;
				std::atomic_thread_fence(std::memory_order_release);
				m_instance.store(tmp, std::memory_order_relaxed);
			}
		}
		return tmp;
	}
}

/* references
https://en.cppreference.com/w/cpp/thread/once_flag
https://www.modernescpp.com/index.php/thread-safe-initialization-of-a-singleton
https://preshing.com/20130930/double-checked-locking-is-fixed-in-cpp11/

*/
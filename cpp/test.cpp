#include "common.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

static mutex gmutex;
static vector<void*> ptrs;
static const int niter = 1000000;

void produce()
{
    RingBuf rb(2 * 1024 * 1024); // 2 MB
    srand(time(nullptr)); // use current time as seed for random generator
    while (true) {
        for (int i = 0; i < 32; ++i) {
            int size = 1 + rand() % 1024;
            void* ptr = rb.alloc(size);
            rb.freeze(size);
            {
                lock_guard<mutex> guard(gmutex);
                ptrs.push_back(ptr);
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void consume()
{
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(1));
        {
            lock_guard<mutex> guard(gmutex);
            for (size_t i = 0; i < ptrs.size(); ++i) {
                RingBuf::dealloc(ptrs[i]);
            }
            ptrs.clear();
        }
    }
}

int main()
{   
    thread t1(produce);
    thread::id t1_id = t1.get_id();
 
    thread t2(consume);
    thread::id t2_id = t2.get_id();
 
    cout << "t1's id: " << t1_id << '\n';
    cout << "t2's id: " << t2_id << '\n';
 
    t1.join();
    t2.join();
}

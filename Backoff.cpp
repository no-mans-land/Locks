#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <cmath>

using namespace std;

class Backoff{
	int min_delay = 1;
	int max_delay = 5;
	atomic<bool> state;
	public:
   	
	Backoff(){
		state.store(false, memory_order_relaxed);
	}

	void lock(){
		int delay = min_delay;
		while(true){
			while(state.load(memory_order_relaxed)){
				if(!state.exchange(true)){
					return;
				}
				else{
					sleep(rand()%delay);
					if(delay < max_delay)
							delay = 2 * delay;
				}
			}
		}
	}
	void unlock(){
		state.store(false, memory_order_relaxed);
	}
};

void testCS(int id, int k, Backoff &testLock){
	for(int i=0; i<k; i++){
		// get the time
		// cout<<"thread "<<id<<" request the cs\n";
		testLock.lock();
		cout<<"thread "<<id<<" enter the cs\n";
		sleep(1);
		testLock.unlock();
		// cout<<"thread "<<id<<"exit the cs\n";
	}
	return;
}

int main(){
	int n=5;
	int k=5;


	Backoff testLock;
	thread thrdArr[n];
	for(int i=0; i<n; i++){
		thrdArr[i] = thread(testCS, i, k, ref(testLock));
	}
	
	for(int i=0; i<n; i++){
		thrdArr[i].join();
	}

	return 1;
}




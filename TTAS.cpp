#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>

using namespace std;

class TTASLock{
	atomic<bool> state;
	public:
   	
	TTASLock(){
		state.store(false, memory_order_relaxed);
	}

	void lock(){
		while(true){
				while(state.load(memory_order_relaxed)){
				}
				if(!state.exchange(true)){
					return;
				}
		}
	}
	void unlock(){
		state.store(false, memory_order_relaxed);
	}
};

void testCS(int id, int k, TTASLock &testLock){
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


	TTASLock testLock;
	thread thrdArr[n];
	for(int i=0; i<n; i++){
		thrdArr[i] = thread(testCS, i, k, ref(testLock));
	}
	
	for(int i=0; i<n; i++){
		thrdArr[i].join();
	}

	return 1;
}




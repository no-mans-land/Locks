#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <cmath>
#include <time.h>
#include <mutex>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

mutex mtx;

class BackoffLock{
	int min_delay = 1;
	int max_delay = 5;
	atomic<bool> state;
	public:
   	
	BackoffLock(){
		state.store(false, memory_order_relaxed);
	}

	void lock(){
		int delay = min_delay;
		while(true){
			while(state.load(memory_order_relaxed))
			{}
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
	void unlock(){
		state.store(false, memory_order_relaxed);
	}
};

void testCS(int id, int k, BackoffLock &testLock, int lmb1, int lmb2){

	default_random_engine generator(1); // 1 is seed    
	exponential_distribution<double> exp_dist1(1/lmb1);
	exponential_distribution<double> exp_dist2(1/lmb2);
	for(int i=0; i<k; i++){
		// get the time
	
		time_t rawtime;
		time(&rawtime);
		struct tm *timeinfo = localtime(&rawtime);

		mtx.lock();
		cout<<i+1<<" CS requested by thread "<<id<<" at "<<asctime(timeinfo);
		mtx.unlock();

		testLock.lock();
		mtx.lock();
		cout<<i+1<<" CS Entered by thread "<<id<<" at "<<asctime(timeinfo);
		mtx.unlock();
		double num1 = exp_dist1(generator);
		sleep(num1);

		testLock.unlock();
		
		mtx.lock();
		cout<<i+1<<" CS Exited by thread "<<id<<" at "<<asctime(timeinfo);
		mtx.unlock();
		double num2 = exp_dist2(generator);
		sleep(num2);

	}
	return;
}

int main(int argc, char const *argv[]){
	int N, k, lmb1, lmb2;

	// read input from the file
	ifstream inFile;
	string filename = argv[1];
	inFile.open(filename);
    if(!inFile){
        cout<<"unable to openfile"<<endl;
        exit(1);
    }

    string line;
    while(getline(inFile, line)){
         istringstream iss(line);

         int x;
         // get the distribution logparameters
		 // n k lmb1 lmb2
         int cn = 0;
         while((iss>>x)){
             if(cn == 0)
                 N = x;
             else if(cn == 1)
                 k = x;
			 else if(cn == 2)
				lmb1 = x;
			 else if(cn == 3)
				lmb2 = x;
             cn++;
         }
    }


	BackoffLock testLock;
	thread thrdArr[N];
	for(int i=0; i<N; i++){
		thrdArr[i] = thread(testCS, i, k, ref(testLock), lmb1, lmb2);
	}
	
	for(int i=0; i<N; i++){
		thrdArr[i].join();
	}

	return 1;
}


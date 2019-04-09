#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <mutex>
#include <sstream>
#include <fstream>
#include <random>

using namespace std;

mutex mtx;

class TASLock{
	atomic<bool> state;
	public:
   	
	TASLock(){
		state.store(false, memory_order_relaxed);
	}

	void lock(){
		while(state.exchange(true)){
		}
	}
	void unlock(){
		state.store(false, memory_order_relaxed);
	}
};

void testCS(int id, int k, TASLock &testLock, int lmb1, int lmb2){
	
		
	default_random_engine generator(1); // 1 is seed	
	exponential_distribution<double> exp_dist1(1/lmb1);
	exponential_distribution<double> exp_dist2(1/lmb2);	

		
	for(int i=0; i<k; i++){
		// ---------- get the time
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

		// sleep
		double num1 = exp_dist1(generator);
		sleep(num1);
		testLock.unlock();
		
		mtx.lock();
		cout<<i+1<<" CS Exited by thread "<<id<<" at "<<asctime(timeinfo);
		mtx.unlock();
		// sleep
		double num2 = exp_dist2(generator);
		sleep(num2);


	}
	return;
}

int main(int argc, char const *argv[1]){
	
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

	TASLock testLock;
	thread thrdArr[N];
	for(int i=0; i<N; i++){
		thrdArr[i] = thread(testCS, i, k, ref(testLock), lmb1, lmb2);
	}
	
	for(int i=0; i<N; i++){
		thrdArr[i].join();
	}

	inFile.close();
	return 1;
}




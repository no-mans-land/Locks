#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <time.h>
#include <mutex>
#include <sstream>
#include <fstream>


using namespace std;

mutex mtx;

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
		sleep(1);
		testLock.unlock();
		// cout<<"thread "<<id<<"exit the cs\n";
		
		mtx.lock();
		cout<<i+1<<" CS Exited by thread "<<id<<" at "<<asctime(timeinfo);
		mtx.unlock();

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


	TTASLock testLock;
	thread thrdArr[N];
	for(int i=0; i<N; i++){
		thrdArr[i] = thread(testCS, i, k, ref(testLock));
	}
	
	for(int i=0; i<N; i++){
		thrdArr[i].join();
	}

	return 1;
}



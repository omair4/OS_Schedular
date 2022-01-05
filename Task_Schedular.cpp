#include <iostream>
#include <fstream>
#include <pthread.h>
#include <string>
#include <stdio.h>
#include <list>
#include <iterator>
#include <queue>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

struct process{
	int process_num;
	int arrival_time;	
	int state;		//ready=1;  active=2;  waiting=3;  terminated=4;  
	int instruction_num;	//number of instruction a process has
	int Q_name;		// ready_FCFS=10;   ready_SRR=11; ready_SJF=12
				//wait_Hardrive=20; wait_Net=21 ; wait_Device=22
//	int start_IO;
	int has_IO;
	int IO_pos;
	int instructions_done;
	int IO_type;		//DISK=0 ; SCREEN=1; PRINTER=2; KEYBOARD=3; NETWORK=4;
};

int g_Time=0;
list < process > PCB;
queue <process> FCFS;
queue <process> SRR;
queue <process> SJF;
queue <process> terminated;
queue <process> w_HDD;
queue <process> w_network;
queue <process> w_device;


void* process_create(void * arg);
void* ReadyQ(void *);
void fcfs2active();
void srr2active();
void sjf2active();
void srr2sjf(process );


int main(){
	pthread_t pCreateID,pReadyID;
	pthread_create(&pCreateID,NULL,process_create,NULL);
	pthread_create(&pReadyID,NULL,ReadyQ,NULL);
	pthread_join(pCreateID,NULL);
	pthread_join(pReadyID,NULL);

}

void* process_create(void* arg){

	process p[5];
	ofstream fP;
	string fname;
	for(int i=0;i<5;i++){
		p[i].process_num=i;
		fname="P"+to_string(i);
		fP.open(fname,ios::out);	//creating file like P0.txt
		fP<<"Start\n";				//and writing start to it.
		fP.close();
		p[i].arrival_time=g_Time;		//storing arrival time
		p[i].instruction_num=rand()%30;		//randomly generating number of instruction from 0-29
		p[i].has_IO=rand()%2;			//has IO or not decided
		if(p[i].has_IO){
			p[i].IO_pos=rand()%p[i].instruction_num;		//If process has IO then its position is decided which is between 0 to no.instruction
			p[i].IO_type=rand()%5;					//Type of instruction is decided e.g printer harddrive etc.
		}
		p[i].instructions_done=0;
		p[i].state=1;  //making the state ready
		PCB.push_back(p[i]);
		FCFS.push(p[i]);	//pushing into ready FCFS Queue
		sleep(rand()%4);
	}
	//Printing process list "JOBS.TXT"
	
	ofstream fJobs("jobs.txt",ios::out);
	
	list <process> :: iterator itr;
	for(itr=PCB.begin();itr!=PCB.end();itr++){
		fJobs<<"P"<<itr->process_num<<"  "<<itr->arrival_time<<" Instructions: "<<itr->instruction_num<<endl;
		
	}
	fJobs.close();
		
}

void* ReadyQ(void* arg){
	
	while(1){
		if(!FCFS.empty())	fcfs2active();
		else if(!SRR.empty())   srr2active();
		else if(!SJF.empty())   sjf2active();
	}
			
}

void IO_checking(process tempActive,string filename){
	ofstream f;
	f.open(filename,ios::out|ios::app);
	if((tempActive.IO_type==1)||(tempActive.IO_type==2)||(tempActive.IO_type==3)){	w_device.push(tempActive);
													if(tempActive.IO_type==1){ f<<"READ SCREEN 2 ticks\n"; g_Time+=2; 
																   FCFS.push(w_device.front());  w_device.pop(); }
													if(tempActive.IO_type==2){ f<<"WRITE PRINTER 2 ticks\n"; g_Time+=2; 
																   FCFS.push(w_device.front());  w_device.pop(); }
													if(tempActive.IO_type==3){ f<<"READ KEYBOARD 2 ticks\n"; g_Time+=2; 
																   FCFS.push(w_device.front());  w_device.pop(); }
	}
	if(tempActive.IO_type==0){							w_HDD.push(tempActive);
													f<<"WRITE DISK 2 ticks\n"; g_Time+=2;FCFS.push(w_HDD.front());  w_HDD.pop(); }
	if(tempActive.IO_type==4){							w_network.push(tempActive);
													f<<"WRITE NETWORK 2 ticks\n"; g_Time+=2;FCFS.push(w_network.front());  w_network.pop(); }
	f.close();
}
	

void fcfs2active(){
	//cout<<"FCFS2active\n";
	process tempActive=FCFS.front();
	FCFS.pop();
	string file="P" + to_string(tempActive.process_num);
	//cout<<file<<endl;
	ofstream f;
	int cpu_cycles;
	f.open(file,ios::out|ios::app);
	if(tempActive.instruction_num<=3){
		if((tempActive.has_IO)&&((tempActive.instructions_done+tempActive.instruction_num)>tempActive.IO_pos)&&(tempActive.instructions_done<tempActive.IO_pos)){
			cpu_cycles=tempActive.instructions_done+tempActive.instruction_num-tempActive.IO_pos;
			g_Time+=cpu_cycles;
			tempActive.instructions_done+=cpu_cycles;
			for(int i=0;i<cpu_cycles;i++)
				f<<"COMPUTE(FCFS)\n";		//TODO: remove fcfs from file output
			//f<<"END\n";
			f.close();
			IO_checking(tempActive,file);
			
		}
		else{
			g_Time+=tempActive.instruction_num;	//incrementing g_Time
			tempActive.instructions_done+=tempActive.instruction_num;
			terminated.push(tempActive);	//process is terminated if instruction <=3
			for(int i=0;i<tempActive.instruction_num;i++)
				f<<"COMPUTE(FCFS)\n";		//TODO: remove fcfs from file output
			f<<"END\n";
			f.close();
		}
	}
	else{
		if((tempActive.has_IO)&&((tempActive.instructions_done+3)>tempActive.IO_pos)&&(tempActive.instructions_done<tempActive.IO_pos)){	//if program has IO and IO position is now then
			cpu_cycles=tempActive.instructions_done+3-tempActive.IO_pos;
			g_Time+=cpu_cycles;
			tempActive.instructions_done+=cpu_cycles;
			for(int i=0;i<cpu_cycles;i++)
				f<<"COMPUTE(FCFS)\n";		//TODO: remove fcfs from file output
			//f<<"END\n";
			f.close();
			IO_checking(tempActive,file);
		}
		else{
			g_Time+=3;	//incrementing g_Time
			tempActive.instructions_done+=3;
			for(int i=0;i<3;i++)
				f<<"COMPUTE(FCFS)\n";
			tempActive.instruction_num-=3;
			SRR.push(tempActive);
			f.close();
		}
	}
	//TODO: Printing to files
	//TODO: changing states
	//TODO:Implement IO operation in between
}

void srr2active(){//TODO: see what selfish RR is.
	//cout<<"SRR2Active\n";
	process tempActive=SRR.front();
	SRR.pop();
	int cpu_cycles;
	string file="P" + to_string(tempActive.process_num);
	ofstream f;
	f.open(file,ios::out|ios::app);
	if(tempActive.instruction_num<=6){
		if((tempActive.has_IO)&&((tempActive.instructions_done+tempActive.instruction_num)>tempActive.IO_pos)&&(tempActive.instructions_done<tempActive.IO_pos)){	
			cpu_cycles=tempActive.instructions_done+tempActive.instruction_num-tempActive.IO_pos;
			g_Time+=cpu_cycles;
			tempActive.instructions_done+=cpu_cycles;
			for(int i=0;i<cpu_cycles;i++)
				f<<"COMPUTE(SRR)\n";		//TODO: remove fcfs from file output
			//f<<"END\n";
			f.close();
			IO_checking(tempActive,file);
		}
		else{
			g_Time+=tempActive.instruction_num;	//incrementing g_Time
			tempActive.instructions_done+=tempActive.instruction_num;
			terminated.push(tempActive);	//process is terminated if instruction <=6
			for(int i=0;i<tempActive.instruction_num;i++)
				f<<"COMPUTE(SRR)\n";
			f<<"END\n";
			f.close();
		}
		
	}
	else{
		if((tempActive.has_IO)&&((tempActive.instructions_done+6)>tempActive.IO_pos)&&(tempActive.instructions_done<tempActive.IO_pos)){	//if program has IO and IO position is now then
			cpu_cycles=tempActive.instructions_done+6-tempActive.IO_pos;
			g_Time+=cpu_cycles;
			tempActive.instructions_done+=cpu_cycles;
			for(int i=0;i<cpu_cycles;i++)
				f<<"COMPUTE(SRR)\n";		//TODO: remove fcfs from file output
			//f<<"END\n";
			f.close();
			IO_checking(tempActive,file);
		}
		else{
			g_Time+=6;	//incrementing g_Time
			tempActive.instructions_done+=6;
			tempActive.instruction_num-=6;
			for(int i=0;i<6;i++)
				f<<"COMPUTE(SRR)\n";
			f.close();
			SJF.push(tempActive);
			srr2sjf(tempActive);
		}
	}
	//TODO: Printing to files
	//TODO: changing states
	//TODO:Implement IO operation in between

}

void srr2sjf(process temp){//to sort according to instruction num

	int size=SJF.size();
	process* tem=new process [size];
	process* sorted=new process [size];
	for(int i=0;i<size;i++){
		tem[i]=SJF.front();
		SJF.pop();
	}
	process max;
	int index,sindex;
	for(int i=0;i<size;i++){
		max=tem[0];
		for(int j=0;j<size;j++){
			if((max.instruction_num<=tem[j].instruction_num)&&(tem[j].instruction_num!=-1))
			{	
				max=tem[j]; //save index of min
				index=j;
			}
		}
		sindex=size-1-i;
		sorted[sindex]=max;
		tem[index].instruction_num=-1;
	}
	for(int i=0;i<size;i++){
		SJF.push(sorted[i]);
	}
			
	delete[] tem;
	delete[] sorted;		
}

void sjf2active(){
	//cout<<"SJF started\n";
	process tempActive;
	tempActive=SJF.front();
	SJF.pop();
	int cpu_cycles;
	string file="P" + to_string(tempActive.process_num);
	ofstream f;
	f.open(file,ios::out|ios::app);
	if((tempActive.has_IO)&&((tempActive.instructions_done+tempActive.instruction_num)>tempActive.IO_pos)&&(tempActive.instructions_done<tempActive.IO_pos)){	
			cpu_cycles=tempActive.instructions_done+tempActive.instruction_num-tempActive.IO_pos;
			g_Time+=cpu_cycles;
			tempActive.instructions_done+=cpu_cycles;
			for(int i=0;i<cpu_cycles;i++)
				f<<"COMPUTE(SJF)\n";		//TODO: remove fcfs from file output
			//f<<"END\n";
			f.close();
			IO_checking(tempActive,file);
		}
	else{
		g_Time+=tempActive.instruction_num;
		tempActive.instructions_done+=tempActive.instruction_num;
		for(int i=0;i<tempActive.instruction_num;i++)
				f<<"COMPUTE(SJF)\n";
		f<<"END\n";
		f.close();
		terminated.push(tempActive);
	}

	//TODO: printing to files
	//TODO: changing states
	//TODO:Implement IO operation in between
}


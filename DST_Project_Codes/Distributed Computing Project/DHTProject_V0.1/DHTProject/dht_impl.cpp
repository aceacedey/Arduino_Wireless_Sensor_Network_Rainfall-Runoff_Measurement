#include "dht_impl.h"
#include<cmath>

ChordDHT* ChordDHT::dht_instance = NULL;

DHTReturnCode ChordDHT::SpawnNodes(int number_process)
{
	if(number_process)
	{
   		if(fork()==0)
    		{
      			if(number_process)
        		{
		            int node_id = getpid();
			    int identifier = HashFunction(node_id);
           		    this->SpawnNodes(number_process-1);
        		}
       			 else
          			return;
   		 }
	}		
}


ChordDHT::ChordDHT()
{
	process_count = 0;
}
ChordDHT* ChordDHT::GetInstance()
{
	if(dht_instance)
	{
		return dht_instance;
	}
	else
	{
		dht_instance = new ChordDHT();
		return dht_instance;
	}
}
void ChordDHT::DeleteInstance()
{
	if(dht_instance)
	{
		delete dht_instance;
		dht_instance = NULL;
	}
}


DHTReturnCode ChordDHT::ChordDHTInitialize(int max_process,int start_process,int interval,bool ins_del)
{
	//Get the maximum number of process i.e, 2^m
	this->max_process = max_process;
	//calculate m 
	this->m_bit = log(max_process) /log(2);		

	//Start all the process
	SpawnNodes(start_process);
		
	
	//Has function to map the node id to the m-bit identifier
	//Assign an m bit identifier to each of the process	
	//Form a logical ring{predecessor and successor info updated}
}
DHTReturnCode ChordDHT::ChordDHTTerminate()
{
	
}
DHTReturnCode ChordDHT::ChordDHTInsert(int key)
{
	
}
DHTReturnCode ChordDHT::ChordDHTDelete(int key)
{
	
}
DHTReturnCode ChordDHT::ChordDHTPrintStatus()
{
	
}

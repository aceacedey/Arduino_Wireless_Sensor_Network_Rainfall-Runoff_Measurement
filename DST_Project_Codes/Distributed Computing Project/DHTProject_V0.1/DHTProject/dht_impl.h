#include<stdio.h>
#include "dht_helper.h"

class ChordDHT
{
	private:
		static ChordDHT* dht_instance; //single instance of the class
		int m_bit ; //m bit identifier	
		int max_process;
		int process_count;	
		std::list<NodeInfo> node_list;	
	public:
		ChordDHT(); //constructor
		~ChordDHT(); //destructor
		static ChordDHT* GetInstance();
		DHTReturnCode ChordDHTInitialize();//Class initialization has to be done through this method
		DHTReturnCode ChordDHTTerminate();//Class Terminate has to be done through this method
		DHTReturnCode ChordDHTInsert(int key) ;
		DHTReturnCode ChordDHTTerminate(int key);
		DHTReturnCode ChordDHTPrintStatus();
		DHTReturnCode ChordDHTGetNodeSuccessor(int node_id,int& succ_node_id);
		DHTReturnCode ChordDHTGetNodePredecessor(int node_id,int& pred_node_id);
		DHTReturnCode ChordDHTGetKeySuccessor(int key,int& key_succ_id);
	
};

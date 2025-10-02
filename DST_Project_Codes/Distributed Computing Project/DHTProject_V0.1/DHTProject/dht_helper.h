#include<stdio.h>

typedef enum
{
 DHT_SUCCESS,
 DHT_FAILURE,
 DHT_NODEDOWN
}DHTReturnCode;

std::list<int> dht_local;
typeder struct
{
	int node_identier;
	int pred_identifier;
	int succ_identifier;
	int number_keys;
	dht_local local_table;
}NodeInfo;

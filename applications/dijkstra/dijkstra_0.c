#include <api.h>
#include <stdlib.h>
//#include <stdio.h>
#include "dijkstra.h"

#define NONE                       9999		//Maximum
#define MAXPROCESSORS			   64		//The amount of processor
#define NUM_NODES                  16		//16 for small input; 160 for large input; 30 for medium input;

struct _NODE{
	int iDist;
	int iPrev;
	int iCatched;
};
typedef struct _NODE NODE;

struct _UVERTEX{
	int iPID;
	int iNID;
	int iDist;
};
typedef struct _UVERTEX UVERTEX;

UVERTEX uVertex[MAXPROCESSORS];
NODE rgnNodes[MAXPROCESSORS][NUM_NODES];
int g_qCount[MAXPROCESSORS];
int paths;
int resultSend[33];

int tasks[MAXPROCESSORS][2];
int nodes_tasks[NUM_NODES*(NUM_NODES-1)/2][2];
int AdjMatrix[NUM_NODES][NUM_NODES];

int globalMiniCost[MAXPROCESSORS];
int qtdEnvios;

int main(int argc, char *argv[])
{
	int i, j;
	Message msg;
	int rank = 0;

	RealTime(DEADLINE, DEADLINE, EXEC_TIME);

	qtdEnvios = 0;

	msg.length = NUM_NODES*(NUM_NODES-1)/2;
	Receive(&msg, divider);
	for (i=0; i<(NUM_NODES*(NUM_NODES-1)/2); i++)
		nodes_tasks[i][0] = msg.msg[i];

	Receive(&msg, divider);
	for (i=0; i<(NUM_NODES*(NUM_NODES-1)/2); i++)
		nodes_tasks[i][1] = msg.msg[i];


	msg.length = MAXPROCESSORS;
	Receive(&msg, divider);
	/*Echo("\n OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO msg.msg[i]");
	Echo(itoa(msg.msg[0]));
	Echo("\n");*/
	for (i=0; i<MAXPROCESSORS; i++) {
		tasks[i][0] = msg.msg[i];
		/*Echo("\n pppppppppppppppppppppp tasks[i][0]");
		Echo(itoa(tasks[i][0]));
		Echo("\n");*/
	}

	Receive(&msg, divider);
	/*Echo("\n OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO msg.msg[i]");
	Echo(itoa(msg.msg[0]));
	Echo("\n");*/
	for (i=0; i<MAXPROCESSORS; i++) {
		tasks[i][1] = msg.msg[i];
		/*Echo("\n pppppppppppppppppppppp tasks[i][1]");
		Echo(itoa(tasks[i][1]));
		Echo("\n");*/
	}


	msg.length = NUM_NODES;
	for (i=0; i<NUM_NODES; i++) {
		Receive(&msg, divider);
		for (j=0; j<NUM_NODES; j++)
			AdjMatrix[j][i] = msg.msg[j];
	}

	/*for(i=0; i<NUM_NODES; i++) {
		Echo(" D1: ");
		for(j=0; j<NUM_NODES; j++) {
			Echo(itoa(AdjMatrix[i][j]));
			Echo(" ");
		}
		Echo("\n");
	}*/

	//dijkstra(rank);

	int x,v;
	int chStart, chEnd;
	int u =-1;

	RealTime(43200, 43200, 10800); //RealTime(103349, 103349, 10500, 0) = 10% utilization

	for(x=tasks[rank][0]; x<tasks[rank][1]; x++){
		chStart = nodes_tasks[x][0];	//Start node
		chEnd = nodes_tasks[x][1];		//End node
		u=-1;

		//Initialize and clear
		uVertex[rank].iDist=NONE;
		uVertex[rank].iPID=rank;
		uVertex[rank].iNID=NONE;
		g_qCount[rank] = 0;
		u=-1;
		for (v=0; v<NUM_NODES; v++) {
			rgnNodes[rank][v].iDist =  AdjMatrix[chStart][v];
			rgnNodes[rank][v].iPrev = NONE;
			rgnNodes[rank][v].iCatched = 0;
		}
		//Start working
		while (qcount(rank) < NUM_NODES-1){
			for (i=0; i<NUM_NODES; i++) {
				if(rgnNodes[rank][i].iCatched==0 && rgnNodes[rank][i].iDist<uVertex[rank].iDist && rgnNodes[rank][i].iDist!=0){
					uVertex[rank].iDist=rgnNodes[rank][i].iDist;
					uVertex[rank].iNID=i;
				}
			}
			globalMiniCost[rank]=NONE;
			if(globalMiniCost[rank]>uVertex[rank].iDist){
				globalMiniCost[rank] = uVertex[rank].iDist;
				u=uVertex[rank].iNID;
				g_qCount[rank]++;
			}
			for (v=0; v<NUM_NODES; v++) {
				if(v==u){
					rgnNodes[rank][v].iCatched = 1;
					continue;
				}
				if((rgnNodes[rank][v].iCatched==0 && rgnNodes[rank][v].iDist>(rgnNodes[rank][u].iDist+AdjMatrix[u][v]))){
					rgnNodes[rank][v].iDist=rgnNodes[rank][u].iDist+AdjMatrix[u][v];
					rgnNodes[rank][v].iPrev = u;
				}
			}
			uVertex[rank].iDist = NONE;	//Reset
		}

		sendResult(rank,chStart,chEnd);
		qtdEnvios++;
	}

	//Message msg;
	msg.length = 33;
	msg.msg[0] = -1;
	Send(&msg, print);
	Echo("finaliza\n");

	Echo(itoa(GetTick()));
	Echo("Dijkstra_0 finished.");



	/*Echo("\n OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO tasks[rank][0]");
	Echo(itoa(tasks[rank][0]));
	Echo("\n");

	Echo("\n OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO tasks[rank][1]");
	Echo(itoa(tasks[rank][1]));
	Echo("\n");*/

	exit();
}

int qcount (int myID){
	return(g_qCount[myID]);
}

void sendPath(NODE *rgnNodes, int chNode){
	if ((rgnNodes+chNode)->iPrev != NONE){
		sendPath(rgnNodes, (rgnNodes+chNode)->iPrev);
	}
	resultSend[paths] = chNode+1;
	paths++;
}

void sendResult(int myID,int chStart, int chEnd){
	paths = 3;
	int k;
	for(k=0; k<33; k++)
		resultSend[k] = 0;
	resultSend[0] = chStart;
	resultSend[1] = chEnd;
	resultSend[2] = rgnNodes[myID][chEnd].iDist;
	sendPath(rgnNodes[myID], chEnd);

	Message msg;
	msg.length = 33;

	for(k=0; k<33; k++)
		msg.msg[k] = resultSend[k];
	Send(&msg, print);
}

void dijkstra(int myID) {

}

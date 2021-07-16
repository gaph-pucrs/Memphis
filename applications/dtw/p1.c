#include <api.h>
#include "dtw.h"

Message msg;

int main(){

	int test[SIZE][SIZE];
	int pattern[SIZE][SIZE];
	int result, j;

	Receive(&msg, recognizer);

	Echo("Task P1 INIT\n");

	__builtin_memcpy(test, msg.msg, sizeof(test));

	for(j=0; j<PATTERN_PER_TASK; j++){

		Echo("Task P1 FOR\n");

		memset(msg.msg,0, sizeof(int)*MSG_SIZE);

		Receive(&msg, bank);

		//Echo("Task P1 received pattern from bank\n");

		__builtin_memcpy(pattern, msg.msg, sizeof(pattern));

		result = dynamicTimeWarping(test, pattern);

		msg.length = 1;

		msg.msg[0] = result;

		Send(&msg, recognizer);
	}

	Echo("Task P1 FINISHEDD IN\n");
	Echo(itoa(GetTick()));

	exit();
}

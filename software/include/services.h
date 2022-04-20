/*!\file services.h
 * MEMPHIS VERSION - 8.0 - support for RT applications
 *
 * Distribution:  June 2016
 *
 * Edited by: Marcelo Ruaro - contact: marcelo.ruaro@acad.pucrs.br
 *
 * Research group: GAPH-PUCRS   -  contact:  fernando.moraes@pucrs.br
 *
 * \brief  Kernel services definitions. This services are used to
 * identifies a packet.
 */

#ifndef __SERVICES_H__
#define __SERVICES_H__

#define 	MESSAGE_REQUEST					0x00000010
#define 	MESSAGE_DELIVERY				0x00000020
#define 	TASK_ALLOCATION     			0x00000040
#define 	TASK_ALLOCATED     				0x00000050
#define 	TASK_REQUEST        			0x00000060
#define 	TASK_TERMINATED     			0x00000070
#define 	LOAN_PROCESSOR_RELEASE			0x00000090
#define 	DEBUG_MESSAGE  					0x00000100
#define 	TASK_MAPPING_REPORT				0x00000130
#define	 	APP_TERMINATED					0x00000140
#define		NEW_APP							0x00000150 //Injector to Mestre (carries App descriptor)
#define		INITIALIZE_CLUSTER				0x00000160
#define		INITIALIZE_SLAVE				0x00000170
#define		TASK_TERMINATED_OTHER_CLUSTER	0x00000180
#define		LOAN_PROCESSOR_REQUEST			0x00000190
#define 	LOAN_PROCESSOR_DELIVERY			0x00000200
#define 	TASK_MIGRATION					0x00000210
#define 	MIGRATION_CODE					0x00000220
#define 	MIGRATION_TCB					0x00000221
#define 	MIGRATION_TASK_LOCATION			0x00000222
#define 	MIGRATION_MSG_REQUEST			0x00000223
#define 	MIGRATION_STACK					0x00000224
#define 	MIGRATION_DATA_BSS				0x00000225
#define 	UPDATE_TASK_LOCATION			0x00000230
#define 	TASK_MIGRATED					0x00000235
#define 	APP_ALLOCATION_REQUEST			0x00000240 //Mestre to Injector (carries tasks properties and mapping)
#define 	TASK_RELEASE					0x00000250
#define 	SLACK_TIME_REPORT				0x00000260
#define 	DEADLINE_MISS_REPORT			0x00000270
#define 	REAL_TIME_CHANGE				0x00000280
#define		NEW_APP_REQ						0x00000290 //Injector to Mestre (carries num_tasks)
#define		APP_REQ_ACK						0x00000300 //Mestre to Injector (carries cluster addr)
#define		MIGRATION_REQUEST				0x00000770

#endif


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               proc.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"


void initializeAllPro()	//重新初始化所有的进程，加入到不同的进程优先级队列中
{
	PROCESS* p;
	firstLen=0;
	secondLen=0;
	firstHead=0;
	int i;
	for (i=0; i<NR_TASKS+NR_PROCS; i++)
	{
		p=&proc_table[i]; 
		if (p->state!=kRUNNABLE) continue;
		if (p->priority>=10)
		{
			firstQueue[firstLen]=p;
			firstLen++;
			p->ticks=2;
			p->whichQueue=1;
			
		}
		else
		{
			secondQueue[secondLen]=p;
			secondLen++;
			p->ticks=p->priority;
			p->whichQueue=2;
		}
	}
}

/*======================================================================*
                              schedule
 *======================================================================*/


PUBLIC void schedule()
{
	PROCESS*	p;
	int 		greatest_priority=0;
	int 		i;
	
	while (!greatest_priority) 
	{
		if (firstLen-firstHead>0)
		{		
			p_proc_ready=firstQueue[firstHead];	//第一个队列按照先到先得
			greatest_priority=p_proc_ready->ticks;
			break;
		}
		else						//第二个队列按照优先级
		{
			for (i=0; i<secondLen; i++)		//第二个队列增设判断是否为runnable状态
			{
				p=secondQueue[i];
				if (p->state!=kRUNNABLE || p->ticks==0) continue;
				if (p->ticks > greatest_priority) 
				{
					greatest_priority = p->ticks;
					p_proc_ready = p;
				}
				/*{	下面代码被卡死。。。我也不懂为什么。。
					if (p->priority>greatest_priority && p->ticks!=0)
					{
						greatest_priority=p->priority;
						p_proc_ready=p;
					}
				}*/
			}
		}
		if (!greatest_priority)	initializeAllPro();
	}
	p_proc_ready->state=kRUNNING;
}


/*======================================================================*
                           sys_get_ticks
 *======================================================================*/
PUBLIC int sys_get_ticks()
{
	return ticks;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            main.c
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

int strcmp(char *str1,char *str2)
{
	int i;
	for (i=0; i<strlen(str1); i++)
	{
		if (i==strlen(str2)) return 1;
		if (str1[i]>str2[i]) return 1;
		else if (str1[i]<str2[i]) return -1;
	}
	return 0;
}

void strlwr(char *str)
{
	int i;
	for (i=0; i<strlen(str); i++)
	{
		if ('A'<=str[i] && str[i]<='Z') str[i]=str[i]+'a'-'A';
	}
}

void addToQueue(PROCESS* p)
{
	p->state=kRUNNABLE;
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

/*======================================================================*
                            tinix_main
 *======================================================================*/
PUBLIC int tinix_main()
{
	//disp_str("-----\"tinix_main\" begins-----\n");
	clearScreen();
	disp_str("         *************************************************************\n");
	disp_str("        *                                                             \n");
	disp_str("       * **********    ******    ***      **    ******    **    **    \n");
	disp_str("      *      **          **      ****     **      **        *  *      \n");
	disp_str("     *       **          **      ** **    **      **         **       \n");
	disp_str("    *        **          **      **  **   **      **         **       \n");
	disp_str("   *         **          **      **   **  **      **         **       \n");
	disp_str("  *          **          **      **    ** **      **         **       \n");
	disp_str(" *           **          **      **     ****      **       *    *     \n");
	disp_str("*            **        ******    **      ***    ******   **      **   \n");
	disp_str("*                                                                     \n");
	disp_str("**********************************************************************\n");
	TASK*		p_task;
	PROCESS*	p_proc		= proc_table;
	char*		p_task_stack	= task_stack + STACK_SIZE_TOTAL;
	t_16		selector_ldt	= SELECTOR_LDT_FIRST;
	int		i;
	t_8		privilege;
	t_8		rpl;
	int		eflags;
	for(i=0;i<NR_TASKS+NR_PROCS;i++){
		if (i < NR_TASKS) {	/* 任务 */
			p_task		= task_table + i;
			privilege	= PRIVILEGE_TASK;
			rpl		= RPL_TASK;
			eflags		= 0x1202;	/* IF=1, IOPL=1, bit 2 is always 1 */
		}
		else {			/* 用户进程 */
			p_task		= user_proc_table + (i - NR_TASKS);
			privilege	= PRIVILEGE_USER;
			rpl		= RPL_USER;
			eflags		= 0x202;	/* IF=1, bit 2 is always 1 */
		}

		strcpy(p_proc->name, p_task->name);	/* name of the process */
		p_proc->pid	= i;			/* pid */

		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	/* change the DPL */
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;/* change the DPL */
		p_proc->regs.cs		= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss		= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs		= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip	= (t_32)p_task->initial_eip;
		p_proc->regs.esp	= (t_32)p_task_stack;
		p_proc->regs.eflags	= eflags;

		p_proc->nr_tty		= 0;

		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}

	//修改这里的优先级和ticks
	proc_table[0].priority = 15;
	proc_table[1].priority =  5;
	proc_table[2].priority =  5;
	proc_table[3].priority =  5;
	proc_table[4].priority =  7;
	proc_table[5].priority =  10;
	proc_table[6].priority =  10;

	//对优先队列初始化
	firstLen=firstHead=secondLen=0;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		addToQueue(proc_table+i);
	}
	//指定控制台
	proc_table[1].nr_tty = 0;
	proc_table[2].nr_tty = 1;
	proc_table[3].nr_tty = 1;
	proc_table[4].nr_tty = 1;
	proc_table[5].nr_tty = 1;
	proc_table[6].nr_tty = 2;

	k_reenter	= 0;
	ticks		= 0;

	p_proc_ready	= proc_table;

	init_clock();

	restart();

	while(1){}
}

void clearScreen()
{
	int i;
	disp_pos=0;
	for(i=0;i<80*25;i++)
	{
		disp_str(" ");
	}
	disp_pos=0;
}


void help()
{
	printf("           *////////////////////////////////////////////*/\n");
	printf("                   design by Doubi && LiangPuHe         \n");
	printf("           *////////////////////////////////////////////*/\n");
	printf("\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("      *////  help         --------  shwo the help menu     ////*\n");
	printf("      *////  clear        --------  clear screen           ////*\n");
	printf("      *////  alt+F2       --------  show the process run   ////*\n");
	printf("      *////  alt+F3       --------  goBang game            ////*\n");
	printf("      *////  alt+F4       --------  guess number game      ////*\n");
	printf("      *////  kill 2~5     --------  kill the process 2~5   ////*\n");
	printf("      *////  start 2~5    --------  start the process 2~5  ////*\n");
	printf("      *////  show         --------  show the process state ////*\n");
	printf("      *////////////////////////////////////////////////////////*\n");
	printf("\n");
}

void show()
{
	PROCESS* p;
	int i;
	for (i=0; i<NR_TASKS+NR_PROCS;i++)
	{
		p=&proc_table[i];
		printf("process%d:",p->pid);
		switch (p->state)
		{
		case kRUNNABLE:
			printf("    Runnable\n");
			break;
		case kRUNNING:
			printf("    Running\n");
			break;
		case kREADY:
			printf("    Ready\n");
			break;
		}
	}
}

void readOneStringAndOneNumber(char* command,char* str,int* number)
{
	int i;
	int j=0;
	for (i=0; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]==' ') break;
		str[j]=command[i];
		j++;
	}
	for (; i<strlen(command); i++)
	{
		if (command[i]!=' ') break;
	}

	*number=0;
	for (; i<strlen(command) && '0'<=command[i] && command[i]<='9'; i++)
	{
		*number=*number*10+(int) command[i]-'0';
	}
}

void dealWithCommand(char* command)
{
	strlwr(command);
	if (strcmp(command,"clear")==0)
	{
		clearScreen();
		sys_clear(tty_table);
		return ;
	}
	if (strcmp(command,"help")==0)
	{
		help();
		return ;
	}
	if (strcmp(command,"show")==0)
	{
		show();
		return ;
	}

	char str[100];
	int number;
	readOneStringAndOneNumber(command,str,& number);
	if (strcmp(str,"kill")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kREADY;
			printf("kill process %d successful\n",number);
		}
		return ;
	}
	if (strcmp(str,"start")==0)
	{
		if (number<0 || number>NR_TASKS+NR_PROCS)
		{
			printf("No found this process!!");
		}
		else if (number==0 || number==6)
		{
			printf("You do not have sufficient privileges\n");
		}
		else if (2<=number && number <=5)
		{
			proc_table[number].state=kRUNNABLE;
			printf("start process %d successful\n",number);
		}
		return ;
	}
	printf("can not find this command\n");
}

/*======================================================================*
                               Terminal
 *======================================================================*/
void Terminal()
{
	TTY *p_tty=tty_table;
	p_tty->startScanf=0;
	while(1)
	{
		printf("DB=>");
		//printf("<Ticks:%x>", get_ticks());
		openStartScanf(p_tty);
		while (p_tty->startScanf) ;
		dealWithCommand(p_tty->str);
	}
}


/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
	int i = 0;
	while(1){
		printf("B");
		milli_delay(1000);
	}
}



/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()
{
	int i = 0;
	while(1){
		printf("C");
		milli_delay(1000);
	}
}

void TestD()
{
	int i=0;
	while (1)
	{
		printf("D");
		milli_delay(1000);
	}
}

void TestE()
{
	int i=0;
	while (1)
	{
		printf("E");
		milli_delay(1000);
	}
}
/*======================================================================*
				goBangGame
*=======================================================================*/
char gameMap[15][15];
TTY *goBangGameTty=tty_table+2;

void readTwoNumber(int* x,int* y)
{
	int i=0;
	*x=0;
	*y=0;
	for (i=0; i<goBangGameTty->len && goBangGameTty->str[i]==' '; i++);
	for (; i<goBangGameTty->len && goBangGameTty->str[i]!=' '  && goBangGameTty->str[i]!='\n'; i++)
	{
		*x=(*x)*10+(int) goBangGameTty->str[i]-48;
	}
	for (i; i<goBangGameTty->len && goBangGameTty->str[i]==' '; i++);
	for (; i<goBangGameTty->len && goBangGameTty->str[i]!=' ' && goBangGameTty->str[i]!='\n'; i++)
	{
		*y=(*y)*10+(int) goBangGameTty->str[i]-48;
	}
}

int max(int x,int y)
{
	return x>y?x:y;
}

int selectPlayerOrder()
{
	printf("o player\n");
	printf("* computer\n");
	printf("who play first?[1/user  other/computer]");
	openStartScanf(goBangGameTty);
	while (goBangGameTty->startScanf) ;
	if (strcmp(goBangGameTty->str,"1")==0) return 1;
	else return 0;
}

void displayGameState()
{
	sys_clear(goBangGameTty);
	int n=15;
	int i,j;
	for (i=0; i<=n; i++)
	{
		if (i<10) printf("%d   ",i);
		else printf("%d  ",i);
	}
	printf("\n");
	for (i=0; i<n; i++)
	{
		if (i<9) printf("%d   ",i+1);
		else printf("%d  ",i+1);
		for (j=0; j<n; j++)
		{
			if (j<10) printf("%c   ",gameMap[i][j]);
			else printf("%c   ",gameMap[i][j]);
		}
		printf("\n");
	}

}

int checkParameter(int x, int y)	//检查玩家输入的参数是否正确
{
	int n=15;
	if (x<0 || y<0 || x>=n || y>=n) return 0;
	if (gameMap[x][y]!='_') return 0;
	return 1;
}

//更新的位置为x，y，因此 只要检查坐标为x，y的位置
int win(int x,int y)		//胜利返回1    否则0（目前无人获胜）
{
	int n=15;
	int i,j;
	int gameCount;
	//左右扩展
	gameCount=1;
	for (j=y+1; j<n; j++)
	{
		if (gameMap[x][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	for (j=y-1; j>=0; j--)
	{
		if (gameMap[x][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	if (gameCount>=5) return 1;

	//上下扩展
	gameCount=1;
	for (i=x-1; i>0; i--)
	{
		if (gameMap[i][y]==gameMap[x][y]) gameCount++;
		else break;
	}
	for (i=x+1; i<n; i++)
	{
		if (gameMap[i][y]==gameMap[x][y]) gameCount++;
		else break;
	}
	if (gameCount>=5) return 1;

	//正对角线扩展
	gameCount=1;
	for (i=x-1,j=y-1; i>=0 && j>=0; i--,j--)
	{
		if (gameMap[i][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	for (i=x+1,j=y+1; i<n && j<n; i++,j++)
	{
		if (gameMap[i][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	if (gameCount>=5) return 1;

	//负对角线扩展
	gameCount=1;
	for (i=x-1,j=y+1; i>=0 && j<n; i--,j++)
	{
		if (gameMap[i][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	for (i=x+1,j=y-1; i<n && j>=0; i++,j--)
	{
		if (gameMap[i][j]==gameMap[x][y]) gameCount++;
		else break;
	}
	if (gameCount>=5) return 1;

	return 0;
}

void free1(int x,int y1,int y2,int* ff1,int* ff2)
{
	int n=15;
	int i;
	int f1=0,f2=0;
	for (i=y1; i>=0; i++)
	{
		if (gameMap[x][i]=='_') f1++;
		else break;
	}
	for (i=y2; i<n; i++)
	{
		if (gameMap[x][i]=='_') f2++;
		else break;
	}
	*ff1=f1;
	*ff2=f2;
}

void free2(int x1,int x2,int y,int *ff1,int *ff2)
{
	int n=15;
	int i;
	int f1=0,f2=0;
	for (i=x1; i>=0; i--)
	{
		if (gameMap[i][y]=='_') f1++;
		else break;
	}
	for (i=x2; i<n; i++)
	{
		if (gameMap[i][y]=='_') f2++;
		else break;
	}
	*ff1=f1;
	*ff2=f2;
}

void free3(int x1,int y1,int x2,int y2,int *ff1,int *ff2)
{
	int n=15;
	int x,y;
	int f1=0;
	int f2=0;
	for (x=x1,y=y1; 0<=x && 0<=y; x--,y--)
	{
		if (gameMap[x][y]=='_') f1++;
		else break;
	}
	for (x=x2,y=y2; x<n &&  y<n; x++,y++)
	{
		if (gameMap[x][y]=='_') f2++;
		else break;
	}
	*ff1=f1;
	*ff2=f2;
}

void free4(int x1,int y1,int x2,int y2,int *ff1,int *ff2)
{
	int n=15;
	int x,y;
	int f1=0,f2=0;
	for (x=x1,y=y1; x>=0 && y<n; x--,y++)
	{
		if (gameMap[x][y]=='_') f1++;
		else break;
	}
	for (x=x2,y=y2; x<n && y>=0; x++,y--)
	{
		if (gameMap[x][y]=='_') f2++;
		else break;
	}
	*ff1=f1;
	*ff2=f2;
}

int getPossibleByAD(int attack,int defence,int attackFree1,int attackFree2,int defenceFree1,int defenceFree2)
{
	if (attack>=5) return 20;						//5攻击
	if (defence>=5) return 19;						//5防御
	if (attack==4 && (attackFree1>=1 && attackFree2>=1)) return 18;		//4攻击 2边
	if (attack==4 && (attackFree1>=1 || attackFree2>=1)) return 17;		//4攻击 1边
	if (defence==4 && (defenceFree1>=1 || defenceFree2>=1)) return 16;	//4防御
	if (attack==3 && (attackFree1>=2 && attackFree2>=2)) return 15;		//3攻击 2边
	if (defence==3 && (defenceFree1>=2 && defenceFree2>=2)) return 14;	//3防御 2边
	if (defence==3 && (defenceFree1>=2 || defenceFree2>=2)) return 13;	//3防御 1边
	if (attack==3 && (attackFree1>=2 || attackFree2>=2)) return 12;		//3攻击 1边
	if (attack==2 && (attackFree1>=3 && attackFree2>=3)) return 11;		//2攻击 2边
	if (defence==2 && defenceFree1+defenceFree2>=3) return 10;	//2防御 2边
	if (defence==2 && defenceFree1+defenceFree2>=3) return 9;		//2防御 1边
	if (attack==1 && attackFree1+attackFree2>=4) return 8;
	if (defence==1 && defenceFree1+defenceFree2>=4) return 7;
	return 6;
}

int getPossible(int x,int y)
{
	int n=15;
	int attack;
	int defence;
	int attackFree1;
	int defenceFree1;
	int attackFree2;
	int defenceFree2;
	int possible=-100;

	//左右扩展
	int al,ar;
	int dl,dr;
	//横向攻击
	for (al=y-1; al>=0; al--)
	{
		if (gameMap[x][al]!='*') break;
	}
	for (ar=y+1; ar<n; ar++)
	{
		if (gameMap[x][ar]!='*') break;
	}
	//横向防守
	for (dl=y-1; dl>=0; dl--)
	{
		if (gameMap[x][dl]!='o') break;
	}
	for (dr=y+1; dr<n; dr++)
	{
		if (gameMap[x][dr]!='o') break;
	}
	attack=ar-al-1;
	defence=dr-dl-1;
	free1(x,al,ar,&attackFree1,&attackFree2);
	free1(x,dl,dr,&defenceFree1,&defenceFree2);
	possible=max(possible,getPossibleByAD(attack,defence,attackFree1,attackFree2,defenceFree1,defenceFree2));

	//竖向进攻
	for (al=x-1; al>=0; al--)
	{
		if (gameMap[al][y]!='*') break;
	}
	for (ar=x+1; ar<n; ar++)
	{
		if (gameMap[ar][y]!='*') break;
	}
	//竖向防守
	for (dl=x-1; dl>=0; dl--)
	{
		if (gameMap[dl][y]!='o') break;
	}
	for (dr=x+1; dr<n; dr++)
	{
		if (gameMap[dr][y]!='o') break;
	}
	attack=ar-al-1;
	defence=dr-dl-1;
	free2(al,ar,y,&attackFree1,&attackFree2);
	free2(dl,dr,y,&defenceFree1,&defenceFree2);
	possible=max(possible,getPossibleByAD(attack,defence,attackFree1,attackFree2,defenceFree1,defenceFree2));

	//正对角线进攻
	int al1,al2,ar1,ar2;
	int dl1,dl2,dr1,dr2;
	for (al1=x-1,al2=y-1; al1>=0 && al2>=0; al1--,al2--)
	{
		if (gameMap[al1][al2]!='*') break;
	}
	for (ar1=x+1,ar2=y+1; ar1<n && ar2<n; ar1++,ar2++)
	{
		if (gameMap[ar1][ar2]!='*') break;
	}
	//正对角线防守
	for (dl1=x-1,dl2=y-1; dl1>=0 && dl2>=0; dl1--,dl2--)
	{
		if (gameMap[dl1][dl2]!='o') break;
	}
	for (dr1=x+1,dr2=y+1; dr1<n && dr2<n; dr1++,dr2++)
	{
		if (gameMap[dr1][dr2]!='o') break;
	}
	attack=ar1-al1-1;
	defence=dr1-dl1-1;
	free3(al1,al2,ar1,ar2,&attackFree1,&attackFree2);
	free3(dl1,dl2,dr1,dr2,&defenceFree1,&defenceFree2);
	possible=max(possible,getPossibleByAD(attack,defence,attackFree1,attackFree1,defenceFree1,defenceFree2));

	//负对角线进攻
	for (al1=x-1,al2=y+1; al1>=0 && al2<n; al1--,al2++)
	{
		if (gameMap[al1][al2]!='*') break;
	}
	for (ar1=x+1,ar2=y-1; ar1<n && ar2>=0; ar1++,ar2--)
	{
		if (gameMap[ar1][ar2]!='*') break;
	}
	//负对角线防守
	for (dl1=x-1,dl2=y+1; dl1>=0 && dl2<n; dl1--,dl2++)
	{
		if (gameMap[dl1][dl2]!='o') break;
	}
	for (dr1=x+1,dr2=y-1; dr1<n && dr2>=0; dr1++,dr2--)
	{
		if (gameMap[dr1][dr2]!='o') break;
	}
	attack=ar1-al1-1;
	defence=dr1-dl1-1;
	free4(al1,al2,ar1,ar2,&attackFree1,&attackFree2);
	free4(dl1,dl2,dr1,dr2,&defenceFree1,&defenceFree2);
	possible=max(possible,getPossibleByAD(attack,defence,attackFree1,attackFree2,defenceFree1,defenceFree2));
	return possible;
}


void goBangGameStart()
{
	int playerStep=0;
	int computerStep=0;
	int n=15;
	int i,j;
	while (1)
	{
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			gameMap[i][j]='_';


	if (selectPlayerOrder()==0)
	{
		gameMap[n>>1][n>>1]='*';
		displayGameState();
		printf("[computer step:%d]%d,%d\n",++computerStep,(n>>1)+1,(n>>1)+1);
	}
	else
	{
		displayGameState();
	}

	while (1)
	{
		int x,y;
		while (1)
		{
			printf("[player step:%d]",++playerStep);
			//scanf("%d%d",&x,&y);
			openStartScanf(goBangGameTty);
			while (goBangGameTty->startScanf) ;
			readTwoNumber(&x,&y);
			x--,y--;
			if ( checkParameter(x,y) )
			{
				gameMap[x][y]='o';
				break;
			}
			else
			{
				playerStep--;
				printf("the position you put error\n");
			}
		}
		if (win(x,y))
		{
			displayGameState();
			printf("Congratulation you won the game\n");
			break;
		}
		int willx,willy,winPossible=-100;
		for (i=0; i<n; i++)
			for (j=0; j<n; j++)
			{
				if (gameMap[i][j]=='_')
				{
					int possible=getPossible(i,j);
					if (possible>=winPossible)
					{
						willx=i; willy=j;
						winPossible=possible;
					}
				}
			}
			gameMap[willx][willy]='*';
			displayGameState();
			printf("[computer step:%d]%d,%d\n",++computerStep,willx+1,willy+1);
			if (win(willx,willy))
			{
				printf("Sorry you lost the game\n");
				break;
			}
	}
	}

}


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 15
#define CHARSIZE 2
#define live5 1000000
#define live4 10000
#define live3 2000
#define live2 10
#define sleep4 1000
#define sleep3 100
#define sleep2 5

void initRecordBoard(void);
void recordtoDisplayArray(void);
void displayBoard(void);

//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//棋盘基本模板 
char aInitDisplayBoardArray[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组用于显示棋盘 
char aDisplayBoardArray[SIZE][SIZE*CHARSIZE+1];
 
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";
int x[4]={0,1,1,1};
int y[4]={1,1,0,-1};
//此数组用于记录棋盘格局 
int aRecordBoard[SIZE][SIZE];

int faraway(int i,int j){
	int direct,k;
	for (direct=0;direct<=3;direct++){
		for (k=-2;k<=2;k++){
			if ((j+k*x[direct])>0 && (j+k*x[direct])<14 && (i+k*y[direct])>0 && (i+k*y[direct])<14){
				if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]!=0)
					return 1;
			}
		}
	}
	return 0;
}

//void aimove(int flag);
int val(int count,int len,int len2,int block){
	if (len>=5&&count>1){
		if (count==5){
			return live5;
		}
		if (len2-count==0){
			if (len>=5 && len2<5 && block==0){
				switch(count){
					case 2:return live2;
					case 3:return live3;
					case 4:return live4;
				}
			}
			else{
				switch(count){
					case 2:return sleep2;
					case 3:return sleep3;
					case 4:return sleep4;
				}
			}
		}
		else{
			if (len>=5 && len2<5 && block==0){
				switch(count){
					case 2:return live2/2;
					case 3:return live3/2;
					case 4:return live4/2;
				}
			}
			else{
				switch(count){
					case 2:return sleep2/2;
					case 3:return sleep3/2;
					case 4:return sleep4/2;
				}
			}
		}			
	}
	return 0;
}

void estimate(int flag){
	int i,j,k,direct;
	int empty=0,count=1,len=1,len2=1,block=0;
	int value,maxval,meval,hisval,stepval;
	int bestmove[2];
	for (i=0;i<SIZE;i++){
		for (j=0;j<SIZE;j++){
			if((aRecordBoard[i][j]==3) ||(aRecordBoard[i][j]==4))
				aRecordBoard[i][j]-=2;
		}
	}
	maxval=0;
	for (i=0;i<SIZE;i++){
		for (j=0;j<SIZE;j++){
			if ((aRecordBoard[i][j]==0) && (faraway(i,j)==1)){
				value=0;
				for (direct=0;direct<=3;direct++){
					empty=0;count=1;len=1;len2=1;block=0;
					for (k=1;k<=4;k++){
						if ((j+k*x[direct])<0 || (j+k*x[direct])>14 ||(i+k*y[direct])<0 ||(i+k*y[direct])>14)
							break;
						if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==0){
							++len;
							++empty;
						}
						else if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==flag%2+1){
							if (empty+count>4)
								break;
							count++;
							len++;
							len2=empty+count;
						}
						else{
							if(len2==empty+count)
								block++;
							break;
						}
					}
					empty=len2-count;
					for (k=-1;k>=-4;k--){
						if ((j+k*x[direct])<0 || (j+k*x[direct])>14 ||(i+k*y[direct])<0 ||(i+k*y[direct])>14)
							break;
						if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==0){
							++len;
							++empty;
						}
						else if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==flag%2+1){
							if (empty+count>4)
								break;
							count++;
							len++;
							len2=empty+count;
						}
						else{
							if(len2==empty+count)
								block++;
							break;
						}
					}
					value+=val(count,len,len2,block);
				}
				meval=value;
				value=0;
				for (direct=0;direct<=3;direct++){
					empty=0;count=1;len=1;len2=1;block=0;
					for (k=1;k<=4;k++){
						if ((j+k*x[direct])<0 || (j+k*x[direct])>14 ||(i+k*y[direct])<0 ||(i+k*y[direct])>14)
							break;
						if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==0){
							++len;
							++empty;
						}
						else if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==(flag+1)%2+1){
							if (empty+count>4)
								break;
							count++;
							len++;
							len2=empty+count;
						}
						else{
							if(len2==empty+count)
								block++;
							break;
						}
					}
					empty=len2-count;
					for (k=-1;k>=-4;k--){
						if ((j+k*x[direct])<0 || (j+k*x[direct])>14 ||(i+k*y[direct])<0 ||(i+k*y[direct])>14)
							break;
						if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==0){
							++len;
							++empty;
						}
						else if(aRecordBoard[i+k*y[direct]][j+k*x[direct]]==(flag+1)%2+1){
							if (empty+count>4)
								break;
							count++;
							len++;
							len2=empty+count;
						}
						else{
							if(len2==empty+count)
								block++;
							break;
						}
					}
					value+=val(count,len,len2,block);
				}
				hisval=value;
				stepval=(hisval>meval)?hisval:meval;
				if (maxval<=stepval){
					bestmove[0]=i;
					bestmove[1]=j;
					maxval=stepval;
				}
			}
		}
	}
	aRecordBoard[bestmove[0]][bestmove[1]]=flag%2+3;
}

int pending(int flag){
	int i,j,k;
	int same;
	for (i=0;i<SIZE;i++){
		for (j=0;j<SIZE;j++){
			if((aRecordBoard[i][j]==3) ||(aRecordBoard[i][j]==4))
				aRecordBoard[i][j]-=2;
		}
	}
	for (i=0;i<SIZE;i++){
		for (j=0;j<11;j++){
			same=0;
			for (k=1;k<5;k++){
				if (aRecordBoard[i][j]==0)
					break;
				if (aRecordBoard[i][j+k]==aRecordBoard[i][j])
					same++;
			}
			if (same==4){
				if ((flag%2+1)==1)
					printf("White is winner!\n");
				else if((flag%2+1)==2)
					printf("Black is winner!\n");
				return -1;
			}
		}
	}
	for (i=0;i<SIZE;i++){
		for (j=0;j<11;j++){
			same=0;
			for (k=1;k<5;k++){
				if (aRecordBoard[j][i]==0)
					break;
				if (aRecordBoard[j+k][i]==aRecordBoard[j][i])
					same++;
			}
			if (same==4){
				if ((flag%2+1)==1)
					printf("White is winner!\n");
				else if((flag%2+1)==2)
					printf("Black is winner!\n");
				return -1;
			}
		}
	}
	for (i=0;i<11;i++){
		for (j=0;j<11;j++){
			same=0;
			for (k=1;k<5;k++){
				if (aRecordBoard[i][j]==0)
					break;
				if (aRecordBoard[i+k][j+k]==aRecordBoard[i][j])
					same++;
			}
			if (same==4){
				if ((flag%2+1)==1)
					printf("White is winner!\n");
				else if((flag%2+1)==2)
					printf("Black is winner!\n");
				return -1;
			}
		}
	}
	for (i=4;i<15;i++){
		for (j=0;j<11;j++){
			same=0;
			for (k=1;k<5;k++){
				if (aRecordBoard[i][j]==0)
					break;
				if (aRecordBoard[i-k][j+k]==aRecordBoard[i][j])
					same++;
			}
			if (same==4){
				if ((flag%2+1)==1)
					printf("White is winner!\n");
				else if((flag%2+1)==2)
					printf("Black is winner!\n");
				return -1;
			}
		}
	}
	return flag;
}


int pvp()

{
	int flag=0;
	char c;
	int x,y,i,j;
	char s[10];
	initRecordBoard();
	recordtoDisplayArray();
	recordtoDisplayArray();
	displayBoard();
	printf("input should be like 'A02''D11'\n");
	for (x=0;x<10;x++)
		s[x]=0;
	while (flag!=-1){
		while (isspace(s[0]=getchar()))
			;					
		for (i=1;i<3;i++)
			s[i]=getchar();
		if (s[0]=='q')
			flag=-1;
		else{
			y=s[0]-'A';
			if (y>14){
				printf("Warning,consider revising!\n");
				continue;
			}
			if (s[2]>='0'&&s[2]<='9'){
				x=s[2]-'0'+10*(s[1]-'0')-1;
				x=14-x;
				if (x>14){
					printf("Warning,consider revising!\n");
					continue;
				}
			}
			else{
				x=s[1]-'0'-1;
				x=14-x;
			}
			if(aRecordBoard[x][y]==0){
				aRecordBoard[x][y]=flag%2+3;
				recordtoDisplayArray();
			    displayBoard();
			    getchar();
				flag++;
			}
			else{
				printf("Warning,consider revising!\n");
				continue;
			}
		}
		flag=pending(flag);
	}			
    return 0;
}

int pvc(){
	int flag=0;
	char c;
	int x,y,i,j;
	char s[10];
	int order;
	initRecordBoard();
	recordtoDisplayArray();
	recordtoDisplayArray();
	displayBoard();
	printf("Please pick the color for yourself:1.●;2.◎\n");
	scanf("%d",&order);
	if (order==2){
		aRecordBoard[7][7]=3;
		flag++;
	}
	recordtoDisplayArray();
	displayBoard();
	printf("input should be like 'A02''D11'\n");
	for (x=0;x<10;x++)
		s[x]=0;
	while (flag!=-1){
		while (isspace(s[0]=getchar()))
			;					
		for (i=1;i<3;i++)
			s[i]=getchar();
		if (s[0]=='q')
			flag=-1;
		else{
			y=s[0]-'A';
			if (y>14){
				printf("Warning,consider revising!\n");
				continue;
			}
			if (s[2]>='0'&&s[2]<='9'){
				x=s[2]-'0'+10*(s[1]-'0')-1;
				x=14-x;
				if (x>14){
					printf("Warning,consider revising!\n");
					continue;
				}
			}
			else{
				x=s[1]-'0'-1;
				x=14-x;
			}
			if(aRecordBoard[x][y]==0){
				aRecordBoard[x][y]=flag%2+3;
				recordtoDisplayArray();
			    displayBoard();
			    getchar();
				flag++;
			}
			else{
				printf("Warning,consider revising!\n");
				continue;
			}
		}
		flag=pending(flag);
		if (flag!=-1){
			estimate(flag);
			flag++;
			recordtoDisplayArray();
		    	displayBoard();
			flag=pending(flag);
		}
	}			
    return 0;
}

//初始化棋盘格局 
void initRecordBoard(void){
	int i,j;
	for (i=0;i<SIZE;i++)
		for (j=0;j<SIZE;j++)
			aRecordBoard[i][j]=0;
	return;
//通过双重循环，将aRecordBoard清0
}

//将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void){
	int i,j;
	for (i=0;i<SIZE;i++)
		for (j=0;j<SIZE*2+1;j++)
			aDisplayBoardArray[i][j]=aInitDisplayBoardArray[i][j];
	for (i=0;i<SIZE;i++){
		for (j=0;j<SIZE;j++){
			if(aRecordBoard[i][j]==1){
				aDisplayBoardArray[i][j*2]=play1Pic[0];
				aDisplayBoardArray[i][j*2+1]=play1Pic[1];
			}
			else if (aRecordBoard[i][j]==2){
				aDisplayBoardArray[i][j*2]=play2Pic[0];
				aDisplayBoardArray[i][j*2+1]=play2Pic[1];
			}
			else if (aRecordBoard[i][j]==3){
				aDisplayBoardArray[i][j*2]=play1CurrentPic[0];
				aDisplayBoardArray[i][j*2+1]=play1CurrentPic[1];
			}
			else if (aRecordBoard[i][j]==4){
				aDisplayBoardArray[i][j*2]=play2CurrentPic[0];
				aDisplayBoardArray[i][j*2+1]=play2CurrentPic[1];
			}
		}
	}
	
//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
}


//显示棋盘格局 
void displayBoard(void){
	int i;
	//第一步：清屏
	system("clear");   //清屏  
	//第二步：将aDisplayBoardArray输出到屏幕上
	for (i=0;i<SIZE;i++)
		printf("%3d %s\n",SIZE-i,aDisplayBoardArray[i]);
	
	//第三步：输出最下面的一行字母A B .... 
	printf("   ");
	for (i=0;i<SIZE;i++){
		printf("%2c",'A'+i);
	}
	printf("\n");
} 

int main(){
	int mode;
	int another;
	another=1;
	printf("Welcome to the 5-in-row chess game!\nPlease choose the mode:1.p v p mode; 2. p v AI mode:\n");
	scanf("%d",&mode);
	if (mode == 1)
		pvp();
	if (mode ==2)
		pvc();
	return 0;
}


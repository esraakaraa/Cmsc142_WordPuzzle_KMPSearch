/*===================================================================
							Cmsc 142
							Final Output

VELASCO, Gimel David F.
MENESES, Juancho Rodrigo
=====================================================================
						WORD FINDING PUZZLE
=====================================================================
Input:	Word Puzzle of size RxC from wordpuzzle.txt file
		Word Patterns that will be searched from patterns.txt file
Output: Row and Column and Direction of Word found in the Word Puzzle
		Elapsed time in milliseconds
=====================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <windows.h>
#define TRIALS 1000
#define maxSTR 100
#define maxSRCH 100
#define maxRES 100

struct node{
	int row,col; //row and column
	char letter; //letter on a given cell
	struct node *neighbors[8]; //the 8 neighbors (N,NE,E,SE,S,SW,W,NW)
	struct node *templink; //a linear link just for conveniece of accessing all the tiles
};

void insert_templink(struct node *e,struct node **h);

void print_templink(struct node **h);

void link_neighbors(struct node **h, int dimROW, int dimCOL);

struct node * search_rowcol(struct node **h, int i, int j);

void start_bf(struct node **h, char pattern[maxSTR]);

int start_kmp(struct node **h, char pattern[maxSTR]);

struct node * bruteforce_stringsrch(struct node *p, int drc, char T[maxSTR], char P[maxSTR]);

struct node * knuthmorrispratt_stringsrch(struct node *p, int drc, char T[maxSTR], char P[maxSTR], int F[maxSTR]);

int main(){
	int i,j,k;//loops
	int r,c;//brd navigator
	int ctr,ctrROW,ctrCOL;//counters
	int dimROW, dimCOL;//holds the size of the Word Puzzle
	int srch,num_patterns;//hold the number of all the patterns
	int trial;//trial iterator
	char patterns[maxSRCH][maxSTR];//holds all the patterns
	int chc;
	/////////////////////////////////////////////////////////////////
	FILE *strm = NULL;
	FILE *strm2 = NULL;
	FILE *strm3 = NULL;
	FILE *strm4 = NULL;
	char cchar,nchar,dmp;
	strm = fopen("wordpuzzle.txt","r");
	strm2 = fopen("wordpuzzle.txt","r");
	strm3 = fopen("wordpuzzle.txt","r");
	strm4 = fopen("patterns.txt","r");
	/////////////////////////////////////////////////////////////////
	printf("\t\t\tWORD FINDING PUZZLE\n\n");
	/////////////////////////////////////////////////////////////////
	if(strm==NULL || strm4==NULL){
		printf("One of more files are not found.\n");
		sleep(1);
		printf("Program Terminated.");
		return 0;
	}
	else{
		/////////////Getting the Size of the Word Puzzle/////////////////
		ctrCOL = 0;
		ctrROW = 0;
		do{
			cchar = fgetc(strm2);
			if(cchar == '\n' || cchar == EOF){//another loop condition
				break;
			}
			if(cchar == ','){
				//skip
			}
			else{
				ctrCOL++;
			}
		}while(cchar!='\n');
		fclose(strm2);
		do{
			cchar = fgetc(strm3);
			if(cchar == '\n' || cchar == EOF){//another loop condition
				ctrROW++;
			}
		}while(cchar!=EOF);
		fclose(strm3);
		dimCOL = ctrCOL;
		dimROW = ctrROW;
		//////////////Getting the patterns from patterns.txt////////////
		num_patterns=0;
		do{
			ctr=0;
			do{
				cchar = fgetc(strm4);
				if(cchar == '\n' || cchar == EOF){
					break;
				}
				else{
					patterns[num_patterns][ctr] = cchar;
					ctr++;	
				}
			}while(cchar!='\n');
			num_patterns++;
		}while(cchar!=EOF);
		fclose(strm4);
		///////////////////Initializing Data Structure///////////////////
		struct node **head = (struct node **) malloc(sizeof(struct node *));
		*head = NULL;
		struct node *temp = (struct node *) malloc(sizeof(struct node));

		for(i=0;i<dimROW;i++){
			for(j=0;j<dimCOL;j++){
				temp = (struct node *) malloc(sizeof(struct node));
				temp->row = i;
				temp->col = j;
				temp->letter = ' ';
				for(k=0;k<8;k++){
					temp->neighbors[k] = NULL;
				}
				temp->templink = NULL;
				insert_templink(temp,head);
			}
		}
		link_neighbors(head,dimROW,dimCOL);
		////////////////////PARSING SECTION////////////////////////////
		struct node *p = *head;
		for(i=0;i<dimROW;i++){
			for(j=0;j<dimCOL;j++){
				do{
					cchar = fgetc(strm);
					if(cchar == ',' || cchar == '\n' || cchar == EOF){
						//skip
					}
					else{
						p->letter = cchar;
						p = p->templink;
					}
					if(cchar == '\n' || cchar == EOF){//another loop condition
						break;
					}
				}while(cchar!=',');
			}
		}
		fclose(strm);
		/////////////////////////////////////////////////////////////////
		//print_templink(head); //for testing
		//return 0;
		/////////////////////////////////////////////////////////////////
		printf("[1] Brute-Force Search\n[2] KMP Search\n\nInput Mode of Search: ");
		scanf("%d",&chc);
		if(chc==1){
			printf("\n\tBrute-Force Search\n");
		}
		else if(chc==2){
			printf("\n\tKMP Search\n");
		}
		//char dmp[maxSTR];
		//printf("Press Enter to start searching...");
		//gets(dmp);
		////////////////////////MAIN PROGRAM/////////////////////////////
		struct timeb start, end;
		int diff,totaltime=0,diffFF,totaltimeFF=0;
		for(srch=0;srch<num_patterns;srch++){
			for(trial=0;trial<TRIALS;trial++){
				ftime(&start);
				switch(chc){
					case 1:
						start_bf(head,patterns[srch]);
						break;
					case 2:
						diffFF = start_kmp(head,patterns[srch]);
						totaltimeFF += diffFF;
						break;
					default:
						printf("\nInvalid Input. ");
						sleep(1);
						printf("Program Terminated.\n");
						sleep(1);
						return 0;
				}
				ftime(&end);
				diff = (int)(1000*(end.time-start.time)+(end.millitm-start.millitm));
				totaltime += diff;
			}
		}
		printf("After %d Trials and %d Patterns searched in a %dx%d Word Puzzle,",TRIALS,num_patterns,dimROW,dimCOL);
		printf("\nTotal Elapsed time is %u ms\n", totaltime);
		if(chc==2){printf("Total Main KMP Function time: %u ms\nTotal Failure Function time: %u ms\n", totaltimeFF,totaltime-totaltimeFF);}
	}
	return 0;
}

void insert_templink(struct node *e,struct node **h){
	if(*h == NULL){//if empty
		*h = e;
	}
	else{
		struct node *p = *h;
		while(p->templink != NULL){
			p = p->templink;
		}
		e->templink = p->templink;
		p->templink = e;
	}
}

void print_templink(struct node **h){
	struct node *p = *h;
	int drc,i;
	if(p == NULL){
		printf("No elements in list.\n");
	}
	else{
		for(i=0;i<122;i++){printf("=");}printf("\n");
		printf("\t\t\t\t\tNoDE LiST\n");
		for(i=0;i<122;i++){printf("=");}printf("\n");
		while(p != NULL){
			printf("| RC: %d,%d | L: '%c' | Next: %p | Neighbors: ",p->row, p->col, p->letter, p->templink);
			for(drc=0;drc<8;drc++){
				printf("%p ",p->neighbors[drc]);
			}
			printf("|\n");
			p = p->templink;
		}
		for(i=0;i<122;i++){printf("=");}printf("\n");
	}
}

void link_neighbors(struct node **h, int dimROW, int dimCOL){
	int i,j,k;
	struct node *p = *h;
	struct node *t;
	int row_inc[8] = {-1,-1,0,1,1,1,0,-1};
	int col_inc[8] = {0,1,1,1,0,-1,-1,-1};
	int rii,cii,rio,cio;
	while(p != NULL){
		//printf("%d %d | ",p->row,p->col);
		for(i=0;i<8;i++){
			if(i%2 == 0){
				rii = p->row+row_inc[i];
				cii = p->col+col_inc[i];
				rio = (p->row+(dimROW+row_inc[i]))%dimROW;
				cio = (p->col+(dimCOL+col_inc[i]))%dimCOL;
				if((rii >= 0 && rii < dimROW) && (cii >= 0 && cii < dimCOL)){//if within the left and right and within top and bottom
					p->neighbors[i] = search_rowcol(h,rii,cii); //links adjacent
					//printf("%d%dii.",rii,cii);
				}
				else{
					if((rii < 0 || rii >= dimROW) && (cii >= 0 && cii < dimCOL)){//if within left and right <=> outside top and bottom
						p->neighbors[i] = search_rowcol(h,rio,cii);
						//printf("%d%doi.",rio,cii);
					}
					else if((rii >= 0 && rii < dimROW) && (cii < 0 || cii >= dimCOL)){//if within top and bottom <=> outside left and right
						p->neighbors[i] = search_rowcol(h,rii,cio);
						//printf("%d%dio.",rii,cio);
					}
					else{//if outside everything
						p->neighbors[i] = search_rowcol(h,rio,cio); //links loops
						//printf("%d%doo.",rio,cio);
					}
				}
			}
			else{
				rii = p->row+row_inc[i];
				cii = p->col+col_inc[i];
				if((rii >= 0 && rii < dimROW) && (cii >= 0 && cii < dimCOL)){//if within the left and right and within top and bottom
					p->neighbors[i] = search_rowcol(h,rii,cii); //links adjacent
					//printf("%d%dii.",rii,cii);
				}
				else{
					if(i==1 || i==5){
						//flip R,C
						rio = p->col;
						cio = p->row;
						p->neighbors[i] = search_rowcol(h,rio,cio);
						//printf("++++.");
					}
					else{//i==3 || i==7
						//flip DIM-R,DIM-C
						rio = (dimCOL-(p->col))-1;
						cio = (dimROW-(p->row))-1;
						p->neighbors[i] = search_rowcol(h,rio,cio);
						//printf("----.");
					}
				}
			}
		}
		//printf("\n");
		//printf("%d ",k);
		p = p->templink;
	}
}

struct node * search_rowcol(struct node **h, int i, int j){
	struct node *p = *h;
	while(p != NULL){
		if((p->row == i) && (p->col == j)){
			return p;
		}
		p = p->templink;
	}
	return NULL;
}

void start_bf(struct node **h, char pattern[maxSTR]){
	struct node *p = *h;
	struct node *match;
	char text[maxSTR];
	int drc,n,rctr=0,i,cnd;
	struct node *results[maxRES];
	for(i=0;i<maxRES;i++){results[i]==NULL;}
	char direc[8][3];
	strcpy(direc[0],"N");
	strcpy(direc[1],"NE");
	strcpy(direc[2],"E");
	strcpy(direc[3],"SE");
	strcpy(direc[4],"S");
	strcpy(direc[5],"SW");
	strcpy(direc[6],"W");
	strcpy(direc[7],"NW");
	//////////////////////////////////////////////////////////////
	while(p != NULL){
		struct node *q = p;
		for(drc=0;drc<8;drc++){
			n=0;
			do{
				//append q->letter to srchPString
				text[n] = q->letter;
				n++;
				q = q->neighbors[drc];
			}while(q != p);
			text[n] = '\0';
			///////////////////////BRUTEFORCE/////////////////////////////
			match = bruteforce_stringsrch(p,drc,text,pattern);
			//////////////////////////////////////////////////////////////
			cnd = 0;//not yet in results
			if(match != NULL){
				for(i=0;i<maxRES;i++){
					if(results[i]==NULL){break;}
					if(results[i]==match){cnd = 1;}//already exists in results
				}
				if(cnd==0){
					results[rctr] = match;
					printf("FOUND '%s' in RC: %d %d | D: %s\n",pattern,match->row,match->col,direc[drc]);
					rctr++;
				}
			}
		}
		p = p->templink;
		//printf("\n");
	}
}

int start_kmp(struct node **h, char pattern[maxSTR]){
	struct node *p = *h;
	struct node *match;
	char text[maxSTR];
	int drc,n,m,rctr=0,cnd;
	int F[maxSTR];
	int i,j,k;
	struct node *results[maxRES];
	char direc[8][3];
	strcpy(direc[0],"N");
	strcpy(direc[1],"NE");
	strcpy(direc[2],"E");
	strcpy(direc[3],"SE");
	strcpy(direc[4],"S");
	strcpy(direc[5],"SW");
	strcpy(direc[6],"W");
	strcpy(direc[7],"NW");
	//////////////////////////////////////////////////////////////
	/////////////////////KMP Failure Function/////////////////////
	F[0] = 0;
	i = 1;
	j = 0;
	m = strlen(pattern);
	while(i < m){
		if(pattern[i] == pattern[j]){
			F[i] = j+1;
			i++;
			j++;
		}
		else if(j > 0){
			j = F[j-1];
		}
		else{
			F[i] = 0;
			i++;
		}
	}
	/*//testing Failure Function
	printf("\nFailureFunction: |");
	for(i=0;i<m;i++){
		printf("%d | ",F[i]);
	}
	printf("\n");
	*/
	//////////////////////////////////////////////////////////////
	struct timeb startFF, endFF;
	int diffFF;
	ftime(&startFF);
	while(p != NULL){
		struct node *q = p;
		for(drc=0;drc<8;drc++){
			n=0;
			do{
				//append q->letter to srchPString
				text[n] = q->letter;
				n++;
				q = q->neighbors[drc];
			}while(q != p);
			text[n] = '\0';
			////////////////////////KMP////////////////////////////////////
			match = knuthmorrispratt_stringsrch(p,drc,text,pattern,F);
			//////////////////////////////////////////////////////////////
			cnd = 0;//not yet in results
			if(match != NULL){
				for(k=0;k<maxRES;k++){
					if(results[k]==NULL){break;}
					if(results[k]==match){cnd = 1;}//already exists in results
				}
				if(cnd==0){
					results[rctr] = match;
					printf("FOUND '%s' in RC: %d,%d | D: %s\n",pattern,match->row,match->col,direc[drc]);
					rctr++;
				}
			}
		}
		p = p->templink;
		//printf("\n");
	}
	ftime(&endFF);
	diffFF = (int)(1000*(endFF.time-startFF.time)+(endFF.millitm-startFF.millitm));
	return diffFF;
}

struct node * bruteforce_stringsrch(struct node *p, int drc, char T[maxSTR], char P[maxSTR]){
	//printf("||| Text: %s | Pattern: %s ",T,P);
	//perform bf search
	//////////////The following block of code is based on the Cmsc142 String Matching slides//////////////
	struct node *q = p;
	int m,n,nm,i,j;
	n = strlen(T);
	m = strlen(P);
	if(m <= n){//double checks if pattern is larger than the text
		nm = (n-m) + 1;
		for(i=0;i<nm;i++){
			j = 0;
			while((j < m)&&(T[i+j] == P[j])){//char match
				j++;
				if(j == m){//match
					return q; //return i; //match
				}//else, no match
			}
			q = q->neighbors[drc]; //this used to return the first node of match
		}
	}
	return NULL;
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

struct node * knuthmorrispratt_stringsrch(struct node *p, int drc, char T[maxSTR], char P[maxSTR], int F[maxSTR]){
	//printf("||| Text: %s | Pattern: %s ",T,P);
	//perform kmp search
	//////////////The following block of code is based on the Cmsc142 String Matching slides//////////////
	struct node *q = p;//node 'q' is used to track the first node of the matched subtext and pattern
	int m,n,i,j,k,rdrc;
	n = strlen(T);
	m = strlen(P);
	rdrc = (drc+4)%8;//reverse direction used to return the found node
	///////////////Main KMP Function/////////////////
	i = 0;
	j = 0;
	while(i < n){
		if(T[i] == P[j]){
			if(j == m-1){
				for(k=0;k<j;k++){//this for loop moves node 'q' back in 'j' number of steps
					q = q->neighbors[rdrc]; //return i-j; //match
				}
				return q;
			}
			else{
				i++;
				j++;
				q = q->neighbors[drc];//node 'q' moves with the position of 'i'
			}
		}
		else{
			if(j > 0){
				j = F[j-1];
			}
			else{
				i++;
				q = q->neighbors[drc];//node 'q' moves with the position of 'i'
			}
		}
	}
	return NULL;//no match found
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}

/*
//////////////General Pseudocode////////////////

meow
*/
		//Parsing Pseudocode
			//Get dimensions of wordpuzzle.txt into dimROW and dimCOL
			//initialize data structure size using dimROW and dimCOL
			//import the char from wordpuzzle.txt to data structure
		///////////////////////////////////////////////////////////////
///////////////////////PSEUDOCODE/////////////////////////////
	//start with *head
	//search all texts in directions starting N,NE,E,...,W,NW
	////brute force search
	////kmp algorithm search
	////if match is found, print Row#, Col# and Direction of node of first character of pattern
	//move to templink until null
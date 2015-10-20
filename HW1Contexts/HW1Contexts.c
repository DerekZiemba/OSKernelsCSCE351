#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

#define STACKSIZE  4096

ucontext_t GlobalBuffer;
int i;

void myinsert(void);
void mydelete(void);
void runHW1Contexts(void);

void myinsert(void) {
	printf("Insert an item\n");
	i++;
	printf("Done inserting! i = %d\n", i);
	return;
}

void mydelete(void) { //Renamed because I was having some kind of duplicate name issue. 
	printf("Delete an item\n");
	i--;
	printf("Done deleting! i = %d\n", i);
	return;
}

void runHW1Contexts(void) {
	char InsertStack[STACKSIZE], DeleteStack[STACKSIZE];
	ucontext_t InsertBuffer, DeleteBuffer;
	
	getcontext(&InsertBuffer);
	getcontext(&DeleteBuffer);
	getcontext(&GlobalBuffer);
	
	// set "insert" thread's context below	
	InsertBuffer.uc_link          = &GlobalBuffer;
	InsertBuffer.uc_stack.ss_sp   = InsertStack;
	InsertBuffer.uc_stack.ss_size = sizeof InsertStack;
	makecontext(&InsertBuffer, myinsert, 0);
	int i = 0;
	int x = i + 4;
	// set "delete" thread's context below			
	DeleteBuffer.uc_link          = &GlobalBuffer;
	DeleteBuffer.uc_stack.ss_sp   = DeleteStack;
	DeleteBuffer.uc_stack.ss_size = sizeof DeleteStack;
	makecontext(&DeleteBuffer, mydelete, 0);	
		
	printf("Main: Jumping to insert\n");
	// swap context of main and insert
	swapcontext(&GlobalBuffer, &InsertBuffer);
		
	printf("Main: Jumping to delete\n");
	// swap context of insert and delete  
	swapcontext(&GlobalBuffer, &DeleteBuffer);
	
	printf("Main thread --- all done\n");	
}

int main(int argc, char *argv[]) {
	runHW1Contexts();
	exit(0);
}
// Author: Shikang Xu; ECE 353 TA

// List the full names of ALL group members at the top of your code.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
//feel free to add here any additional library names you may need 
#define SINGLE 1
#define BATCH 0
#define REG_NUM 32

struct latch{
	int valid; 	//indicates whether the latch is currently being read by the phase after it
	int opcode;	//indicates the opcode of operation
	int dest; 	//indicates the destination register of an operation using writeback
	int data1;	//used for various storage of different data
	int data2;
	int data3;
};

struct inst{
	int opcode;
	int rd;
	int rs;
	int rt;
	int imm;
};

//Global Variables
//Initialize Latches
struct latch IF_ID = {0};
struct latch ID_EX = {0};
struct latch EX_MEM = {0};
struct latch MEM_WB = {0};
//initialize instruction memory and data memory
int32_t data_mem[512];
struct inst inst_mem[512];


char *progScanner(char *instr_str){
	char delimiters[] = " ,()\n";
	char *scanned = (char*)malloc(100*sizeof(char));
	char* token;

	//following loop and if statement checks for mismatched parenthesis
	int count =0;
	for(int i=0; i<strlen(instr_str); i++){
		if (instr_str[i] == '(') count++;
		else if(instr_str[i] == ')') count--;
		if (count<0){ 
			printf("error: %s is an invalid instruction\n", instr_str);
			exit(1);
		}
	}
	if (count != 0){ 
		printf("error: %s is an invalid instruction\n", instr_str);
		exit(1);
	}

	token = strtok(instr_str, delimiters);
	
	while(token != NULL){
		strcat(scanned, token);
		strcat(scanned, " ");
		token = strtok(NULL, delimiters);
	}
	return scanned;
}

char *regNumberConverter(char *instr_str){
char delimiter[] = " $"; 
  char *converted = (char*)malloc(100*sizeof(char)); 
  char* token; 
  char temp; 
 
  token = strtok(instr_str, delimiter); 
  while(token != NULL){ 
    if(!strcmp(token, "zero")) strcat(converted, "0"); 
    else if(!strcmp(token, "at")) strcat(converted, "1"); 
    else if(!strcmp(token, "v0")) strcat(converted, "2"); 
    else if(!strcmp(token, "v1")) strcat(converted, "3"); 
    else if(!strcmp(token, "a0")) strcat(converted, "4"); 
    else if(!strcmp(token, "a1")) strcat(converted, "5"); 
    else if(!strcmp(token, "a2")) strcat(converted, "6"); 
    else if(!strcmp(token, "a3")) strcat(converted, "7"); 
    else if(!strcmp(token, "t0")) strcat(converted, "8"); 
    else if(!strcmp(token, "t1")) strcat(converted, "9"); 
    else if(!strcmp(token, "t2")) strcat(converted, "10"); 
    else if(!strcmp(token, "t3")) strcat(converted, "11"); 
    else if(!strcmp(token, "t4")) strcat(converted, "12"); 
    else if(!strcmp(token, "t5")) strcat(converted, "13"); 
    else if(!strcmp(token, "t6")) strcat(converted, "14"); 
    else if(!strcmp(token, "t7")) strcat(converted, "15"); 
    else if(!strcmp(token, "s0")) strcat(converted, "16"); 
    else if(!strcmp(token, "s1")) strcat(converted, "17"); 
    else if(!strcmp(token, "s2")) strcat(converted, "18"); 
    else if(!strcmp(token, "s3")) strcat(converted, "19"); 
    else if(!strcmp(token, "s4")) strcat(converted, "20"); 
    else if(!strcmp(token, "s5")) strcat(converted, "21"); 
    else if(!strcmp(token, "s6")) strcat(converted, "22"); 
    else if(!strcmp(token, "s7")) strcat(converted, "23"); 
    else if(!strcmp(token, "t8")) strcat(converted, "24"); 
    else if(!strcmp(token, "t9")) strcat(converted, "25"); 
    else if(!strcmp(token, "k0")) strcat(converted, "26"); 
    else if(!strcmp(token, "k1")) strcat(converted, "27"); 
    else if(!strcmp(token, "gp")) strcat(converted, "28");
    else if(!strcmp(token, "sp")) strcat(converted, "29"); 
    else if(!strcmp(token, "fp")) strcat(converted, "30"); 
    else if(!strcmp(token, "ra")) strcat(converted, "31");
    
    else strcat(converted, token); 
    strcat(converted, " "); 
    token = strtok(NULL, delimiter); 
  } 
  printf("%s\n", converted); 
  return converted; 
}

void parser(char *instr_str){

}

void IF(){
/*Fetches the instruction struct form memory
  Checks if valid bit in IF_ID latch is 1
  Writes istr.opcode into latch.opcode 
  Writes istr.rd into latch.dest 
  Writes istr.rs into latch.data1
  Writes istr.rt into latch.data2
  Writes istr.imm into latch.data3i
  Sets valid bit to 0	*/
  
}

void ID(long mips_reg[]){
/*	Checks if valid bit in IF_ID = 0
	Writes IF_ID.opcode ID_EX.opcode
	If add, sub, mul
	*********************************
	Writes IF_ID.dest into ID_EX.dest
	Writes mips_reg[IF_ID.data1] into ID_EX.data1
	Writes mips_reg[IF_ID.data2] into ID_EX.data2

	Else if addi, lw, sw
	*********************************
	Writes IF_ID.data2 into ID_EX.dest
	Writes iF_ID.data1 into ID_EX.data1
	Writes IF_ID.data3 into ID_EX.data2

	Else beq...
 */
}

void EX(){

}

void MEM(){

}

void WB(){
	
}


main (int argc, char *argv[]){
	//Beginning of code1.c
	int sim_mode=0;//mode flag, 1 for single-cycle, 0 for batch
	int c,m,n;
	int i;//for loop counter
	long mips_reg[REG_NUM];
	long pgm_c=0;//program counter
	long sim_cycle=0;//simulation cycle counter
	//define your own counter for the usage of each pipeline stage here
	static int IF_counter;
	static int ID_counter;
	static int EX_counter;
	static int MEM_counter;
	static int WB_counter;
	
	int test_counter=0;
	FILE *input=NULL;
	FILE *output=NULL;
	printf("The arguments are:");
	
	for(i=1;i<argc;i++){
		printf("%s ",argv[i]);
	}
	printf("\n");
	//if there is the correct amount of arguements
	if(argc==7){
		if(strcmp("-s",argv[1])==0){
			sim_mode=SINGLE;
		}
		else if(strcmp("-b",argv[1])==0){
			sim_mode=BATCH;
		}
		else{
			printf("Wrong sim mode chosen\n");
			exit(0);
		}
		
		m=atoi(argv[2]);
		n=atoi(argv[3]);
		c=atoi(argv[4]);
		input=fopen(argv[5],"r");
		output=fopen(argv[6],"w");

		
	}
	//if the command line input was incorrect, print out the correct way to write it
	else{
		printf("Usage: ./sim-mips -s m n c input_name output_name (single-sysle mode)\n or \n ./sim-mips -b m n c input_name  output_name(batch mode)\n");
		printf("m,n,c stand for number of cycles needed by multiplication, other operation, and memory access, respectively\n");
		exit(0);
	}
	//print explanations if input or output file could not be created or opened`
	if(input==NULL){
		printf("Unable to open input or output file\n");
		exit(0);
	}
	if(output==NULL){
		printf("Cannot create output file\n");
		exit(0);
	}
	//Initalize registers and program counter
	if(sim_mode==1){
		for (i=0;i<REG_NUM;i++){
			mips_reg[i]=0;
		}
	}
	/////////////////////////////////////////////////End of code1.c

	//Start your code here

	char *instr_str;
	instr_str = malloc(100*sizeof(char));

	while(fgets(instr_str, 100, input))
		parser(regNumberConverter(progScanner(instr_str)));
	fclose(input);
	

	/////////////////////////////////////////////////code2.c: The following code will output the register calue to 
	//screen at every cycle and wait for the ENTER key to be pressed;
	//this will make it proceed to the next cycle
	printf("cycle: %d ",sim_cycle);
	if(sim_mode==1){
		for(i=1;i<REG_NUM;i++){
			printf("%d  ",mips_reg[i]);
		}
	}
	printf("%d\n",pgm_c);
	pgm_c+=4;
	sim_cycle+=1;
	test_counter++;
	printf("press ENTER to continue\n");
	while(getchar() != '\n');
	//end of code2.c









	float ifUtil, idUtil, exUtil, memUtil, wbUtil = 0;
	//Beginning of code3.c, code given to us to be put at the end of main
	if(sim_mode==0){
		fprintf(output,"program name: %s\n",argv[5]);
		fprintf(output,"stage utilization: %f  %f  %f  %f  %f \n", ifUtil, idUtil, exUtil, memUtil, wbUtil);
                     // add the (double) stage_counter/sim_cycle for each 
                     // stage following sequence IF ID EX MEM WB
		
		fprintf(output,"register values ");
		for (i=1;i<REG_NUM;i++){
			fprintf(output,"%d  ",mips_reg[i]);
		}
		fprintf(output,"%d\n",pgm_c);
	
	}
	//close input and output files at the end of the simulation
	fclose(input);
	fclose(output);
	return 0;
	//End of code3.c

	//Jeff muri likes spicy asian food
	//Jackson like whaners
	//start your code from here
	//balhbalhaskdlfjaslkdfh
	//No dad what about you!!!
	//john shea likes boners


}

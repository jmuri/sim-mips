//John Shea, Jeff Muri, Jackson Perkins

// List the full names of ALL group members at the top of your code.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <ctype.h>
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
	int immediate;
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
int nopCounter = 0;//current total of nops queued to be executed
int nopTotal = 0; //total nops exectued in the program
int macces_val; //total cycles for a mem access
int excecution_val; //total cycles for execution 
int mult_val; //total cycles for multiplication

float ifUtil, idUtil, exUtil, memUtil, wbUtil = 0;

char *mycat(char *cur, char *next){
	char *string = malloc(strlen(cur) + strlen(next) + 1);
	char *res = string;
	for (; *cur ; *string++ = *cur++);
	for (; *next ; *string++ = *next++);
	//*string = '\0';
	return res; 
}

int hasletter(char *token){
	int i;
	for(i = 0; i<strlen(token);i++){
		if(token[i] == '-' && i == 0) i++;
		if(isdigit(token[i])==0) return 0;
	}
	return 1;
}

int isdecimal(char *token){
	int i;
	for(i = 0; i<strlen(token);i++){
		if(strcmp(&token[i], ".")==0) return 0;
	}
	return 1;
}

char *progScanner(char *instr_str){
	char delimiters[] = " ,()\n";
	char *scanned = (char*)malloc(100*sizeof(char));
	char* token;
	int sw_lw = 0;
	int addi_beq = 0;
	int token_cnt = 0;

	//following loop and if statement checks for mismatched parenthesis
	int count =0;
	int i;
	for(i=0; i<strlen(instr_str); i++){
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
	//builds instruction without parenthesis or commas
	while(token != NULL){
		if(!strcmp(token, "lw") || !strcmp(token, "sw")) sw_lw = 1;
  		if(!strcmp(token, "addi") || !strcmp(token, "beq")) addi_beq = 1;
  		if(((sw_lw && token_cnt != 2) || (addi_beq && token_cnt < 3) || (!sw_lw && !addi_beq)) 
  			&& token[0] != '$' && token_cnt != 0 ){
  			printf("error: %s is not a register\n", token);
  			exit(1);
  		}
		scanned = mycat(scanned, token);
		scanned = mycat(scanned, " ");
		token = strtok(NULL, delimiters);
		token_cnt++;
	}
	//printf("%s\n", scanned);
	return scanned;
}

char *regNumberConverter(char *instr_str){
  char delimiter[] = " $"; 
  char *converted = (char*)malloc(100*sizeof(char)); 
  char* token; 
  int token_cnt = 0;
  int sw_lw = 0;
  int addi_beq = 0;
  token = strtok(instr_str, delimiter);
  if(!strcmp(token, "lw") || !strcmp(token, "sw")) sw_lw = 1;
  if(!strcmp(token, "addi") || !strcmp(token, "beq")) addi_beq = 1;
  converted = mycat(converted, token);
  converted = mycat(converted, " ");
  token = strtok(NULL, delimiter);

  while(token != NULL){
    if(!strcmp(token, "lw") || !strcmp(token, "sw")) sw_lw = 1;
    if(!strcmp(token, "addi") || !strcmp(token, "beq")) addi_beq = 1;
    if(!strcmp(token, "zero")) converted = mycat(converted, "0"); 
    else if(!strcmp(token, "at")) converted = mycat(converted, "1"); 
    else if(!strcmp(token, "v0")) converted = mycat(converted, "2"); 
    else if(!strcmp(token, "v1")) converted = mycat(converted, "3"); 
    else if(!strcmp(token, "a0")) converted = mycat(converted, "4"); 
    else if(!strcmp(token, "a1")) converted = mycat(converted, "5"); 
    else if(!strcmp(token, "a2")) converted = mycat(converted, "6"); 
    else if(!strcmp(token, "a3")) converted = mycat(converted, "7"); 
    else if(!strcmp(token, "t0")) converted = mycat(converted, "8"); 
    else if(!strcmp(token, "t1")) converted = mycat(converted, "9"); 
    else if(!strcmp(token, "t2")) converted = mycat(converted, "10"); 
    else if(!strcmp(token, "t3")) converted = mycat(converted, "11"); 
    else if(!strcmp(token, "t4")) converted = mycat(converted, "12"); 
    else if(!strcmp(token, "t5")) converted = mycat(converted, "13"); 
    else if(!strcmp(token, "t6")) converted = mycat(converted, "14"); 
    else if(!strcmp(token, "t7")) converted = mycat(converted, "15"); 
    else if(!strcmp(token, "s0")) converted = mycat(converted, "16"); 
    else if(!strcmp(token, "s1")) converted = mycat(converted, "17"); 
    else if(!strcmp(token, "s2")) converted = mycat(converted, "18"); 
    else if(!strcmp(token, "s3")) converted = mycat(converted, "19"); 
    else if(!strcmp(token, "s4")) converted = mycat(converted, "20"); 
    else if(!strcmp(token, "s5")) converted = mycat(converted, "21"); 
    else if(!strcmp(token, "s6")) converted = mycat(converted, "22"); 
    else if(!strcmp(token, "s7")) converted = mycat(converted, "23"); 
    else if(!strcmp(token, "t8")) converted = mycat(converted, "24"); 
    else if(!strcmp(token, "t9")) converted = mycat(converted, "25"); 
    else if(!strcmp(token, "k0")) converted = mycat(converted, "26"); 
    else if(!strcmp(token, "k1")) converted = mycat(converted, "27"); 
    else if(!strcmp(token, "gp")) converted = mycat(converted, "28");
    else if(!strcmp(token, "sp")) converted = mycat(converted, "29"); 
    else if(!strcmp(token, "fp")) converted = mycat(converted, "30"); 
    else if(!strcmp(token, "ra")) converted = mycat(converted, "31");
    //following statements check for invalid register values
    else if(!isdigit(token[1]) && strcmp(token, "sp") && strcmp(token, "fp") && token[1] != NULL &&
    	strcmp(token, "ra") && strcmp(token, "zero") ) return NULL;
    else if(!(atoi(token) >= 0 && atoi(token) <= 31) && (token_cnt != 2) && (addi_beq==1)) return NULL;
    else if(!(atoi(token) >= 0 && atoi(token) <= 31) && (token_cnt != 1) && (sw_lw==1)) return NULL;
    else if((atoi(token) < 0 || atoi(token) > 31) && (sw_lw == 0) && (addi_beq == 0)) return NULL;
    else if(isdecimal(token)==0) return NULL;
    else if(hasletter(token)==0) return NULL;
    else converted = mycat(converted, token); 
    converted = mycat(converted, " "); 
    token = strtok(NULL, delimiter);
    token_cnt++; 
  } 
  return converted; 
}

struct inst parser(char *instr_str){
	//No idea why they suggest to use enum here, we're passed character array, not literals.
	char *token;
	char *parsed = (char*)malloc(100*sizeof(char));
	char delimiter[] = " ";
	int i = 0;
	int r = 0;
	int sw_lw = 0;
	struct inst instruction;

	token = strtok(instr_str, delimiter);
	if(!strcmp(token, "add")){
		instruction.opcode = 0;
		token = strtok(NULL, delimiter);
		instruction.rd = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		instruction.immediate = 0;
	}
	else if(!strcmp(token, "addi")){
		instruction.opcode = 3;
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		token = strtok(NULL, delimiter);
		if(atoi(token) > 32767 || atoi(token) < -32768){
			printf("error: %s takes more than 16 bits\n", token); 		
			exit(1);
		}
		instruction.immediate = atoi(token);
		instruction.rd = 0;	
	}
	else if(!strcmp(token, "sub")){
		instruction.opcode = 1;
		token = strtok(NULL, delimiter);
		instruction.rd = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		instruction.immediate = 0;
	}
	else if(!strcmp(token, "mul")){
		instruction.opcode = 2;
		token = strtok(NULL, delimiter);
		instruction.rd = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		instruction.immediate = 0;
	}
	else if(!strcmp(token, "lw")){
		instruction.opcode = 4;
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		token = strtok(NULL, delimiter);
		if(atoi(token)%4 !=0 || atoi(token) > 32767 || atoi(token) < -32768){
			printf("error: %s is an illegal offset\n", token);
			exit(1);
		}
		instruction.immediate = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		instruction.rd = 0;
	}
	else if(!strcmp(token, "sw")){
		instruction.opcode = 5;
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		token = strtok(NULL, delimiter);
		if(atoi(token)%4 != 0 || atoi(token) > 32767 || atoi(token) < -32768){
			printf("error: %s is an illegal offset\n", token);
			exit(1);
		}
		instruction.immediate = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		instruction.rd = 0;
	}
	else if(!strcmp(token, "beq")){
		instruction.opcode = 6;
		token = strtok(NULL, delimiter);
		instruction.rs = atoi(token);
		token = strtok(NULL, delimiter);
		instruction.rt = atoi(token);
		token = strtok(NULL, delimiter);
		if(atoi(token)%4 != 0 || atoi(token) > 32767 || atoi(token) < -32768){
			printf("error: %s is an illegal offset\n", token);
			exit(1);
		}
		instruction.immediate = atoi(token);
		instruction.rd = 0;
	}
	else if(!strcmp(token, "haltSimulation")){
		instruction.opcode = -1;
		instruction.rd = 0;
		instruction.rs = 0;
		instruction.rt = 0;
		instruction.immediate = 0;
	}
	else{
		instruction.opcode = -2;
		instruction.rd = 0;
		instruction.rs = 0;
		instruction.rt = 0;
		instruction.immediate = 0;
	}
	//printf("%d %d %d %d %d %d\n", instruction.opcode, instruction.rd, instruction.rs, instruction.rt, instruction.immediate);
	return instruction;
	
}

void IF(int* IF_counter, struct inst inst_mem[], long* pgm_c){
/*	Fetches the instruction struct from memory
  	Writes istr.opcode into latch.opcode 
  	Writes istr.rd into latch.dest 
  	Writes istr.rs into latch.data1
  	Writes istr.rt into latch.data2
  	Writes istr.imm into latch.data3i
  	Sets valid bit to 0	*/
  	struct inst i = inst_mem[((*pgm_c)/4)];
  	
  	if((*IF_counter)>0){
		(*IF_counter)--;
		assert((*IF_counter)>=0);	
  	}

  	if((*IF_counter)==0 && IF_ID.valid==0){
  		if(nopCounter==0){
			IF_ID.opcode = i.opcode;
			IF_ID.dest = i.rd;
			IF_ID.data1 = i.rs;
			IF_ID.data2 = i.rt;
			IF_ID.data3 = i.immediate;
			(*pgm_c)+=4; 
			assert((*pgm_c)>=0); 
		}
		else{
			IF_ID.opcode = 0;
			IF_ID.dest = 0;
			IF_ID.data1 = 0;
			IF_ID.data2 = 0;
			IF_ID.data3 = 0;
			nopCounter--;
			assert(nopCounter>=0);
		}
		IF_ID.valid = 1;
		*IF_counter = macces_val;
		ifUtil++;
  	}
  	else{
	  	return;	
	}
}

void ID(int* ID_counter, long mips_reg[], long* pgm_c){
/*	Writes IF_ID.opcode ID_EX.opcode
	
	If add, sub, mul
	Writes IF_ID.dest into ID_EX.dest
	Writes mips_reg[IF_ID.data1] into ID_EX.data1
	Writes mips_reg[IF_ID.data2] into ID_EX.data2

	If addi, lw, sw
	Writes IF_ID.data2 into ID_EX.dest
	Writes mips[IF_ID.data1] into ID_EX.data1
	Writes IF_ID.data3 into ID_EX.data2

	If beq, increase nop counter

	/*To check for raw hazards, check that EX_MEM.dest != ID source reg,
	then enter two nops. If MEM_WB!= ID source reg, enter 1 nop*/

	if(IF_ID.valid==1 && (*ID_counter)>0){
			(*ID_counter)--;
			assert((*ID_counter)>=0);
	}
	if((*ID_counter)==0 && ID_EX.valid==0){
  		ID_EX.opcode = IF_ID.opcode;
		//Check to see if RAW hazard exists at EX_MEM, clear latch if exists
		if(((IF_ID.data1==EX_MEM.dest)|(IF_ID.data2==EX_MEM.dest))&&(EX_MEM.dest>0)){
			nopCounter++;
			nopTotal++;
			ID_EX.opcode = 0;
			ID_EX.dest = 0;
			ID_EX.data1 = 0;
			ID_EX.data2 = 0;
			ID_EX.data3 = 0;
			(*pgm_c)-=4;
			assert((*pgm_c)>=0);
		}
		//Check to see if RAW hazard exists at MEM_WB, clear latch if exists
		else if(((IF_ID.data1==MEM_WB.dest)|(IF_ID.data2==MEM_WB.dest))&&(MEM_WB.dest>0)){
			ID_EX.opcode = 0;
			ID_EX.dest = 0;
			ID_EX.data1 = 0;
			ID_EX.data2 = 0;
			ID_EX.data3 = 0;
		}
		//If no RAW hazards, continue through
		else{
			switch(ID_EX.opcode){
				case(0):
				case(1):
					ID_EX.dest = IF_ID.dest;
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = mips_reg[IF_ID.data2];
					break;
				case(2):
					ID_EX.dest = IF_ID.dest;
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = mips_reg[IF_ID.data2];
					ID_EX.data3 = 1;
					break;
				case(3):
					ID_EX.dest = IF_ID.data2;
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = IF_ID.data3;
					break;
				case(4):
					ID_EX.dest = IF_ID.data2;
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = IF_ID.data3;
					ID_EX.data3 = 2;
					break;
				case(5):
					ID_EX.dest = mips_reg[IF_ID.data2];
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = IF_ID.data3;
					ID_EX.data3 = 2;
					break;
				case(6):
					ID_EX.dest = IF_ID.data3;
					ID_EX.data1 = mips_reg[IF_ID.data1];
					ID_EX.data2 = mips_reg[IF_ID.data2];
					nopCounter++;
					nopTotal++;
					break;		
			}
		}
		IF_ID.valid=0;
		ID_EX.valid=1;
		*ID_counter=1;
		idUtil++;
  	}
  	else{
		return;
	}
}

void EX(int* EX_counter, long* pgm_c){
/*	Writes ID_EX.opcode into EX_MEM.opcode
	Writes ID_EX.dest into EX_MEM.dest
	Operates on ID_EX.data1 and ID_EX.data2 and puts into EX_MEM.data1
	depending on opcode */
	if(ID_EX.data3==1){
		(*EX_counter) = mult_val;
		ID_EX.data3 = 0;
	}
	if(ID_EX.valid==1 && (*EX_counter)>0){
			(*EX_counter)--;
			assert((*EX_counter)>=0);
	}
	if((*EX_counter)==0 && EX_MEM.valid==0){
  		EX_MEM.opcode = ID_EX.opcode;
		EX_MEM.dest = ID_EX.dest;
		EX_MEM.data3 = ID_EX.data3;
		switch(EX_MEM.opcode){
			case(0):
				EX_MEM.data1 = ID_EX.data1+ID_EX.data2;
				break;
			case(1):
				EX_MEM.data1 = (ID_EX.data1-ID_EX.data2);
				break;
			case(2):
				EX_MEM.data1 = ID_EX.data1*ID_EX.data2;
				break;
			case(3):
			case(4):
			case(5):
				EX_MEM.data1 = ID_EX.data1+ID_EX.data2;
				break;
			case(6):
				if((ID_EX.data1-ID_EX.data2)==0){
					(*pgm_c)+=EX_MEM.dest;
					assert((*pgm_c)>=0);
				}
				break;
			}
		ID_EX.valid=0;
		EX_MEM.valid=1;
		*EX_counter=excecution_val;	
		exUtil++;
  	}
  	else{
		return;
	}	
}

void MEM(int* MEM_counter){
/*	Writes EX_MEM.opcode into MEM_WB.opcode
	Writes EX_MEM.dest into MEM_WB.dest

	If add, sub, mul, addi, there is no memory access and takes one cycle
	Writes EX_MEM.data1 into MEM_WB.data1

	If lw
	Writes data_mem[EX_MEM.data1] into MEM_WB.data1

	IF sw
	Writes EX_MEM.dest into data_mem[EX_MEM.data1]*/

	if(EX_MEM.data3==2){
		(*MEM_counter) = macces_val;
		EX_MEM.data3 = 0;
	}

	if(EX_MEM.valid==1 && (*MEM_counter)>0){
			(*MEM_counter)--;
			assert((*MEM_counter)>=0);
	}
	if((*MEM_counter)==0 && MEM_WB.valid==0){
  		MEM_WB.opcode = EX_MEM.opcode;
		MEM_WB.dest = EX_MEM.dest;
		switch(MEM_WB.opcode){
			case(0):
			case(1):
			case(2):
			case(3):
				MEM_WB.data1 = EX_MEM.data1;
				break;
			case(4):
				MEM_WB.data1 = data_mem[EX_MEM.data1];
				break;
			case(5):
				data_mem[EX_MEM.data1] = EX_MEM.dest;
				MEM_WB.data1 = EX_MEM.data1;
				break;
			case(6):
				break;
		}
		EX_MEM.valid=0;
		MEM_WB.valid=1;
		*MEM_counter=1;
		memUtil++;
  	}
  	else{
		return;
	}
}

void WB(int* WB_counter, long mips_reg[]){
/* 	If add, sub, mult, addi, lw
	Writes MEM_WB.data1 into mips_reg[MEM_WB.dest]

	If sw
	Nothing gets written to the registers */

	if(MEM_WB.valid==1 && (*WB_counter)>0){
			(*WB_counter)--;
			assert((*WB_counter)>=0);
	}
	if((*WB_counter)==0){
  		if(MEM_WB.opcode<5){
			mips_reg[MEM_WB.dest] = MEM_WB.data1;
		}
		MEM_WB.valid=0;
		*WB_counter=1;
		wbUtil++;
  	}
  	else{
		return;
	}
}

void displayLatch(struct latch l){
	printf("\n");
	printf("=====\n");
	printf("%d\n",l.valid);
	printf("%d\n",l.opcode);
	printf("%d\n",l.dest);
	printf("%d\n",l.data1);
	printf("%d\n",l.data2);
	printf("%d\n",l.data3);
	printf("=====\n");
}



int main (int argc, char *argv[]){
	//Beginning of code1.c
	int sim_mode=0;//mode flag, 1 for single-cycle, 0 for batch
	int c,m,n;
	int i;//for loop counter
	long mips_reg[REG_NUM];
	long pgm_c=0;//program counter
	long sim_cycle=0;//simulation cycle counter
	//define your own counter for the usage of each pipeline stage here
	
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
	//if(sim_mode==1){
		for (i=0;i<REG_NUM;i++){
			mips_reg[i]=0;
		}
	//}
	/////////////////////////////////////////////////End of code1.c

	//Start your code here

	macces_val = c;
	excecution_val = n;
	mult_val = m;

	int IF_counter = macces_val+1;
	int ID_counter = 1;
	int EX_counter = excecution_val;
	int MEM_counter = 1;
	int WB_counter = 1;

	char *instr_str;
	char *valid;
	struct inst parsed_instruction;
	instr_str = malloc(100*sizeof(char));
	int inst_cnt = 0;
	while(fgets(instr_str, 100, input)){
         	valid = regNumberConverter(progScanner(instr_str));
		if(valid==NULL){
			printf("error: instruction %i contains an invalid register\n", inst_cnt);
			return 1;
		}
		parsed_instruction = parser(valid);
		if(parsed_instruction.opcode == -2){
			printf("error: instruction %i contains an invalid opcode\n", inst_cnt);
			return 1;
		}
		if(parsed_instruction.immediate==-1){
			printf("error: instruction %i contains an invalid offset value\n", inst_cnt);
			return 1;
		}
		inst_mem[inst_cnt] = parsed_instruction;
		//printf("op: %i rd: %i rs: %i rt: %i imm: %i\n", inst_mem[inst_cnt].opcode, inst_mem[inst_cnt].rd, inst_mem[inst_cnt].rs, inst_mem[inst_cnt].rt, inst_mem[inst_cnt].immediate);
		inst_cnt++;
	}

////////Jackson Testing/////////
	//Main loop will be while the command is not haltSimulation
	//Will then run out rest of pipeline then stop simulation?
	while(1){
		//always ensure $0 = 0
		mips_reg[0] = 0;
		//run through pipeline
		WB(&WB_counter,mips_reg);
		MEM(&MEM_counter);
		EX(&EX_counter, &pgm_c);
		ID(&ID_counter,mips_reg,&pgm_c);
		IF(&IF_counter,inst_mem,&pgm_c);

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
		//pgm_c+=4;
		sim_cycle+=1;
		test_counter++;
		if(sim_mode==1){
			printf("press ENTER to continue\n");
			while(getchar() != '\n');
		}
		//end of code2.c

		if(MEM_WB.opcode<0){break;}
	}

	/*Adjust utilization count for nops*/
	ifUtil -= nopTotal;
	idUtil -= nopTotal;
	exUtil -= nopTotal;
	memUtil -= nopTotal;
	wbUtil -= nopTotal;

//////////////////code3.c
	//Beginning of code3.c, code given to us to be put at the end of main
	if(sim_mode==0){
		fprintf(output,"program name: %s\n",argv[5]);
		fprintf(output,"stage utilization: %f  %f  %f  %f  %f \n", ifUtil/sim_cycle, idUtil/sim_cycle, exUtil/sim_cycle, memUtil/sim_cycle, wbUtil/sim_cycle);
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

}

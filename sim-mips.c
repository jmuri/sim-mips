// Author: Shikang Xu; ECE 353 TA

// List the full names of ALL group members at the top of your code.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
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

char *progScanner(char *instr_str){

}

char *regNumberConverter(){

}

void parser(char *instr_str){

}

void IF(){

}

void ID(){

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
	//End of code1.c

	//Start your code here
	char *instr_str;
	instr_str = malloc(100*sizeof(char));

	while(fgets(instr_str, 100, input))
		printf("next inst is %s", instr_str);
		progScanner(instr_str);
	fclose(input);





	//code2.c: The following code will output the register calue to 
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
		fprintf(output,"stage utilization: %f  %f  %f  %f  %f \n",
                             ifUtil, idUtil, exUtil, memUtil, wbUtil);
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

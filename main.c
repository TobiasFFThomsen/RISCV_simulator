#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define memSpace 750000

unsigned char instruction[memSpace][4]; 
unsigned int instructionCombined[memSpace]; 

unsigned int reg[32]; 

int finalInstruction = 0; 

struct instructionSet{
	unsigned char opcode[memSpace];
	unsigned char rd[memSpace];
	unsigned char funct3[memSpace];
	unsigned char rs1[memSpace];
	unsigned char rs2[memSpace];
	unsigned char funct7[memSpace];
	unsigned int imm[memSpace];
} instr; 


void readBinFile(){
	// Open the binary instruction file
	FILE *fp;
	fp = fopen("C:/RISC-V_simulator/RISC-V_simulator/addlarge.bin", "r");
	
	char fileContent = fgetc(fp);
	
	// Load all instructions into the 'instruction'-matrix (little endian format)
	for(int lineNo = 0 ; fileContent != EOF ; lineNo++){
		for(int i = 3 ; i >= 0 ; i--){
			instruction[lineNo][i] = fileContent; 
			fileContent = fgetc(fp);
		}
		finalInstruction = lineNo;
	}
	
	// Combine the instruction chars to one single word, per instruction
	for(int lineNo = 0 ; lineNo <= finalInstruction ; lineNo++){
		instructionCombined[lineNo] = (instruction[lineNo][0] << 24 | instruction[lineNo][1] << 16 | instruction[lineNo][2] << 8 | instruction[lineNo][3] );
	}
}

void printInstructionMatrix(){
	printf("\n");
	printf("Choose print mode for instructions (0 for combined, 1 for divided): ");
	int mode = getchar()-'0';
	
	switch(mode){
		case 0: 
			//Print the combined instructions, line by line
			printf("instructionLine #:[3] .. [0]\n");
			for(int lineNo = 0 ; lineNo <= finalInstruction ; lineNo++){
				printf("instructionLine %d: %X\n", lineNo, instructionCombined[lineNo]);
			}
			break;
		case 1: 
			//Print the divided instructions, line by line
			printf("Instruction #%c[3]%c[2]%c[1]%c[0]\n", 9, 9, 9, 9);
			for(int i = 0 ; i <= finalInstruction ; i++){ 
				printf("Instruction %d:", i);
				for(int j = 0 ; j < 4 ; j++){
					printf("%c%X", 9, instruction[i][j]);
				}
				printf("\n");
			}
			break;
		default: 
			break; 
	}
	
}

void formatFinder(){
	char format; 
	for(int i = 0 ; i <= finalInstruction ; i++){
        instr.opcode[i] = instructionCombined[i] & 0x7F;
		switch(instr.opcode[i]){
			case 0x33: 
				format = 'R';
				break;
			case 0x03:
			case 0x13:
            case 0x67:
				format = 'I';
				break;
			case 0x23 : 
				format = 'S';
				break;
			case 0x63 : 
				format = 'B';       //SB
				break;
			case 0x17 : 
            case 0x37 :
				format = 'U';
				break;
			case 0x6F : 
				format = 'J';
				break;
			default : 
				break;
		}
		switch(format){
			case 'R' :
				instr.rd[i] = (instructionCombined[i] >> 7) & 0x1F;
				instr.funct3[i] = (instructionCombined[i] >> 12) & 0x7;
				instr.rs1[i] = (instructionCombined[i] >> 15) & 0x1F;
				instr.rs2[i] = (instructionCombined[i] >> 20) & 0x1F;
				instr.funct7[i] = (instructionCombined[i] >> 25) & 0x7F;
				break; 
			case 'I' :
				instr.rd[i] = (instructionCombined[i] >> 7) & 0x1F;
				instr.funct3[i] = (instructionCombined[i] >> 12) & 0x7;
				instr.rs1[i] = (instructionCombined[i] >> 15) & 0x1F;
				instr.imm[i] = (instructionCombined[i] >> 20) & 0x1F;
				break; 
			case 'S' :
                
				break; 
			case 'B' :
				break; 
			case 'U' :
				break; 
			case 'J' :
				break; 
		}
	}
		int num = 5; 
		printf("opcode: %X\nrd: %X\nfunct3: %X\nrs1: %X\nrs2: %X\nfunct7: %X\nimm: %X\n", instr.opcode[num], instr.rd[num], instr.funct3[num], instr.rs1[num], instr.rs2[num], instr.funct7[num], instr.imm[num]);
}

void instructionCase(){
    for(int i = 0; i < finalInstruction; i++){
            switch(instr.opcode[i]){
                case(0x03)  :
                    //lb
                    //lh
                    //lbu
                    //lhu
                case(0x13)  :
                    if(instr.funct3[i]==0x00)
                        reg[instr.rd[i]] = reg[0]+instr.imm[i];                                             //addi
                        
                    if(instr.funct3[i]==0x00||instr.funct7[i]==0x00)
                        reg[instr.rd[i]] = reg[instr.rs1[i]]<<instr.imm[i];                                 //slli
                        
                    if(instr.funct3[i]==0x02){
                        if(reg[instr.rs1[i]]<instr.imm[i]){
                            reg[instr.rd[i]] = 1;                                                           //slti
                        }else{
                            reg[instr.rd[i]] = 0;        
                           }
                           
                   if(instr.funct3[i]==0x04){
                            reg[instr.rd[i]] = reg[instr.rs1[i]]^instr.imm[i];                                  //xori     
                   }
                   if(instr.funct3[i]==0x05||instr.funct7[i]==0x00){
                       reg[instr.rd[i]] = (unsigned char)reg[instr.rs1[i]]>>instr.imm[i];                       //srli
                   }

                    if(instr.funct7[i]==0x05||instr.funct3[i]==0x20){                                           //srai
                        reg[instr.rd[i]] = reg[instr.rs1[i]]>>instr.imm[i];
                    }
                    
                    if(instr.funct7[i]==0x06){
                        reg[instr.rd[i]] = reg[instr.rs1[i]]|instr.imm[i];                                      //ori
                    }
                    
                    if(instr.funct7[i]==0x07){
                        reg[instr.rd[i]] = reg[instr.rs1[i]]+instr.imm[i];                                      //andi
                    }
                    
                 case(0x33) : 
                    if(instr)
                    
            }
        }
    }
} 





void test(){
    char s  = 0xF0;
   // char us = (unsigned)s;

    char sl = s>>1;
    char sa = (unsigned char)s>>1;
    
    printf("original %X\n", s);
    printf("Logical shift: %X\n", sa);
    printf("Algo shift: %X\n", sl); 
    
    
    
    
   // printf("unsigned representation %u", neg);
    
}


int main(int argc, char **argv){
/*	readBinFile();
	formatFinder();
	instructionCase();
	printInstructionMatrix(); */
    test();
	return 0;
}
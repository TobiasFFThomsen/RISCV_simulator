#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define memSpace 750000

unsigned char instruction[memSpace][4]; 
unsigned int instructionCombined[memSpace]; 
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
		instr.opcode[0] = 0x3B;
		switch(instr.opcode[i]){
			case 0x33: 
			case 0x3B:
				format = 'R';
				break;
			case 0x3 :
//			case 0xF :
			case 0x13:
			case 0x1B:
//			case 0xF : 
				format = 'I';
				break;
			case 0x23 : 
				format = 'S';
				break;
			case 0x63 : 
				format = 'B';
				break;
			case 0x17 : 
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
		int num = 0; 
		printf("opcode: %X\nrd: %X\nfunct3: %X\nrs1: %X\nrs2: %X\nfunct7: %X\nimm: %X\n", instr.opcode[num], instr.rd[num], instr.funct3[num], instr.rs1[num], instr.rs2[num], instr.funct7[num], instr.imm[num]);
}

void instructionCase(){
	
}

int main(int argc, char **argv){
	readBinFile();
	formatFinder();
	instructionCase();
	printInstructionMatrix(); 
	return 0;
}
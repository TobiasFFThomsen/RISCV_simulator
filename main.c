#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define memSpace 750000

unsigned int mem[memSpace];
unsigned char instruction[4]; 

int finalInstructionNo = 0; 

unsigned int reg[32]; 


struct instructionSet{
	unsigned char opcode;
	unsigned char rd;
	unsigned char funct3;
	unsigned char rs1;
	unsigned char rs2;
	unsigned char funct7;
	unsigned int imm;
} instr; 



void readBinFile(){
	// Open the binary instruction file
	FILE *fp;
	fp = fopen("/Users/PvWN/Dropbox/Computerarkitektur/Final_assignment/TestFiler/addlarge.bin", "r");
	
	char fileContent = fgetc(fp);
	int lineNo = 0;
	
	// Load all instructions into the 'instruction'-matrix (little endian format)
	while( fileContent != EOF){
		for(int i = 3 ; i >= 0 ; i--){
			instruction[i] = fileContent; 
			fileContent = fgetc(fp);
		}
		mem[lineNo] = (instruction[0] << 24 | instruction[1] << 16 | instruction[2] << 8 | instruction[3] );
		lineNo++;
	}
	finalInstructionNo = lineNo;
}

char formatFinder(int pc){
	char format; 
	instr.opcode = mem[pc] & 0x7F;
	
	switch(instr.opcode){
		case 0x33: 
			format = 'R';
			break;
		case 0x3 :
		case 0x13:
		case 0x67:
			format = 'I';
			break;
		case 0x23 : 
			format = 'S';
			break;
		case 0x63 : 
			format = 'B';
			break;
		case 0x17 :
		case 0x37 : 
			format = 'U';
			break;
		case 0x6F : 
			format = 'J';
			break;
	}
	return format; 
}


void registerDecoder(char format, int pc){
	switch(format){
		case 'R' :
			instr.rd = (mem[pc] >> 7) & 0x1F;
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.rs1 = (mem[pc] >> 15) & 0x1F;
			instr.rs2 = (mem[pc] >> 20) & 0x1F;
			instr.funct7 = (mem[pc] >> 25) & 0x7F;
			break; 
		case 'I' :
			instr.rd = (mem[pc] >> 7) & 0x1F;
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.rs1 = (mem[pc] >> 15) & 0x1F;
			instr.imm = (mem[pc] >> 20) & 0xFFF;
			break; 
		case 'S' :
			instr.imm = (mem[pc] >> 7) & 0x1F;
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.rs1 = (mem[pc] >> 15) & 0x1F;
			instr.rs2 = (mem[pc] >> 20) & 0x1F;
			instr.imm |= (mem[pc] >> 20) & 0xFE0; //imm[11:5]
			break; 
		case 'B' :
			instr.imm = (mem[pc] & 0xF00) >> 7; //imm[4:1]
			instr.imm |= (mem[pc] & 0x80) << 4 ; //imm[11]
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.rs1 = (mem[pc] >> 15) & 0x1F;
			instr.rs2 = (mem[pc] >> 20) & 0x1F;
			instr.imm |= (mem[pc] & 0x7E000080) >> 20; //imm[10:5]
			instr.imm |= (unsigned int)(mem[pc] & 0x80000000) >> 19; //imm[12]
			break; 
		case 'U' :
			instr.rd = (mem[pc] >> 7) & 0x1F;
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.imm = mem[pc] & 0xFFFFF000;
			break; 
		case 'J' :
			instr.rd = (mem[pc] >> 7) & 0x1F;
			instr.funct3 = (mem[pc] >> 12) & 0x7;
			instr.imm = (unsigned int)(mem[pc] & 0x80000000) >> 11; //imm[20]
			instr.imm |= (mem[pc] & 0x7FE00000) >> 20; //imm[10:1]
			instr.imm |= (mem[pc] & 0x100000) >> 9; //imm[11]
			instr.imm |= mem[pc] & 0xFF0000; //imm[19:12]
			break; 
	}
}


void instructionCase(){
   
            switch(instr.opcode){
                case(0x03)  :                                               //lb
                    if(instr.funct3==0x00){
                        reg[instr.rd] = mem[instr.rs1+instr.imm]>>24;
                    }
                    if(instr.funct3==0x04){                                 //lbu
                        reg[instr.rd] = (unsigned)mem[instr.rs1+instr.imm]>>24;
                    }    
                    
                    //lh
                    //lhu
                case(0x13)  :
                    if(instr.funct3==0x00){
                        reg[instr.rd] = reg[0]+instr.imm;                                                   //addi
                    }
    
                    if(instr.funct3==0x00&&instr.funct7==0x00){
                        reg[instr.rd] = reg[instr.rs1]<<instr.imm;                                          //slli
                    }
                        
                    if(instr.funct3==0x02){
                        if(reg[instr.rs1]<instr.imm){
                            reg[instr.rd] = 1;                                                               //slti
                        }else{
                            reg[instr.rd] = 0;        
                           }
                    }
                    
                   if(instr.funct3==0x04){
                        reg[instr.rd] = reg[instr.rs1]^instr.imm;                                      //xori     
                   }
                   if(instr.funct3==0x05&&instr.funct7==0x00){
                       reg[instr.rd] = (unsigned char)reg[instr.rs1]>>instr.imm;                       //srli
                   }

                    if(instr.funct3==0x05&&instr.funct3==0x20){                                        //srai
                        reg[instr.rd] = reg[instr.rs1]>>instr.imm;
                    }
                    
                    if(instr.funct3==0x06){
                        reg[instr.rd] = reg[instr.rs1]|instr.imm;                                      //ori
                    }
                    
                    if(instr.funct3==0x07){
                        reg[instr.rd] = reg[instr.rs1]+instr.imm;                                      //andi
                    }
                 break;  
                 
                 case(0x33) : 
                    if(instr.funct3==0x00&&instr.funct7==0x00){
                        reg[instr.rd] = reg[instr.rs1]+reg[instr.rs2];                                 //add
                    }
                    if(instr.funct3==0x0&&instr.funct7==0x20){
                        reg[instr.rd] = reg[instr.rs1]-reg[instr.rs2];                                 //sub
                    }
                    if(instr.funct3==0x01&&instr.funct7==0x00){                                        //sll
                        reg[instr.rd] = reg[instr.rs1]>>reg[instr.rs2];
                    }
                    if(instr.funct3==0x02&&instr.funct7==0x00){                                            //slt
                        if(reg[instr.rs1]<reg[instr.rs2]){
                            reg[instr.rd] = 1;
                        }else{
                            reg[instr.rd] = 0;
                        }
                    }
                    if(instr.funct3==0x03&&instr.funct7==0x00){                                             //sltu
                        if((unsigned char)reg[instr.rs1]<reg[instr.rs2]){
                            reg[instr.rd] = 1;   
                        }else{
                            reg[instr.rd] = 0;
                        }
                    }
                    if(instr.funct3==0x04){  
                        reg[instr.rd] = reg[instr.rs1]^reg[instr.rs2];                                       //xor
                    }
                    if(instr.funct3==0x05){
                        reg[instr.rd] = (unsigned char)reg[instr.rs1]>>reg[instr.rs2];                       //srl
                    }
                    if(instr.funct3==0x20){
                        reg[instr.rd] = reg[instr.rs1]>>reg[instr.rs2];                                      //sra       
                    }
                    if(instr.funct3==0x06){
                        reg[instr.rd] = reg[instr.rs1] | reg[instr.rs2];                                     //or
                    }
                    break;
                    
                    case(0x37) :
                    if(instr.funct3==0x07){
                        reg[instr.rd] = instr.imm<<12;                                                       //lui
                    }
                    break;
                    case(0x73):                                                                              //ecal
                        if(reg[10]==1){
                            printf("%d",reg[11]);
                        }else if(reg[10]==4){
                            //den der fucked up ting vi ikke kunne finde ud af...
                        }
                    default:
                    break;
            }
        }
     





void test(){
     int test = -2147483648; 
/*    char charBuf[3];
    sprintf(charBuf,"%i",testString);*/
    //charBuf[2]= 0;
/*     printf("charBuf[0] = %d , charBuf[1] = %d , charBuf[2] = %d , charBuf[3] = %d",charBuf[0]-'0',charBuf[1]-'0',charBuf[2]-'0',charBuf[3]-'0');*/
    
    //printf("unsigned representation %u", neg);
    
     int ashift = test>>24; 
     int lshift = (unsigned)test>>24;
    printf("original: %X, algorithmical shifted: %X,logical shifted: %X ",test, ashift,lshift);
}


int main(int argc, char **argv){
/*	readBinFile();
	formatFinder();
	instructionCase();
	printInstructionMatrix(); */
    test();
	return 0;
}
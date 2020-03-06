#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

#define OPCODE_NUMBER 101

static int LOC;
static int program_length;
static int starting_address;
static int symtab_size = 1;

const struct OPCODE
{
  char name[7];
  char code[2];
  int format;
}

OPCODES[] = {
				 {"ADD","18",3}, {"ADDF","58",3}, {"AND","40",3}, {"COMP","28",3}, {"COMPF","88",3}, {"DIV","24",3}, {"DIVF","64",3}, 
				 {"J","3C",3}, {"JEQ","30",3}, {"JGT","34",3}, {"JLT","38",3}, {"JSUB","48",3}, {"LDA","00",3}, {"LDB","68",3}, 
				 {"LDCH","50",3}, {"LDF","70",3}, {"LDL","08",3}, {"LDS","6C",3}, {"LDT","74",3}, {"LDX","04",3}, {"LPS","D0",3}, 
				 {"MUL","20",3}, {"MULF","60",3}, {"OR","44",3}, {"RD","D8",3}, {"RSUB","4C",3}, {"SSK","EC",3}, {"STA","0C",3}, 					 {"STB","78",3}, {"STCH","54",3}, {"STF","80",3}, {"STI","D4",3}, {"STL","14",3}, {"STS","7C",3}, {"STSW","E8",3}, 					 {"STT","84",3}, {"STX","10",3}, {"SUB","1C",3}, {"SUBF","5C",3}, {"TD","E0",3}, {"TIX","2C",3}, {"WD","DC",3}, 
				 
				 {"+ADD","18",4}, {"+ADDF","58",4}, {"+AND","40",4}, {"+COMP","28",4}, {"+COMPF","88",4}, {"+DIV","24",4}, 
				 {"+DIVF","64",4}, {"+J","3C",4}, {"+JEQ","30",4}, {"+JGT","34",4}, {"+JLT","38",4}, {"+JSUB","48",4}, {"+LDA","00",4}, 				 {"+LDB","68",4}, {"+LDCH","50",4}, {"+LDF","70",4}, {"+LDL","08",4}, {"+LDS","6C",4}, {"+LDT","74",4}, {"+LDX","04",4}, 					 {"+LPS","D0",4}, {"+MUL","20",4}, {"+MULF","60",4}, {"+OR","44",4}, {"+RD","D8",4}, {"+RSUB","4C",4}, {"+SSK","EC",4}, 				 {"+STA","0C",4}, {"+STB","78",4}, {"+STCH","54",4}, {"+STF","80",4}, {"+STI","D4",4}, {"+STL","14",4}, {"+STS","7C",4}, 
				 {"+STSW","E8",4}, {"+STT","84",4}, {"+STX","10",4}, {"+SUB","1C",4}, {"+SUBF","5C",4}, {"+TD","E0",4}, {"+TIX","2C",4}, 					 {"+WD","DC",4}, 
				 
				 {"TIXR","B8",2},{"SVC","B0",2},{"SUBR","94",2},{"SHIFTR","A8",2},{"SHIFTL","A4",2},{"RMO","AC",2},{"MULR","98",2},
				 {"DIVR","9C",2},{"COMPR","A0",2},{"CLEAR","B4",2},{"ADDR","90",2},
				 { "FIX","C4",1}, { "HIO", "F4", 1}, {"FLOAT","",1}, {"TIO","",1}, {"SIO","",1}, {"NORM","",1}

};

static struct table
{
  char name[7];
  int address;
  int type;
  int length;
}SYMTAB[1000];

void insert(char lexeme[])
{
   int i;
   for(i=0;i<symtab_size;i++)
   {
      if(strcmp(SYMTAB[i].name,lexeme)==0)
      {
         printf("Duplicate Label : %s\n",lexeme);
         exit(0);
      }
   }
   strcpy(SYMTAB[symtab_size-1].name,lexeme);
   SYMTAB[symtab_size-1].address = LOC;
   symtab_size++;
}

void print_symtab()
{
  FILE *s;
  s = fopen("symtab.txt","w");
  int i;
  for(i=0;i<symtab_size-1;i++)
  {
     fprintf(s,"%s %d\n",SYMTAB[i].name,SYMTAB[i].address);
     printf("%s\t%d\n",SYMTAB[i].name,SYMTAB[i].address);
  }
}

void set_starting_address(int a)
{
  LOC = a;
  starting_address = LOC;
}

int check_in_opcode_table(char *token)
{
  int i;
  for(i=0;i<OPCODE_NUMBER;i++)
    if(strcmp(token,OPCODES[i].name)==0)
      return i+1;
  return 0;
}

void opcode(char *token)
{
  int n = check_in_opcode_table(token);
  if(n!=0)
    LOC += OPCODES[n-1].format;
  else if(n==0)
    printf("Unknown Opcode %s\n",token);
}

void opcode_number(char *token,int number)
{
  int flag = 0;
  int n = check_in_opcode_table(token);
  if(strcmp(token,"WORD")==0)
  {
    LOC += 3;
    flag = 1;
  }
  else if(strcmp(token,"RESW")==0)
  {
    LOC += 3*number;
    flag = 1;
  }
  else if(strcmp(token,"RESB")==0)
  {
    LOC += number;
    flag = 1;
  }
  else if(strcmp(token,"BYTE")==0)
  {
    LOC += number;
  }
  else
    LOC += OPCODES[n-1].format;
  if(n==0 && flag==0)
    printf("Unknown Opcode %s\n",token);
}

int get_object_code(char op_code[],char opcode[])
{
  int n = check_in_opcode_table(opcode);
  if(n!=0)
    strcpy(op_code,OPCODES[n-1].code);
  return n; 
}


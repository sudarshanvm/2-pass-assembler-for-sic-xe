%{

#include "ds.c"


FILE *out;

void yyerror();
void file_setup();
int yylex();

extern int yylineno;

/*void insert(char a[]);
void print_symtab();
void set_starting_address(int a);
int check_in_opcode_table(char *token);
void opcode(char *token);
void opcode_number(char *token,int number);
int get_object_code(char op_code[],char opcode[]);*/

%}

%token START END
%union
{
   int number;
   char* string;
}
%token <string> LABEL
%token <string> OPCODE
%token <number> NUMBER
%token <string> RSUB
%token <string> FORMAT1
%token <string> BASE

%%
S  : LABEL START Y X END START | START Y X END START  
   ;
Y  : NUMBER        { 
				 		fprintf(out,"%d %s\n",$1,"START");
                        set_starting_address($1);
			       }
	;		       
X  : type1 X | type2 X | type3 X | type4 X | type5 X | type6 X| ;

type1 : LABEL OPCODE LABEL   { 
							 fprintf(out,"%d %s %s\n",LOC,$2,$3);
							 insert($1);
                             opcode($2);
			       		  }
	  ;	       		  
type2 : LABEL OPCODE NUMBER  { 
				 			 fprintf(out,"%d %s %d\n",LOC,$2,$3);
			         		 insert($1);
                             opcode_number($2,$3);
			       		  }
			       		  
   | LABEL OPCODE '#' NUMBER   {
           			 			 fprintf(out,"%d %s #%d\n",LOC,$2,$4);
                                 insert($1);
                                 opcode_number($2,$4);
                               }
   ;
   
type3 : OPCODE LABEL  {
				 		fprintf(out,"%d %s %s\n",LOC,$1,$2);
 				 		opcode($1);
			       }
	   ;
	   		       
type4 : OPCODE NUMBER  {
				 		fprintf(out,"%d %s %d\n",LOC,$1,$2); 
				 		opcode($1);
			       }
			       
   | OPCODE '#' NUMBER  {
	                       fprintf(out,"%d %s #%d\n",LOC,$1,$3);
				 		   opcode($1);
			       		}
	   ;
	   		       		 
type5 : RSUB  {
              fprintf(out,"%d %s\n",LOC,$1);
			  opcode($1);
           }
           
   | FORMAT1 {
 				 fprintf(out,"%d %s\n",LOC,$1);
				 opcode($1);
             }
   ;
   
type6: BASE LABEL {printf("BASE on line %d\n",yylineno-1);}
	 ; 
%%

void yyerror()
{
   printf("Error on line %d in the input assembly program.\n", yylineno);
   exit(0);
}

int main()
{
  out = fopen("output.txt","w");
  file_setup();
  printf("Scanning the assembly file...\n");
  yyparse(); 
  fprintf(out,"%d %s",LOC,"END START\n");
  printf("All instructions in the given program are Syntactically Correct.\n");
  printf("SYMTAB:\n");
  print_symtab();
}


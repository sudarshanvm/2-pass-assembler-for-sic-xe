%{
#include "y.tab.h"
#include<stdio.h>
%}

%%
"START"	{ yylineno++; return START; }

"END"	{ yylineno++; return END;   }

[0-9]+  { 
            yylval.number = atoi(yytext);
		    return NUMBER;
 		}
 		
(ADD|AND|ADDF|COMP|COMPF|DIV|DIVF|J|JEQ|JLT|JGT|JSUB|LDA|LDB|LDCH|LDF|LDL|LDS|LDT|LDX|LPS|MUL|MULF|OR|RD|SSK|STA|STB|STCH|STF|STI|STL|STS|STSW|STT|STX|SUB|SUBF|TD|TIX|WD|ADDR|SUBR|MULR|DIVR|TIXR|CLEAR|COMPR|RMO|SHIFTL|SHIFTR|WORD|RESW|RESB|BYTE)     { 
																			yylineno++;
																			yylval.string = strdup(yytext);		
									  										return OPCODE;
																		}
																		
"+JSUB"	{
		  yylineno++;	
		  yylval.string = strdup(yytext);
		  return OPCODE;
		}
		
"RSUB"  {
          yylineno++;
          yylval.string = strdup(yytext);
		  return RSUB;
		}
		
(FIX|FLOAT|HIO|SIO|TIO|NORM)	{
			  yylineno++;
			  yylval.string = strdup(yytext);
		      return FORMAT1;
			}
			
"BASE" 	{ 
			yylineno++;
			yylval.string=strdup(yytext); 
			return BASE;
		}
			
[,a-zA-Z]+	{ 
			    
			    yylval.string = strdup(yytext);
		        return LABEL;  
			}
			
#			{
				return yytext[0];
			}
			
.	{}

%%

int yywrap()
{
  return 1;
}

void file_setup()
{
  yyin = fopen("input.txt","r");
  yyout = fopen("output.txt","w");
}

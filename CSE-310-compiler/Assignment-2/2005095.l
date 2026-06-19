%option noyywrap yylineno

%x STRING_STATE
%x MULTILINE_COMMENT_STATE

%{
#include<bits/stdc++.h>
#include"SymbolTable.cpp"
#include"ScopeTable.cpp"
#include"SymbolInfo.cpp"
using namespace std;


SymbolTable symboltable(7);

int line_count=1;
int store_line_count;
int string_line = 0;
int error_count=0;
ofstream tokenout;

string str = "";
string original_str = "";

char makeSpecialChar(char * ch)   {
		if(ch[1]  !=  '\\'){
			return ch[1];
		}
		else if(ch[1] == '\\'){
			 if(ch[2] == 'n'){
			 	return (char)10;
			 }
			 if(ch[2] == 't'){
			 	return (char)9;
			 }
			 if(ch[2] == 'f'){
			 	return (char)12;
			 }
			 if(ch[2] == 'v'){
			 	return (char)11;
			 }
			 if(ch[2] == 'r'){
			 	return (char)13;
			 }
			 if(ch[2] == 'a'){
			 	return (char)7;
			 }
			 if(ch[2] == 'b'){
			 	return (char)8;
			 }
			 if(ch[2] == '0'){
			 	return (char)0;
			 }
			 if(ch[2] == '\\'){
			 	return (char)92;
			 }
			 if(ch[2] == '\''){
			 	return (char)39;
			 }	
			 if(ch[2] == '\"'){
			 	return (char)34;
			 }	
	}
}
%}
  
WHITESPACE [  \f\r\t\v]+
NEWLINE \r?\n
DIGIT [0-9]
DIGITS {DIGIT}+

ADDOP [+-]
MULOP [*/%]
INCOP ("++"|"--")
RELOP ("<"|"<="|">"|">="|"=="|"!=")
ASSIGNOP [=]
LOGICOP ("&&"|"||")
NOT [!]
LPAREN [(]
RPAREN [)]
LCURL [{]
RCURL [}]
LTHIRD ("[")
RTHIRD("]")
COMMA [,]
SEMICOLON [;]

IDENTIFIER [A-Za-z_][a-zA-Z0-9_]*

INTEGER_LITERAL {DIGITS}
FLOAT_LITERAL {DIGITS}?(\.{DIGITS})?([Ee][+-]?{DIGITS})?

CHAR '([ -~]{-}['\\])'
SPECIAL_CHAR '(\\[\\nfratbv0\"\'])'
CHARACTER_LITERAL {CHAR}|{SPECIAL_CHAR}


STRING_START \"
SPECIAL_STRING2 (\\\\(\r)?\n)
SPECIAL_STRING1 (\\[\\nfratbv0\"\'])

SINGLELINE_COMMENT \/\/(([\\]+([\n])?)?|([^\n])*)*

MULTILINE_COMMENT_START (\/\*)
MULTILINE_COMMENT (\/\*)([^\*]|\*[^\/])*(\*\/)

ILL_FORMED_NUMBER ({DIGIT}*([Ee][+-]?{DIGITS})+(\.{DIGITS})+)|({DIGITS}\.)+
TOO_MANY_DECIMAL_POINT ({DIGITS}\.{DIGITS}\.{DIGITS})+
MULTI_CHAR_CONST '(([ -~]{-}['\\])([ -~]{-}['\\])+)'

UNFINISHED_COMMENT (\/\*)([^\*]|\*[^\/])*

UNFINISHED_CHARACTER ('([ -~]{-}['\\])*)|('(\\[\\nfratbv0\"\'])*)

%%
{WHITESPACE}    {    }
 
"if"         {
			tokenout << "<IF> ";
			logout << "Line no " << line_count<< ": Token <IF> Lexeme " << yytext <<" found\n";
			logout << endl;
		}   
"else"   {
			tokenout << "<ELSE> ";
			logout << "Line no " << line_count<< ": Token <ELSE> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"for"   {
			tokenout << "<FOR> ";
			logout << "Line no " << line_count<< ": Token <FOR> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"while"   {
			tokenout << "<WHILE> ";
			logout << "Line no " << line_count<< ": Token <WHILE> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"do"   {
			tokenout << "<DO> ";
			logout << "Line no " << line_count<< ": Token <DO> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"break"   {
			tokenout << "<BREAK> ";
			logout << "Line no " << line_count<< ": Token <BREAK> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"int"   {
			tokenout << "<INT> ";
			logout << "Line no " << line_count<< ": Token <INT> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"char"   {
			tokenout << "<CHAR> ";
			logout << "Line no " << line_count<< ": Token <CHAR> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"float"   {
			tokenout << "<FLOAT> ";
			logout << "Line no " << line_count<< ": Token <FLOAT> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"double"   {
			tokenout << "<DOUBLE> ";
			logout << "Line no " << line_count<< ": Token <DOUBLE> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"void"   {
			tokenout << "<VOID> ";
			logout << "Line no " << line_count<< ": Token <VOID> Lexeme " << yytext <<" found\n";
			logout << endl;
		}	
"return"   {
			tokenout << "<RETURN> ";
			logout << "Line no " << line_count<< ": Token <RETURN> Lexeme " << yytext <<" found\n";
			logout << endl;
		}	
"switch"   {
			tokenout << "<SWITCH> ";
			logout << "Line no " << line_count<< ": Token <SWITCH> Lexeme " << yytext <<" found\n";
			logout << endl;
		}
"case"   {
			tokenout << "<CASE> ";
			logout << "Line no " << line_count<< ": Token <CASE> Lexeme " << yytext <<" found\n";
			logout << endl;
		}		
"default"   {
			tokenout << "<DEFAULT> ";
			logout << "Line no " << line_count<< ": Token <DEFAULT> Lexeme " << yytext <<" found\n";
			logout << endl;
		}		
"continue"   {
			tokenout << "<CONTINUE> ";
			logout << "Line no " << line_count<< ": Token <CONTINUE> Lexeme " << yytext <<" found\n";
			logout << endl;
		}	
{ADDOP}     {
			tokenout << "<ADDOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <ADDOP> Lexeme " << yytext <<" found\n";
			logout << endl;	
}	
{MULOP}     {
			tokenout << "<MULOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <MULOP> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{INCOP}     {
			tokenout << "<INCOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <INCOP> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{RELOP}     {
			tokenout << "<RELOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <RELOP> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{ASSIGNOP}     {
			tokenout << "<ASSIGNOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <ASSIGNOP> Lexeme " << yytext <<" found\n";
			logout << endl;
}
{LOGICOP}     {
			tokenout << "<LOGICOP, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <LOGIC> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{NOT}     {
			tokenout << "<NOT, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <NOT> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{LPAREN}     {
			tokenout << "<LPAREN, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <LPAREN> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{RPAREN}     {
			tokenout << "<RPAREN, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <RPAREN> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{LCURL}     {
			tokenout << "<LCURL, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <LCURL> Lexeme " << yytext <<" found\n";	
			logout << endl;
			symboltable.EnterScope();	
}
{RCURL}     {
			tokenout << "<RCURL, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <RCURL> Lexeme " << yytext <<" found\n";
			logout << endl;
			symboltable.ExitScope();	
}
{LTHIRD}     {
			tokenout << "<LTHIRD, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <LTHIRD> Lexeme " << yytext <<" found\n";
			logout << endl;
}
{RTHIRD}     {
			tokenout << "<RTHIRD, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <RTHIRD> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{COMMA}     {
			tokenout << "<COMMA, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <COMMA> Lexeme " << yytext <<" found\n";	
			logout << endl;
}
{SEMICOLON}     {
			tokenout << "<SEMICOLON, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <SEMICOLON> Lexeme " << yytext <<" found\n";	
			logout << endl;
}

{IDENTIFIER}  {
			tokenout << "<ID, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <ID> Lexeme " << yytext <<" found\n";
			logout << endl;
			bool inserted = symboltable.Insert(yytext,"ID");
			if(inserted){
				symboltable.printAllScopeTable();
			}
}	

{INTEGER_LITERAL}   {
			tokenout << "<CONST_INT, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <CONST_INT> Lexeme " << yytext <<" found\n";
			logout << endl;
			bool inserted = symboltable.Insert(yytext,"CONST_INT");
			if(inserted){
				symboltable.printAllScopeTable();
			}
}

{FLOAT_LITERAL}  {
			tokenout << "<CONST_FLOAT, "<< yytext << "> ";
			logout << "Line no " << line_count<< ": Token <CONST_FLOAT> Lexeme " << yytext <<" found\n";
			logout << endl;
			bool inserted = symboltable.Insert(yytext,"CONST_FLOAT");
			if(inserted){
				symboltable.printAllScopeTable();
			}		
}

{CHARACTER_LITERAL}  {
		tokenout << "<CONST_CHAR, "<< makeSpecialChar(yytext) << "> ";
		logout << "Line no " << line_count<< ": Token <CONST_CHAR> Lexeme " << yytext <<" found --> " <<   "<CHAR_CONST, "<< makeSpecialChar(yytext) << ">\n";
		logout << endl;
		bool inserted = symboltable.Insert(yytext,"CONST_CHAR");
		if(inserted){
			symboltable.printAllScopeTable();
		}
}

{STRING_START} {
		BEGIN STRING_STATE;
		store_line_count = line_count;
		//string_line++;
		str.append(yytext);
		original_str.append(yytext);
}

<STRING_STATE><<EOF>> {
		logout << "Error at line no " << store_line_count+1 << ": " << "Unterminated String " <<  str << endl;
		logout << endl;
		error_count++;
		str = "";
		original_str = "";
		BEGIN INITIAL;
		return 0;
}

<STRING_STATE>{NEWLINE} {
		logout << "Error at line no " << store_line_count << ": " << "Unterminated String " <<  original_str << endl;
		logout << endl;
		line_count++;
		error_count++;
		str = "";
		original_str = "";
		BEGIN INITIAL;
}

<STRING_STATE>\" {
		str.append(yytext);
		original_str.append(yytext);
		tokenout << "<STRING, "<< str << "> ";
		logout << "Line no " << store_line_count << ": Token <STRING> Lexeme " << original_str <<" found --> " <<   "<STRING, "<< str << ">\n";
		logout << endl;
		str = "";
		original_str = "";
		BEGIN INITIAL;
}

<STRING_STATE>((\\){NEWLINE}) {
		line_count++;
		original_str.append(yytext);
}

<STRING_STATE>{SPECIAL_STRING1} {
		original_str.append(yytext);
		if(yytext[1] == 'n'){
			str.append(1u, '\n');
		}
		if(yytext[1] == 't'){
			str.append(1u, '\t');
		}
		if(yytext[1] == 'a'){
			str.append(1u, '\a');
		}
		if(yytext[1] == 'b'){
			str.append(1u, '\b');
		}
		if(yytext[1] == 'r'){
			str.append(1u, '\r');
		}
		if(yytext[1] == 'f'){
			str.append(1u, '\f');
		}
		if(yytext[1] == 'v'){
			str.append(1u, '\v');
		}
		if(yytext[1] == '0'){
			str.append(1u, '\0');
		}
		if(yytext[1] == '\\'){
			str.append(1u, '\\');
		}	
		if(yytext[1] == '\''){
			str.append(1u, '\'');
		}
		if(yytext[1] == '\"'){
			str.append(1u, '\"');
		}
}

<STRING_STATE>. {
		str.append(yytext);
		original_str.append(yytext);
}


{SINGLELINE_COMMENT}  {
		logout << "Line no " << line_count<< ": Token <COMMENT> Lexeme " << yytext <<" found\n";	
		for(int i = 0; i < strlen(yytext); i++){
			if(yytext[i] == '\n'){
				line_count++;
			}
		}	
		logout << endl;	
}


{MULTILINE_COMMENT}  {
		logout << "Line no " << line_count<< ": Token <COMMENT> Lexeme " << yytext <<" found\n";
		for(int i = 0; i < strlen(yytext); i++){
			if(yytext[i] == '\n'){
				line_count++;
			}
		}	
		logout << endl;			
}
		
{NEWLINE}    {
			line_count++;
}

{ILL_FORMED_NUMBER} {
		logout << "Error at line no "  << line_count << ": " << "Ill formed number " <<  yytext << endl;
		logout << endl;
		error_count++;
}

{TOO_MANY_DECIMAL_POINT} {
		logout << "Error at line no " << line_count << ": " << "Too many decimal points " <<  yytext << endl;
		logout << endl;
		error_count++;
}

([0-9]+[a-zA-Z_]+)|([a-zA-Z_]+[0-9]+) {
		logout << "Error at line no " << line_count << ": " << "Invalid prefix on ID or invalid suffix on Number " <<  yytext << endl;
		logout << endl;
		error_count++;
}

{MULTI_CHAR_CONST} {
		logout << "Error at line no " << line_count << ": " << "Multi character constant error " <<  yytext << endl;
		logout << endl;
		error_count++;
}

{UNFINISHED_COMMENT} {
		logout << "Error at line no " << line_count << ": " << "Unterminated Comment " <<  yytext << endl;
		for(int i = 0; i < strlen(yytext); i++){
			if(yytext[i] == '\n'){
				line_count++;
			}
		}
		error_count++;
}


{UNFINISHED_CHARACTER} {
		logout << "Error at line no " << line_count << ": " << "Unterminated character " <<  yytext << endl;
		logout << endl;
		error_count++;
}

'' {
		logout << "Error at line no " << line_count << ": " << "Empty character constant error " <<  yytext << endl;
		logout << endl;
		error_count++;
}

. {
		logout << "Error at line no " << line_count << ": " << "Unrecognized character " <<  yytext << endl;
		logout << endl;
		error_count++;
}

<<EOF>> {
	line_count--;
	symboltable.printAllScopeTable();
	logout << "Total lines: " << line_count << endl;
	logout << "Total errors: " << error_count << endl;
	return 0;
}

	
%%


int main(int argc,char *argv[]){
	
	if(argc!=2){
		cout << "Please provide input file name and try again\n";
		return 0;
	}
	
	FILE *file=fopen(argv[1],"r");
	if(file==NULL){
		cout << "Cannot open specified file\n";
		return 0;
	}
	tokenout.open("2005095_token.txt");
	
	logout << endl;
	
	yyin= file;
	yylex();
	fclose(yyin);
	logout.close();
	tokenout.close();
	return 0;
}

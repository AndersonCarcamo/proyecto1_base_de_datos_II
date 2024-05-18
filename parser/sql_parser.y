%{
#include <iostream>
#include <string>
#include "Scanner.hpp"
%}

%require "3.7.4"
%language "C++"
%defines "Parser.hpp"
%output "Parser1.cpp"

%define api.parser.class {Parser}
%define api.namespace {sql_parser}
%define api.value.type {std::string}
%param {yyscan_t scanner}

%code provides
{
    #define YY_DECL \
        int yylex(sql_parser::Parser::semantic_type *yylval, yyscan_t yyscanner)
    YY_DECL;
}

%token EOL CREATE TABLE FROM FILE USING INDEX SELECT WHERE BETWEEN INSERT INTO VALUES DELETE

%token IDENTIFIER STRING_LITERAL INTEGER_LITERAL COMMA LPAREN RPAREN EQ AND

%%
command_list
    : /* empty */
    | command_list command
    ;

command
    : create_table_command
    | select_command
    | insert_command
    | delete_command
    ;

create_table_command
    : CREATE TABLE IDENTIFIER FROM FILE STRING_LITERAL USING INDEX index_type LPAREN IDENTIFIER RPAREN EOL
    {
        std::cout << "Create table command: " << $3 << " from file " << $6 << " using index " << $9 << "(" << $11 << ")" << std::endl;
    }
    ;

index_type
    : IDENTIFIER
    ;

select_command
    : SELECT '*' FROM IDENTIFIER where_clause EOL
    {
        std::cout << "Select command from " << $4 << " where " << $5 << std::endl;
    }
    ;

where_clause
    : WHERE IDENTIFIER EQ value
    {
        $$ = $2 + " = " + $4;
    }
    | WHERE IDENTIFIER BETWEEN value AND value
    {
        $$ = $2 + " between " + $4 + " and " + $6;
    }
    | /* empty */
    {
        $$ = "true";
    }
    ;

insert_command
    : INSERT INTO IDENTIFIER VALUES LPAREN value_list RPAREN EOL
    {
        std::cout << "Insert command into " << $3 << " values (" << $6 << ")" << std::endl;
    }
    ;

value_list
    : value
    | value_list COMMA value
    ;

value
    : STRING_LITERAL
    | INTEGER_LITERAL
    ;

delete_command
    : DELETE FROM IDENTIFIER WHERE IDENTIFIER EQ value EOL
    {
        std::cout << "Delete command from " << $3 << " where " << $5 << " = " << $7 << std::endl;
    }
    ;

%%

void sql_parser::Parser::error(const std::string& msg) {
    std::cerr << "Error: " << msg << '\n';
}
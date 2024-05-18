%{
#include <iostream>
#include <string>
#include "Scanner.hpp"
#include "../extendiblehashing/Bucket_EH.hh"
#include "../extendiblehashing/ExtendibleHashing.hh"


using namespace std;
using namespace sql_parser;

directory_EH<string> hashTable(5, 5); // inicilizaremos con un deph global de 3 y size de bucket 4
void loadCSVToHashTable(const std::string& filename, const std::string& key);
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
    : CREATE TABLE IDENTIFIER FROM FILE STRING_LITERAL USING INDEX LPAREN IDENTIFIER RPAREN EOL
    {
        std::cout << "Create table command: " << $3 << " from file " << $6 << " using index on " << $10 << std::endl;
        loadCSVToHashTable($6, $10);
    }
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

void loadCSVToHashTable(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> columns;
    bool firstLine = true;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Registro record;
        bool isKeyColumn = false;

        int colIdx = 0;
        while (std::getline(ss, token, ',')) {
            if (firstLine) {
                columns.push_back(token);
                if (token == key) {
                    isKeyColumn = true;
                }
            } else {
                if (columns[colIdx] == key) {
                    record.setKey(key, token);
                } else {
                    record.setField(columns[colIdx], token);
                }
                colIdx++;
            }
        }

        if (!firstLine) {
            hashTable.add(record.getKey(), record);
        }

        firstLine = false;
    }

    file.close();
}
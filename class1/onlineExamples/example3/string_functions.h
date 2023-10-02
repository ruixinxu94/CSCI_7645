#pragma once

/* Declares functions for converting strings to uppercase and lowercase
* Author: Sourav Mukherjee, sourav@fdu.edu
*/

void stringToUpper(char* input);

void stringToLower(char* output);

void stringModify(char* input, char (*visit)(char));
char* lower(char* str);
char* upper(char* str);

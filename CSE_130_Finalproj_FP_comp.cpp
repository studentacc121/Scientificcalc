/*
Fern Potter
CSE 130
Scientific calculator program

made in assistance with generative AI. Uses of AI listed below:
	inserting debug statements (printf) in designed functions
 	identifying problems and mistakes
  	finishing repetiive tasks
   	adjusting formatting for readability
AI NOT used for:
	major design or strucuture
 	any original statements and design methods
	significant design methods
 	large chunks of code which are not repetitive
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>

void calculator(char userin[]);
void accessHistory();
long double strntold(char strnum1[]);
long double addsub(char userin[], int strleng);
double mdas(char userin[], int *digits);
int formattingchk(char userin[], char *type);
void remove_newline(char str[]);
int opformattingchk(char userin[]);
void addtohis(char userin[]);
void exppro(char userin[], int openpar, int closepar, double val);

double mdas(char userin[], int *digits) { // handles multiplication division addition and subtraction when given only related characters
    //printf("\nmdas loop, string is: %s\n", userin);
    int len = strlen(userin);
    int i, j;
    char temp[1000];
    double numarr1[1000];
    char oparr1[1000];
    int numuq1 = -1, opuq = -1;

    //printf("Starting mdas with input: %s\n", userin);

    for (i = 0; i < len; i++) {
        if (isdigit(userin[i]) || userin[i] == '.') {
            j = 0;
            while (i < len && (isdigit(userin[i]) || userin[i] == '.')) {
                temp[j++] = userin[i++];
            }
            temp[j] = '\0';
            numarr1[++numuq1] = strntold(temp);
            //printf("Pushed number: %Lf\n", numarr1[numuq1]);
            i--; // Adjust index necessary because it will be incremented in the loop
        } else if (userin[i] == '+' || userin[i] == '-') {
            if (i > 0 && (userin[i - 1] == '*' || userin[i - 1] == '/' || userin[i - 1] == '+' || userin[i - 1] == '-')) {
                // Handle the case of valid operator duos
                j = 0;
                temp[j++] = userin[i++];
                while (i < len && (isdigit(userin[i]) || userin[i] == '.')) {
                    temp[j++] = userin[i++];
                }
                temp[j] = '\0';
                numarr1[++numuq1] = strntold(temp);
                //printf("Pushed number: %Lf\n", numarr1[numuq1]);
                i--; // Adjust index necessary because it will be incremented in the loop
            } else {
                while (opuq >= 0 && (oparr1[opuq] == '*' || oparr1[opuq] == '/' || oparr1[opuq] == '+' || oparr1[opuq] == '-')) {
                    double b = numarr1[numuq1--];
                    double a = numarr1[numuq1--];
                    switch (oparr1[opuq--]) {
                        case '+': numarr1[++numuq1] = a + b; break;
                        case '-': numarr1[++numuq1] = a - b; break;
                        case '*': numarr1[++numuq1] = a * b; break;
                        case '/': numarr1[++numuq1] = a / b; break;
                    }
                    //printf("Performed operation, stack top now: %Lf\n\n", numarr1[numuq1]);
                }
                oparr1[++opuq] = userin[i];
                //printf("Pushed operator: %c\n", oparr1[opuq]);
            }
        } else if (userin[i] == '*' || userin[i] == '/') {
            while (opuq >= 0 && (oparr1[opuq] == '*' || oparr1[opuq] == '/')) {
                double b = numarr1[numuq1--];
                double a = numarr1[numuq1--];
                switch (oparr1[opuq--]) {
                    case '*': numarr1[++numuq1] = a * b; break;
                    case '/': numarr1[++numuq1] = a / b; break;
                }
                //printf("Performed operation, stack top now: %Lf\n\n", numarr1[numuq1]);
            }
            oparr1[++opuq] = userin[i];
            //printf("Pushed operator: %c\n", oparr1[opuq]);
        }
    }

    while (opuq >= 0) {
        double b = numarr1[numuq1--];
        double a = numarr1[numuq1--];
        switch (oparr1[opuq--]) {
            case '+': numarr1[++numuq1] = a + b; break;
            case '-': numarr1[++numuq1] = a - b; break;
            case '*': numarr1[++numuq1] = a * b; break;
            case '/': numarr1[++numuq1] = a / b; break;
        }
        //printf("Performed operation, stack top now: %Lf\n\n", numarr1[numuq1]);
    }

    double result = numarr1[numuq1];
    // printf("Final result in mdas: %Lf\n\n\n", result);

    return result;
}

void addtohis(char userin[]) { // adds to history file
    FILE *file1;
    file1 = fopen("./calculatorHistory.txt", "a+");
    if (file1 == NULL) {
        printf("Error opening file.");
        return;
    }
    fprintf(file1, "%s\n", userin);
    fclose(file1);
}

void remove_newline(char str[]) { // removes newline from fgets
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}

int formattingchk(char userin[], char *type) { // first formatting check for errant characters and such
    int offset = 0;
    int unknowncharlen = 0;
    int stlen = strlen(userin);
    while (1) {
        unknowncharlen = strspn(userin + offset, "1234567890*/()-+.");
        offset += unknowncharlen;
        if (offset >= stlen) {
            *type = '0';
            return 0;
        }
        if (userin[offset] == '\0') {
            return 0;
        } else if (((userin[offset] == 's' && userin[offset + 1] == 'i' && userin[offset + 2] == 'n') || 
                    (userin[offset] == 'c' && userin[offset + 1] == 'o' && userin[offset + 2] == 's') || 
                    (userin[offset] == 't' && userin[offset + 1] == 'a' && userin[offset + 2] == 'n') || 
                    (userin[offset] == 's' && userin[offset + 1] == 'e' && userin[offset + 2] == 'c') || 
                    (userin[offset] == 'c' && userin[offset + 1] == 'o' && userin[offset + 2] == 't') || 
                    (userin[offset] == 'c' && userin[offset + 1] == 's' && userin[offset + 2] == 'c') || 
                    (userin[offset] == 'l' && userin[offset + 1] == 'o' && userin[offset + 2] == 'g')) && 
                    userin[offset + 3] == '(') { 
            offset += 4;
        } else if (userin[offset] == '^' && userin[offset + 1] == '(') {
            offset += 2;
        } else if (userin[offset] == 'l' && userin[offset + 1] == 'n' && userin[offset + 2] == '(') {
            offset += 3;
        } else if (((userin[offset] == 'a' && userin[offset + 1] == 't' && userin[offset + 2] == 'a' && userin[offset + 3] == 'n') || 
                    (userin[offset] == 'a' && userin[offset + 1] == 's' && userin[offset + 2] == 'i' && userin[offset + 3] == 'n') || 
                    (userin[offset] == 'a' && userin[offset + 1] == 'c' && userin[offset + 2] == 'o' && userin[offset + 3] == 's') || 
                    (userin[offset] == 'c' && userin[offset + 1] == 'o' && userin[offset + 2] == 's' && userin[offset + 3] == 'h') || 
                    (userin[offset] == 's' && userin[offset + 1] == 'i' && userin[offset + 2] == 'n' && userin[offset + 3] == 'h') || 
                    (userin[offset] == 't' && userin[offset + 1] == 'a' && userin[offset + 2] == 'n' && userin[offset + 3] == 'h')) && 
                    userin[offset + 4] == '(') {
            offset += 5;
        } else {
            *type = userin[offset];
            return 1;
        }
    }
}

void exppro(char userin[], int openpar, int closepar, double val) { // handles when something is raised to the power of another
    char arr1[100];
    int arrstrt = openpar - 2;

    //printf("\nexpro loop\n");
    //printf("Initial userin: %s\n", userin);
    //printf("openpar: %d, closepar: %d, val: %Lf\n", openpar, closepar, val);

    // Find the starting point of the base number before the exponentiation
    while (arrstrt >= 0 && (isdigit(userin[arrstrt]) || userin[arrstrt] == '.' || userin[arrstrt] == '+' || userin[arrstrt] == '-' || userin[arrstrt] == '*' || userin[arrstrt] == '/')) {
        if (userin[arrstrt] == '+' || userin[arrstrt] == '-' || userin[arrstrt] == '*' || userin[arrstrt] == '/' || userin[arrstrt] == '(' || userin[arrstrt] == ')') {
            break;
        }
        arrstrt--;
    }
    arrstrt++;
	//printf("arrsrt is: %d", arrstrt);
    // Copy the base number into arr1
    strncpy(arr1, &userin[arrstrt], -1 +openpar - arrstrt);
    arr1[openpar - arrstrt] = '\0';

    //printf("Base number extracted: %s\n", arr1);

    // Convert the base number to long double
    double base = strntold(arr1);
    double result = pow(base, val);

    //printf("Result of pow(%Lf, %Lf): %Lf\n", base, val, result);

    // Convert the result back to a string
    char result_str[100];
    snprintf(result_str, sizeof(result_str), "%.10Lf", result);

    int result_len = strlen(result_str);
    int shift_len = closepar + 1 - arrstrt;

    //printf("Result string: %s, result_len: %d, shift_len: %d\n", result_str, result_len, shift_len);

    // Shift the rest of the userin string to make space for the result
    memmove(&userin[arrstrt + result_len], &userin[closepar + 1], strlen(&userin[closepar + 1]) + 1);
    // Copy the result string into userin
    strncpy(&userin[arrstrt], result_str, result_len);

    // Print debug information
    //printf("Updated userin with result: %s\n", userin);
    //printf("Finished exppro function\n");
}

void calculator(char userin[]) { // main calculator algorithm
	char anschk = '0';
    addtohis(userin);
    char *openparptr;
    char *closeparptr;
    int openpar;
    int closepar;

    while (anschk != '1') {
	//system("pause");
    int stl = strlen(userin);
    char *nullptr1 = &userin[0] + stl + 1;
    

        if (strcspn(userin, "(") != stl) { // Parentheses ARE present
            //printf("parentheses found\n"); // debug
            openparptr = strrchr(userin, '('); // pointer to the location of the opening parenthesis
            closeparptr = strpbrk(userin, ")"); // pointer to the location of the closing parenthesis
            openpar = openparptr - &userin[0]; // The location of the opening parenthesis
            closepar = closeparptr - &userin[0]; // The location of the closing parenthesis
            while (openpar > closepar) { // If location is not leftmost, iterate through this
                char temparr[100] = "";
                strncpy(temparr, userin, openpar);
                openparptr = strrchr(temparr, '(');
                openpar = openparptr - &temparr[0];
            }

            if (closepar > openpar) { // Parentheses are present and leftmost or innermost has been identified
                //printf("parentheses of interest identified\n");
				char innerarr[100] = "";
                strncpy(innerarr, &userin[openpar + 1], (closepar - openpar - 1));
                innerarr[closepar - openpar - 1] = '\0'; // make sure its null-terminated
                int stl2 = strlen(innerarr);
                //printf("inner array is now %s\n", innerarr); // innerarr now holds the array
                
                int digits = 0;
                int *d = &digits;

                if (strcspn(&innerarr[1], "+-*/") == stl2 - 1) { // operators not present (just a num)
                double val = strntold(innerarr);
                //printf("character found is: %c\n", userin[openpar - 1]);
                    if (userin[openpar - 1] == '^') { // if operator to left is exp then pow
                        exppro(userin, openpar, closepar, val);
                        continue;
                    } else if (userin[openpar - 1] == 'n' || userin[openpar - 1] == 's' || userin[openpar - 1] == 'h' || userin[openpar - 1] == 'g' || userin[openpar - 1] == 'c' || userin[openpar - 1] == 't') { // check for trig
                        double returnvalue;
                        int srtoftrig = (openpar >= 5) ? openpar - 5 : 0;
						//printf("trig start is: %d\n", openpar);
						int trig_len = 3;
                        // Check for specific trigonometric functions
                        if ((openpar >= 2) && strncmp(&userin[openpar - 3], "sin", 3) == 0) {
					        returnvalue = sin(val);
					    } else if ((openpar >= 2) && strncmp(&userin[openpar - 3], "cos", 3) == 0) {
					        returnvalue = cos(val);
					    } else if ((openpar >= 2) && strncmp(&userin[openpar - 3], "tan", 3) == 0) {
					        returnvalue = tan(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "asin", 4) == 0) {
					        returnvalue = asin(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "acos", 4) == 0) {
					        returnvalue = acos(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "atan", 4) == 0) {
					        returnvalue = atan(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "sinh", 4) == 0) {
					        returnvalue = sinh(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "cosh", 4) == 0) {
					        returnvalue = cosh(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "tanh", 4) == 0) {
					        returnvalue = tanh(val);
					    } else if ((openpar >= 2) && strncmp(&userin[openpar - 2], "ln", 2) == 0) {
					        returnvalue = log(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 3], "log", 3) == 0) {
					        returnvalue = log10(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "asec", 4) == 0) {
					        returnvalue = acos(1 / val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "acsc", 4) == 0) {
					        returnvalue = asin(1 / val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 4], "acot", 4) == 0) {
					        returnvalue = atan(1 / val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 3], "sec", 3) == 0) {
					        returnvalue = 1 / cos(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 3], "csc", 3) == 0) {
					        returnvalue = 1 / sin(val);
					    } else if ((openpar >= 3) && strncmp(&userin[openpar - 3], "cot", 3) == 0) {
					        returnvalue = 1 / tan(val);
                        }  else {
                            printf("No recognized trigonometric function found.\n");
                            continue;
                        }
                        //printf("Result of %.*s(%lf): %lf\n", openpar - srtoftrig, &userin[srtoftrig], val, returnvalue);
                        char result_str[20];
						snprintf(result_str, sizeof(result_str), "%.10Lf", returnvalue);
						
						int result_len = strlen(result_str);
                        int shift_len = closepar + 1 - (openpar - trig_len);

                        memmove(&userin[openpar - trig_len + result_len], &userin[closepar + 1], strlen(&userin[closepar + 1]) + 1);
                        strncpy(&userin[openpar - trig_len], result_str, result_len);
                        //printf("userin is now: %s", userin);
						continue;
                    } else {
                        printf("how did you even get this error\n");//function found but not identified in previous statement
                    }
                } else if (strcspn(userin, "+-*/") != stl) { // operators are present
                    double eval = mdas(innerarr, d);
                    char result_str[20];
                    snprintf(result_str, sizeof(result_str), "%.10Lf", eval);
                    
                    int result_len = strlen(result_str);
                    int shift_len = closepar + 1 - openpar - 1;
                    
                    memmove(&userin[openpar + result_len], &userin[closepar + 1], strlen(&userin[closepar + 1]) + 1);
                    strncpy(&userin[openpar], result_str, result_len);
                    
                    continue; // Continue to evaluate the modified expression
                } else {
                    //printf("failure in operator check loop\n");
                    return;
                }
                long double eval = mdas(innerarr, d);
                return;
            }
        } else if (strcspn(userin, "(") == stl) { // Parentheses not present
            if (strcspn(&userin[1], "+-/*sincotalg^") != stl - 1){ // operators still present
                // printf("parentheses not found\n"); // debug
                double eval;
                int digits = 0;
                int *d = &digits;
                eval = mdas(userin, d);
                // printf("%lf\n", eval);
                memset(userin, 0, 100);
                sprintf(userin, "%.10Lf", eval);
                //printf("%lf\n", eval);
                continue;
            } else {
                printf("Answer is %s\n", userin);
                anschk = '1';
                addtohis(userin);
                break;
            }
        }
    }

    return;
}

void accessHistory() { // takes care of accessing and clearing history
    printf("Displaying history now, type \"c\" to clear history or \"r\" to return to calculator.\n");
    FILE *file1 = fopen("./calculatorHistory.txt", "a+");
    if (file1 == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file1) != NULL) {
        printf("%s", line);
    }
    fclose(file1);

    char choice;
    while (1) {
        scanf(" %c", &choice);
        if (choice == 'r') {
            return;
        } else if (choice == 'c') {
            file1 = fopen("./calculatorHistory.txt", "w");
            fclose(file1);
            return;
        } else {
            printf("Unrecognized character, please try again.\n");
        }
        
    }
}

long double strntold(char strnum1[]) { // takes care of converting strings to numbers when fed only number characters
    //printf("\nstrntold loop\n");
    //printf("num passed is %s\n", strnum1);
    int radix = strcspn(strnum1, ".");
    int size = strlen(strnum1);
    double num = 0;
    int numuq = 0;  
    double radixfactor;
    int sign = 1;

    int i = 0;

    // Check for negative sign at the beginning
    if (strnum1[0] == '-') {
        sign = -1;
        i++;
    }

    for (; i < size; i++) {
        if (i < radix) {
            numuq = strnum1[i] - '0';
            radixfactor = pow(10, (0 - 1 + radix - i));  
            num = num + numuq * radixfactor;
            //printf("num uq is: %d\nnum is now: %Lf\nnum added is: %Lf\n radix factor is: %f\n\n", numuq, num, (numuq * radixfactor), radixfactor);
        } else if (i == radix) {
            continue;
        } else if (i > radix) {
            numuq = strnum1[i] - '0';
            radixfactor = pow(10, (radix - i));  
            num = num + (numuq * radixfactor);
            //printf("num uq is: %d\nnum is now: %Lf\nnum added is: %Lf\n radix factor is: %f\n\n", numuq, num, (numuq * radixfactor), radixfactor);
        } else {
            printf("unrecognized character in strntold\n");
        }
    }

    num *= sign;
    //printf("returned value: %Lf\n", num);
    return num;
}

int opformattingchk(char userin[]) { // This functions makes sure no strange lookin operators are present
    int closep = 0;
    int openp = 0;
    int size = strlen(userin);
    int i; 
    for (i = 0; i < size; i++) {
        if (userin[i] == '(') {
            openp++;
        }
        if (userin[i] == ')') {
            closep++;
        }
        if (userin[i] == '(' && userin[i + 1] == ')') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (userin[i] == ')' && userin[i + 1] == '(') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (userin[i] == '*' && userin[i + 1] == '/') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (userin[i] == '/' && userin[i + 1] == '*') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (userin[i] == '-' && userin[i + 1] == '+') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (userin[0] == '.') {
            printf("forbidden sequence\n");
            return 1;
        }
        if (i != 0 && userin[i] == '.' && (!isdigit(userin[i - 1]) || !isdigit(userin[i + 1]))) {
            printf("forbidden sequence\n");
            return 1;
        }
    }

    if (openp != closep) {
        printf("you are missing at least one parenthesis\n");
        return 1;
    }
    return 0;
}

int main() {
    printf("Welcome to the calculator. Type in your problem followed by the enter key.\nTo access history press 'h'\n");
    char type;
    char *t = &type;
    while (1) {
    	printf("Problem: ");
        char userin[100] = "";
        fgets(userin, 100, stdin);
        remove_newline(userin);
        if ((userin[0] == 'h' && userin[1] == '\0') || (userin[0] == 'H' && userin[1] == '\0')) {
            accessHistory();
        } else if (formattingchk(userin, t) != 0) {
            printf("An unrecognized character '%c' was detected\n", type);
            printf("Formatting tips\n\t1) All trig functions must have a parenthesis immediately following them\n\t2) All exponent operators \"^\" must be followed by a parenthesis\n\t3) All statements are evaluated according to the PEmdas rules from left to right\n");
        } else if (opformattingchk(userin) != 0) {
            printf("It appears there are some issues with the way you are formatting your operators. Look for these mistakes:\n\t1) An opening parenthesis is followed by a closing parenthesis with no operator\n\t2) A division and a multiplication operator are next to each other\n\t3) You missed an opening or closing parenthesis\n\t4) You did not put numbers around a decimal point\n");
        } else {
            calculator(userin);
        }
    }
    return 0;
}

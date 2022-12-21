/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <string.h>
#include <stdio.h>
#include "cal.h"

#define DIGIT 0
#define OPERATOR 1
#define ERROR -1


int isOperator(char op) {
    
    if(op=='^'||op=='*'||op=='/'||op=='+'||op=='-') 
        return 1;
    
    if(op>='0'&&op<='9') 
        return 0;

    else return -1;
}
int cal_pow(int base, int exp) {
    int result = 1;
    while (1)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}
int precedence(char op) {
	switch(op) {
		case '^':
			return 3;
		case '*':
		case '/':
			return 2;
		case '+':
		case '-':
			return 1;
		default:
			printf("this is not operator!\n");
			return 0;
	}
}
int calculate(int a, int b, char op) {

    if(op=='^') return cal_pow(a,b);
    else if (op=='*') return a*b;
    else if(op=='/'){
        if(b==0) {
            printf("cannot divide by zero...\n");
            exit(-1);
        }else return a/b;
    }
    else if(op=='+') return a+b;
    else if(op=='-') return a-b;
}

int *
calculate_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result;
	char* input = *argp;

	int i=0, n=0, len=0;
	int operand[50];
	char operator[50], tmp[50];
	
    int j=-1, k=-1; 
	int num1=0,num2=0;
    char op;
    
    for(i=0; i<strlen(input);) {
		if (input[i] == '*' && input[i+1] == '*') {
			tmp[len++] = '^';
			i += 2;
		}
		else {
			tmp[len++] = input[i++];
		}
	}
	tmp[len] = '\0';
	printf("input : %s\n", tmp);
	
	for (i=0; i<len; i++)
    {	
		int val = 0;
        int res=isOperator(tmp[n]);

        switch(res){
            case 0:
                while(!isOperator(tmp[n])&&n<len)
                {
                    val= (val*10)+(tmp[n]-'0');
                    n++;
                }
                operand[++j]=val;
                break;
            case 1:
                while(k != -1&&precedence(operator[k]) >= precedence(tmp[n]))
                {
                    num1=operand[j--];
                    num2=operand[j--];
                    op=operator[k--];
                    operand[++j]=calculate(num2,num1,op);
                }
                operator[++k]=tmp[n++];
        }
    }

	while(k != -1)
	{
		num1 = operand[j--];
		num2 = operand[j--];
		op = operator[k--];
		operand[++j] = calculate(num2, num1, op);
	}

	result = operand[j];
	return &result;
}


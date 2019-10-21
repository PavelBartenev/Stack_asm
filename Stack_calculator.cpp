#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef int type;

#include <D:\VS projects\Stack_calculator\stack_typedef.h.txt>

int* Assembler(FILE*);
FILE* Disassembler(FILE*);
FILE* CPU(int*, struct stack_t*);

enum Commands
{
	CMD_END = 0,
	CMD_PUSH = 1,
	CMD_POP = 2,
	CMD_ADD = 3,
	CMD_SUB = 4,
	CMD_MUL = 5,
	CMD_DIV = 6,
	CMD_PUSH_ax = 7,
	CMD_PUSH_bx = 8,
	CMD_PUSH_cx = 9,
	CMD_POP_ax = 10,
	CMD_POP_bx = 11,
	CMD_POP_cx = 12
};

int main()
{
	FILE* input = fopen("input.txt", "r");

	stack_t* stk1 = StackConstruct();

	CPU(Assembler(input), stk1);


	return 0;
}

int* Assembler(FILE* input)
{
	FILE* output = fopen("asm_output.txt", "a+");

	int end_of_file = 0;
	int num_of_command = 0;
	int* commands = (int*) calloc(20, sizeof(int));

	while (!end_of_file)
	{
		char str[10] = "";
		fscanf(input, "%s", str);

		if (strcmp(str, "push") == 0)
		{
			fprintf(output, "%d ", CMD_PUSH);
			commands[num_of_command] = CMD_PUSH;
			num_of_command++;
			type elem = 0;
			fscanf(input, "%d", &elem);
			fprintf(output, "%d\n", elem);
			commands[num_of_command] = elem;
		}

		if (strcmp(str, "add") == 0)
		{
			fprintf(output, "%d\n", CMD_ADD);
			commands[num_of_command] = CMD_ADD;
		}

		if (strcmp(str, "sub") == 0)
		{
			fprintf(output, "%d\n", CMD_SUB);
			commands[num_of_command] = CMD_SUB;
		}

		if (strcmp(str, "mul") == 0)
		{
			fprintf(output, "%d\n", CMD_MUL);
			commands[num_of_command] = CMD_MUL;
		}

		if (strcmp(str, "div") == 0)
		{
			fprintf(output, "%d\n", CMD_DIV);
			commands[num_of_command] = CMD_DIV;
		}

		if (strcmp(str, "pop") == 0)
		{
			fprintf(output, "%d\n", CMD_POP);
			commands[num_of_command] = CMD_POP;
		}

		if (strcmp(str, "push_ax") == 0)
		{
			fprintf(output, "%d\n", CMD_PUSH_ax);
			commands[num_of_command] = CMD_PUSH_ax;
		}

		if (strcmp(str, "push_bx") == 0)
		{
			fprintf(output, "%d\n", CMD_PUSH_bx);
			commands[num_of_command] = CMD_PUSH_bx;
		}

		if (strcmp(str, "push_cx") == 0)
		{
			fprintf(output, "%d\n", CMD_PUSH_cx);
			commands[num_of_command] = CMD_PUSH_cx;
		}

		if (strcmp(str, "pop_ax") == 0)
		{
			fprintf(output, "%d\n", CMD_POP_ax);
			commands[num_of_command] = CMD_POP_ax;
		}

		if (strcmp(str, "pop_bx") == 0)
		{
			fprintf(output, "%d\n", CMD_POP_bx);
			commands[num_of_command] = CMD_POP_bx;
		}

		if (strcmp(str, "pop_cx") == 0)
		{
			fprintf(output, "%d\n", CMD_POP_cx);
			commands[num_of_command] = CMD_POP_cx;
		}

		if (strcmp(str, "end") == 0)
		{
			end_of_file = 1;
			fprintf(output, "%d\n", CMD_END);
			commands[num_of_command] = CMD_END;
		}

		num_of_command++;
	}

	fseek(output, 0, SEEK_SET);

	return commands;
}

FILE* Disassembler(FILE* input)
{
	FILE* output = fopen("disasm_output.txt", "w");

	int end_of_file = 0;

	while (!end_of_file)
	{
		type elem = 0;
		int command = -1;
		fscanf(input, "%d", &command);

		if (command == CMD_PUSH)
		{
			fprintf(output, "%s ", "push");
			fscanf(input, "%d", &elem);
			fprintf(output, "%d\n", elem);
		}

		if (command == CMD_ADD)
			fprintf(output, "%s\n", "add");

		if (command == CMD_SUB)
			fprintf(output, "%s\n", "sub");

		if (command == CMD_MUL)
			fprintf(output, "%s\n", "mul");

		if (command == CMD_DIV)
			fprintf(output, "%s\n", "div");

		if (command == CMD_POP)
			fprintf(output, "%s\n", "pop");

		if (command == CMD_PUSH_ax)
			fprintf(output, "%s\n", "push_ax");

		if (command == CMD_PUSH_bx)
			fprintf(output, "%s\n", "push_bx");

		if (command == CMD_PUSH_cx)
			fprintf(output, "%s\n", "push_cx");

		if (command == CMD_POP_ax)
			fprintf(output, "%s\n", "pop_ax");

		if (command == CMD_POP_bx)
			fprintf(output, "%s\n", "pop_bx");

		if (command == CMD_POP_cx)
			fprintf(output, "%s\n", "pop_cx");

		if (command == CMD_END)
		{
			end_of_file = 1;
			fprintf(output, "%s\n", "end");
		}
	}

	return output;
}

FILE* CPU(int* commands, struct stack_t* stk)
{
	FILE* output = fopen("answer.txt", "w");

	int end_of_file = 0;
	int num_of_command = 0;
	type answer = 0;
	while (!end_of_file)
	{
		int command = -1;
		type elem = 0;

		command = commands[num_of_command];

		if (command == CMD_PUSH)
		{
			num_of_command++;
			elem = commands[num_of_command];
			StackPush(stk, elem);
		}

		if (command == CMD_ADD)
		{
			StackPop(stk, &elem);
			type result = elem;
			StackPop(stk, &elem);
			result += elem;
			StackPush(stk, result);
		}

		if (command == CMD_SUB)
		{
			StackPop(stk, &elem);
			type result = -elem;
			StackPop(stk, &elem);
			result += elem;
			StackPush(stk, result);
		}

		if (command == CMD_MUL)
		{
			StackPop(stk, &elem);
			type result = elem;
			StackPop(stk, &elem);
			result *= elem;
			StackPush(stk, result);
		}

		if (command == CMD_DIV)
		{
			StackPop(stk, &elem);
			type result = elem;
			StackPop(stk, &elem);
			result = result/elem;
			StackPush(stk, result);
		}

		if (command == CMD_POP_ax)
		{
			StackPop(stk, &elem);
			stk->ax = elem;
		}

		if (command == CMD_POP_bx)
		{
			StackPop(stk, &elem);
			stk->bx = elem;
		}

		if (command == CMD_POP_cx)
		{
			StackPop(stk, &elem);
			stk->cx = elem;
		}

		if (command == CMD_PUSH_ax)
		{
			elem = stk->ax;
			StackPush(stk, elem);
			stk->ax = 0;
		}

		if (command == CMD_PUSH_bx)
		{
			elem = stk->bx;
			StackPush(stk, elem);
			stk->bx = 0;
		}

		if (command == CMD_PUSH_cx)
		{
			elem = stk->cx;
			StackPush(stk, elem);
			stk->cx = 0;
		}

		if (command == CMD_END)
		{
			end_of_file = 1;
		}

		num_of_command++;
	}

	StackPop(stk, &answer);
	fprintf(output, "%d", answer);

	return output;
}

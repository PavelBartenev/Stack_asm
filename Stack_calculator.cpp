#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>

typedef int type;

#include <D:\VS projects\Stack_asm\stack_typedef.h.txt>

int* Assembler(FILE*);
FILE* Disassembler(FILE*);
FILE* CPU(struct stack_t*);

const int MAX_NUMBER_OF_COMMANDS = 100;
const int MAX_NUMBER_OF_LABELS = 50;
const int MAX_LENGHT_OF_COMMAND = 10;

enum Commands
{
	CMD_END,
	CMD_PUSH,
	CMD_POP,
	CMD_ADD,
	CMD_SUB,
	CMD_MUL,
	CMD_DIV,
	CMD_PUSH_ax,
	CMD_PUSH_bx,
	CMD_PUSH_cx,
	CMD_PUSH_j1x,
	CMD_PUSH_j2x,
	CMD_POP_ax,
	CMD_POP_bx,
	CMD_POP_cx,
	CMD_POP_j1x,
	CMD_POP_j2x,
	CMD_JMP,
	CMD_JA,
	CMD_JB,
	CMD_IN_ax,
	CMD_IN_bx,
	CMD_IN_cx,
	CMD_OUT_ax,
	CMD_OUT_bx,
	CMD_OUT_cx,
	CMD_OUT,
	CMD_FOUT,
};

int main()
{
	FILE* input = fopen("input.txt", "r");

	stack_t* stk1 = StackConstruct();

	Assembler(input);

	CPU(stk1);

	return 0;
}


int* Assembler(FILE* input)
{
	FILE* output = fopen("asm_output.txt", "w");

	int end_of_file = 0;
	int num_of_command = 0;
	int* commands = (int*)calloc(MAX_NUMBER_OF_COMMANDS, sizeof(int));
	char labels[MAX_LENGHT_OF_COMMAND*MAX_NUMBER_OF_LABELS] = {};
	int labels_pointers[2*MAX_NUMBER_OF_LABELS] = {};
	int labels_pos = 0;
	int labels_point_pos = 0;

	while (end_of_file != 2)
	{
		char str[MAX_LENGHT_OF_COMMAND] = {};
		char label[MAX_LENGHT_OF_COMMAND] = {};

		fscanf(input, "%s", str);

		if (strcmp(str, "push") == 0)
		{
			commands[num_of_command] = CMD_PUSH;
			num_of_command++;
			int elem = 0;
			fscanf(input, "%d", &elem);
			commands[num_of_command] = elem;
		}

		if (str[0] == 'j')
		{
			if (strcmp(str, "jmp") == 0)
				commands[num_of_command] = CMD_JMP;

			if (strcmp(str, "ja") == 0)
				commands[num_of_command] = CMD_JA;

			if (strcmp(str, "jb") == 0)
				commands[num_of_command] = CMD_JB;

			num_of_command++;
			fscanf(input, "%s", label);

			char* pointer = strstr(labels, label);

			for (int i = 0; i < 20; i += 2)
			{
				if (labels_pointers[i] == (int)pointer)
				{
					commands[num_of_command] = labels_pointers[i + 1];
					break;
				}
			}
		}

		if (str[0] == ':')
		{
			fscanf(input, "%s", label);
			strncat(labels, label, MAX_LENGHT_OF_COMMAND);
			labels_pointers[labels_point_pos] = ((int)labels) + labels_pos;
			labels_point_pos++;
			labels_pointers[labels_point_pos] = num_of_command;
			labels_point_pos++;
			labels_pos += strlen(label);
			num_of_command--;
		}

		if (strcmp(str, "add") == 0)
			commands[num_of_command] = CMD_ADD;
		

		if (strcmp(str, "sub") == 0)
			commands[num_of_command] = CMD_SUB;

		if (strcmp(str, "mul") == 0)
			commands[num_of_command] = CMD_MUL;

		if (strcmp(str, "div") == 0)
			commands[num_of_command] = CMD_DIV;

		if (strcmp(str, "pop") == 0)
			commands[num_of_command] = CMD_POP;

		if (strcmp(str, "push_ax") == 0)
			commands[num_of_command] = CMD_PUSH_ax;

		if (strcmp(str, "push_bx") == 0)
			commands[num_of_command] = CMD_PUSH_bx;

		if (strcmp(str, "push_cx") == 0)
			commands[num_of_command] = CMD_PUSH_cx;

		if (strcmp(str, "pop_ax") == 0)
			commands[num_of_command] = CMD_POP_ax;

		if (strcmp(str, "pop_bx") == 0)
			commands[num_of_command] = CMD_POP_bx;

		if (strcmp(str, "pop_cx") == 0)
			commands[num_of_command] = CMD_POP_cx;

		if (strcmp(str, "in_ax") == 0)
			commands[num_of_command] = CMD_IN_ax;

		if (strcmp(str, "in_bx") == 0)
			commands[num_of_command] = CMD_IN_bx;

		if (strcmp(str, "in_cx") == 0)
			commands[num_of_command] = CMD_IN_cx;

		if (strcmp(str, "out_ax") == 0)
			commands[num_of_command] = CMD_OUT_ax;

		if (strcmp(str, "out_bx") == 0)
			commands[num_of_command] = CMD_OUT_bx;

		if (strcmp(str, "out_cx") == 0)
			commands[num_of_command] = CMD_OUT_cx;

		if (strcmp(str, "out") == 0)
			commands[num_of_command] = CMD_OUT;

		if (strcmp(str, "fout") == 0)
			commands[num_of_command] = CMD_FOUT;

		if (strcmp(str, "end") == 0)
		{
			end_of_file++;
			commands[num_of_command] = CMD_END;

			if (end_of_file == 1)
			{
				fseek(output, 0, SEEK_SET);
				fseek(input, 0, SEEK_SET);
				num_of_command = -1;
			}
		}

		num_of_command++;
	}

	fprintf(output, "%d ", num_of_command);

	for (int i = 0; i < num_of_command; i++)
	{
		fprintf(output, "%d ", commands[i]);
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

		if (command == CMD_IN_ax)
			fprintf(output, "%s\n", "in_ax");

		if (command == CMD_IN_bx)
			fprintf(output, "%s\n", "in_bx");

		if (command == CMD_IN_cx)
			fprintf(output, "%s\n", "in_cx");

		if (command == CMD_END)
		{
			end_of_file = 1;
			fprintf(output, "%s\n", "end");
		}
	}

	return output;
}

FILE* CPU(struct stack_t* stk)
{
	FILE* input = fopen("asm_output.txt", "r");
	FILE* output = fopen("cpu_output.txt", "w");
	
	int count_of_commands = 0;

	fscanf(input, "%d", &count_of_commands);

	int* commands = (int*)calloc(count_of_commands, sizeof(int));

	for (int i = 0; i < count_of_commands; i++)
	{
		fscanf(input, "%d", commands+i);
	}

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

		if (command == CMD_POP)
		{
			type elem = 0;
			StackPop(stk, &elem);
		}

		if (command == CMD_JMP)
		{
			num_of_command++;
			num_of_command = commands[num_of_command] - 1;
		}

		if ((command == CMD_JA) || (command == CMD_JB))
		{
			type elem1 = 0;
			type elem2 = 0;
			StackPop(stk, &elem2);
			StackPop(stk, &elem1);

			if (((elem1 > elem2) && (command == CMD_JA)) || ((elem1 < elem2) && (command == CMD_JB)))
			{
				num_of_command++;
				num_of_command = commands[num_of_command] - 1;
			}
			else
				num_of_command++;

			StackPush(stk, elem1);
			StackPush(stk, elem2);
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
			result = result / elem;
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
		}

		if (command == CMD_PUSH_bx)
		{
			elem = stk->bx;
			StackPush(stk, elem);
		}

		if (command == CMD_PUSH_cx)
		{
			elem = stk->cx;
			StackPush(stk, elem);
		}

		if (command == CMD_IN_ax)
		{
			printf("Enter a value for the register ax: ");
			scanf("%d", &(stk->ax));
		}

		if (command == CMD_IN_bx)
		{
			printf("Enter a value for the register bx: ");
			scanf("%d", &(stk->bx));
		}

		if (command == CMD_IN_cx)
		{
			printf("Enter a value for the register cx: ");
			scanf("%d", &(stk->cx));
		}

		if (command == CMD_OUT_ax)
			printf("%d\n", stk->ax);

		if (command == CMD_OUT_bx)
			printf("%d\n", stk->bx);

		if (command == CMD_OUT_cx)
			printf("%d\n", stk->cx);

		if (command == CMD_OUT)
		{
			type elem = 0;
			StackPop(stk, &elem);
			printf("%d\n", elem);
			StackPush(stk, elem);
		}

		if (command == CMD_FOUT)
		{
			type elem = 0;
			StackPop(stk, &elem);
			fprintf(output, "%d\n", elem);
			StackPush(stk, elem);
		}

		if (command == CMD_END)
			end_of_file = 1;

		num_of_command++;
	}

	return output;
}



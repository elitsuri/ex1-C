/*
* Author: Eliyho Tsuri - 201610672
*
* Description : the program open a text file with numbers,
*			     transfer all the data on the text file,
*               and return the the biggest line number.
*/
// -------------------------- Include -----------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// --------------------------- Struct ------------------------------
struct Data {
	int _num_of_lines;
	int** _the_data;
	int* _lines_len;
};
// --------------------------- Define ------------------------------
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
// ------------------------ Prototypes -----------------------------
void build_struct(FILE* inp, struct Data* my_data);
void build_row(struct Data* my_data);
void build_col(struct Data* my_data, int row, int row_len);
void transfer(FILE* inp, struct Data* my_data, int row);
int find_max_row(struct Data* my_data);
bool check_max(const struct Data* my_data, int row, int col);
void free_empty(struct Data* my_data, int row);
// -------------------------- Int Main -----------------------------
int main(int argc, char* argv[])
{
	FILE* inp;					  // input file
	struct Data my_data;	      // struct to save the data
	my_data._the_data = NULL;	  // initializing the array[][]
	int the_max_row;			  // to print the max line
	inp = fopen(argv[1], "r");   // open file
								  // check if the file is open
	if (inp == NULL)
	{
		printf("Error in opening file's");
		exit(EXIT_FAILURE);
	}

	// build and transfer all data from input file
	build_struct(inp, &my_data);

	fclose(inp);				  // close the input file
								  // for print the max line
	the_max_row = find_max_row(&my_data);
	printf("%d", the_max_row);		  // print function
	free_empty(&my_data, 0);     // delete all struct
	exit(EXIT_SUCCESS);		  // end the program
}
// ------------------------ build_struct ---------------------------
/*
* This function get the input file and the struct data.
* the function build the struct and transfer all the numbers.
*/
void build_struct(FILE* inp, struct Data* my_data)
{
	int number;
	int row = 0;
	my_data->_num_of_lines = 0;

	while (fscanf(inp, "%d", &number) != EOF)
	{
		build_row(my_data);
		build_col(my_data, row, number);
		transfer(inp, my_data, row);
		row++;
		free_empty(my_data, row);
	}
}
// ------------------------- build_row -----------------------------
/*
* This function called by build struct
* the function get the struct data and build the row/s on the
* member the data, and updating the member num of lines
*/
void build_row(struct Data* my_data)
{
	if (my_data->_num_of_lines == 0)
	{
		my_data->_num_of_lines++;
		my_data->_the_data = (int**)malloc(my_data->_num_of_lines * sizeof(int));
		my_data->_lines_len = (int*)malloc(my_data->_num_of_lines * sizeof(int));
	}
	else
	{
		my_data->_num_of_lines *= 2;
		my_data->_the_data = (int**)realloc(my_data->_the_data, my_data->_num_of_lines * sizeof(int*));
		my_data->_lines_len = (int*)realloc(my_data->_lines_len, my_data->_num_of_lines * sizeof(int));
	}
	if (my_data->_the_data == NULL)
	{
		printf("Error in opening file's");
		exit(EXIT_FAILURE);
	}
}
// ------------------------- build_col -----------------------------
/*
* This function called by build row and build the[row],[row_len]
* the function get the struct, the number row, and the length of the line
*/
void build_col(struct Data* my_data, int row, int row_len)
{
	my_data->_the_data[row] = (int*)malloc(row_len * sizeof(int));
	if (my_data->_the_data[row] == NULL)
	{
		printf("Error in opening file's");
		exit(EXIT_FAILURE);
	}
	my_data->_lines_len[row] = row_len;
}
// ------------------------- transfer ------------------------------
/*
* This function get the input file, the struct data and
* row the function transfer all the numbers in any row
* to the struct instead of the [row] that was sent.
*/
void transfer(FILE* inp, struct Data* my_data, int row)
{
	int number;
	int col = 0;

	while (fgetc(inp) != '\n')
	{
		fscanf(inp, "%d", &number);
		my_data->_the_data[row][col] = number;
		col++;
	}
}
// ------------------------ free_empty -----------------------------
/*
* This function get the struct data and the row (row used)
* the function delete all unused rows
*/
void free_empty(struct Data* my_data, int row)
{
	while (row < my_data->_num_of_lines)
	{
		row++;
		free(my_data->_the_data[row]);
		my_data->_num_of_lines--;
	}
}
// ------------------------ find_max_row ---------------------------
/* find max row the main function that search for the max row and return
* the max row number (if there is no max row it returns -1.
*/
int find_max_row(struct Data* my_data)
{
	int row, col,
		max_row = -1;
	bool found = false;
	for (row = 0; row < my_data->_num_of_lines; row++)
	{
		for (col = 0; col < my_data->_lines_len[row]; col++)
		{
			if (!check_max(my_data, row, col))
				break;
			found = true;
		}
		if (found)
		{
			max_row = row;
			break;
		}
	}
	return (max_row);
}
// ------------------------- check_max -----------------------------
/*
* This function get the struct data, number row, number col, and looking for
* the max tow, the function return if the the row sent to the function return
* true if the maximum line that all the column content in its row is larger
* than the column value in all other rows, otherwise it will return a false.
*/
bool check_max(const struct Data* my_data, int row, int col)
{
	int num = my_data->_the_data[row][col];
	int num_line = my_data->_num_of_lines;

	while (row < num_line)
	{
		int num2 = my_data->_the_data[row][col];

		if (my_data->_lines_len[row] >= col)
			if (num < num2)
				return false;
		row++;
	}
	return true;
}
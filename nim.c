#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "nim.h"

int *new_board( int board_size ){
    int *array = malloc(sizeof(int) * board_size);
    if (array == NULL)
    {
        fprintf(stderr, "Malloc Failed.\n");
        exit(-1);
    }
    return array;
}

struct node *mk_nim_hash( int max_hash, int board_size, int *start_board )
{
    struct node *tempArray = malloc(sizeof(struct node) * max_hash);

    if (tempArray == NULL)
    {
        fprintf(stderr, "Malloc failed in mk_nim_hash\n");
        exit(-1);
    }
    for (int i = 0; i < max_hash; i++)
    {
        tempArray[i].moves = -1;
        tempArray[i].move = NULL;
        tempArray[i].nimsum = -1;
        tempArray[i].board = hash2board(board_size, start_board, i);
    }

    return tempArray;
}

void free_board( int *board ){
    free(board);
}

void free_nim_hash( int max_hash, struct node *nim_hash ) {
    for (int i = 0; i < max_hash; i++) {
        free (nim_hash[i].move);
        free (nim_hash[i].board);
    }
    free (nim_hash);
}

int *board_from_argv( int board_size, char **argv ){
    int *new_board = malloc (sizeof (int) * board_size);
    for (int i = 0; i < board_size; i++) {
        new_board[i] = atoi(argv[i]);
    }

    return new_board;
}

int *copy_board( int board_size, int *board ){
    int * nBoard = new_board(board_size);
    for (int i = 0; i < board_size; i++)
    {
        nBoard[i] = board[i];
    }
    return nBoard;
}

int game_over( int board_size, int *board ){
    int sum = 0;

    for (int i = 0; i < board_size; i++) {
        sum += board[i];    
    }
    if (sum == 1) {
        return 1;
    }
    return 0;
}

int compute_nimsum( int board_size, int *board ){
    int nimsum = board[0];
    int boolean =0;
    if (board[0] > 1)
    {
        boolean = 1;
    }
    for (int i = 0; i < board_size; i++)
    {
        if (board[i] > 1)
        {
            boolean = 1;
        }
        nimsum ^= board[i];
        
    }
    if (boolean == 1)
    {
        return nimsum;
    }
    else
        return !(nimsum);
}

void join_graph( struct node *nim_hash, int hash, int board_size, int *start_board ){

     if (nim_hash[hash].moves != -1) {
        return;
    }

    int moves = 0;

    for (int i = 0; i < board_size; i++) {
        if (nim_hash[hash].board[i] > 0) {
            moves += nim_hash[hash].board[i];
        }
    }

    nim_hash[hash].moves = moves;
    nim_hash[hash].move = malloc (sizeof (struct move) * moves);

    int count = 0;

    for (int i = 0; i < board_size; ++i) {
        for (int j = 1; j < nim_hash[hash].board[i] + 1; ++j) {
            nim_hash[hash].move[count].row = i;

            nim_hash[hash].move[count].matches = j;

            int *tempBoard = copy_board(board_size, nim_hash[hash].board);

            tempBoard[i] -= j;
            nim_hash[hash].move[count].hash = board2hash(board_size, start_board, tempBoard);
            free(tempBoard);
            nim_hash[hash].nimsum = compute_nimsum(board_size, nim_hash[hash].board); // numsum
            join_graph(nim_hash, nim_hash[hash].move[count++].hash, board_size, start_board);
        }
    }
    
}


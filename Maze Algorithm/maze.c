#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "maze.h"

Maze* _read_maze(FILE* fp);
Maze* _malloc_maze(int num_rows, int num_cols);
void _free_maze(Maze* maze);
bool _write_maze(const char* filename, const Maze* maze);
void _find_maze_dimensions(FILE * fp, int * num_rows, int * num_cols);

Maze* _malloc_maze(int num_rows, int num_cols){
    
    
    Maze* maze = malloc(sizeof(Maze));
    if (maze == NULL) {
        free(maze);
    }
    
    maze -> maze_array = malloc(sizeof(char*) * num_rows);
    if(maze -> maze_array == NULL) {
        free(maze -> maze_array);
        // return NULL;
    }
    
    for(int i =0 ; i < num_rows;i++){
        (*maze).maze_array[i] = malloc(sizeof(char) * num_cols);
        if((*maze).maze_array[i] == NULL){
            free((*maze).maze_array[i]);
        }
    }
    
    (*maze).n_row = num_rows;
    (*maze).n_col = num_cols;
    
    return maze;
}

void _find_maze_dimensions(FILE * fp, int * num_rows, int * num_cols) {
    *num_rows = *num_cols = 0;
    int ct = 0;
    int ct1 = 1;
    int j = 0;
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    for(int i=0; i < size; i++){
        fseek(fp, i, SEEK_SET);
        if(fgetc(fp) == '\n'){
            ct++;
        }
    }
    *num_rows = ct;
    
    fseek(fp, 0, SEEK_SET);
    
    while(fgetc(fp) != '\n'){
        fseek(fp, ++j, SEEK_SET);
        ct1++;
    }
    
    *num_cols = ct1;
    
    return;
}


bool _write_maze(const char* filename, const Maze* maze){
    FILE * fp = fopen(filename, "w");
    
    if(fp == NULL){
        return false;
    }
    
    int ct = 0;
    
    for(int i = 0; i < maze -> n_row; i++){
        for(int j = 0; j < maze -> n_col; j++){
            ct  = (maze -> n_col * i) + j;
            fseek(fp, ct, SEEK_SET);
            
            fputc(maze -> maze_array[i][j], fp);
        }
    }
    
    fclose(fp);
    
    return true;
}

void _free_maze(Maze* maze){
    for(int i=0; i< maze->n_row; i++){
        
        free(maze -> maze_array[i]);
    }
    
    free(maze -> maze_array);
    
    free(maze);
    
    return;
}

Maze* _read_maze(FILE* fp){
    int rows = 0;
    int cols = 0;
    
    _find_maze_dimensions(fp, &rows, &cols);
    
    Maze* maze = _malloc_maze(rows, cols);
    
    for(int i = 0; i < rows; i++){
        for(int j = 0; j <= cols; j++){
            int ct  = (cols * i) + j;
            fseek(fp, ct, SEEK_SET);
            maze->maze_array[i][j] = fgetc(fp);
        }
    }
    
    maze->n_row = rows;
    maze->n_col = cols;
    
    
    return maze;
}


static bool is_path(Maze *maze, int row, int col){
    if ((row >= 0) && (row < maze->n_row)) {
        if ((col >= 0) && (col < maze->n_col)) {
            return maze->maze_array[row][col] == PATH;
        }
    }
    return false;
}

/* given a maze, current row (curr),                                     */
/* current column (curc), destination row (drow), destination col (dcol) */
/* current distance from source, determine the where to explore from the */
/* current location                                                      */
/* a recursive function: from the current location, try 'N', 'S', 'E',   */
/* 'W' locations one at a time if the attempt is successful, write the   */
/* direction in the file                                                 */

static int _pathfinder_helper(Maze *maze, int curr, int curc, int endr, int endc,
                              int count, FILE *dfptr)
{
    // you may uncomment this and use the output here to trace the function
    // with sample5.pdf
    // printf("(%d, %d), %d\n", curr, curc, count);
    maze->maze_array[curr][curc] = VISITED;
    if ((curr == endr) && (curc == endc)) { // reach the destination
        int i;
        for (i = 0; i < count; i++) { // create enough space in file
            fputc(VISITED, dfptr);
        }
        return count;
    }
    
    //int found;
    int nF = -1;
    int eF = -1;
    int wF = -1;
    int sF = -1;
    if (is_path(maze, curr+1, curc)) {
        sF = _pathfinder_helper(maze, curr+1, curc, endr, endc, count+1, dfptr);
    }
//        if (found != -1) {
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            fputc('N', dfptr);
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            return found;
//        }
//    }
    if (is_path(maze, curr-1, curc)) {
        nF = _pathfinder_helper(maze, curr-1, curc, endr, endc, count+1, dfptr);
    }
//        if (found != -1) {
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            fputc('S', dfptr);
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            return found;
//        }
//    }
    if (is_path(maze, curr, curc-1)) {
        wF = _pathfinder_helper(maze, curr, curc-1, endr, endc, count+1, dfptr);
    }
//        if (found != -1) {
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            fputc('E', dfptr);
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            return found;
//        }
//    }
    if (is_path(maze, curr, curc+1)) {
        eF = _pathfinder_helper(maze, curr, curc+1, endr, endc, count+1, dfptr);
    }
//        if (found != -1) {
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            fputc('W', dfptr);
//            fseek(dfptr, -1, SEEK_CUR); // go back one position
//            return found;
//        }
//    }
    
    int size = 4;
    int array[4] = {sF, nF, wF, eF};
    int rdm = INT_MAX;
    
    for(int ct = 0; ct< size;ct++){
        if(array[ct] < rdm){
            rdm = array[ct];
        }
    }
    
    maze->maze_array[curr][curc] = PATH;
    
    if (is_path(maze, curr, curc+1)) {
        eF = _pathfinder_helper(maze, curr, curc+1, endr, endc, count+1, dfptr);
        if (eF != -1) {
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          fputc('W', dfptr);
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          return eF;
        }
    }
    
    if (is_path(maze, curr, curc-1)) {
        wF = _pathfinder_helper(maze, curr, curc-1, endr, endc, count+1, dfptr);
            if (wF != -1) {
                fseek(dfptr, -1, SEEK_CUR); // go back one position
                fputc('E', dfptr);
                fseek(dfptr, -1, SEEK_CUR); // go back one position
                return wF;
            }
        }

    if (is_path(maze, curr-1, curc)) {
        nF = _pathfinder_helper(maze, curr-1, curc, endr, endc, count+1, dfptr);
        if (nF != -1) {
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          fputc('S', dfptr);
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          return nF;
        }
    }
    
    if (is_path(maze, curr+1, curc)) {
        sF = _pathfinder_helper(maze, curr+1, curc, endr, endc, count+1, dfptr);
        if (sF != -1) {
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          fputc('N', dfptr);
          fseek(dfptr, -1, SEEK_CUR); // go back one position
          return sF;
            }
        }

    return -1;
}

/* this is a function implemented by the instructor to illustrate           */
/* how to use a recursive function to go from the left-most top entrance to */
/* the right-most bottom exit of a maze.  The function will print the       */
/* directions taken to reach the bottom exit, without visiting a location   */
/* twice.  Note that the recursion may visit a location a few times         */
/* Given three filenames, first one is a given maze, the second one is to   */
/* be written with the directions, the third is the explored maze           */
/* If the maze file does not exist, the function immediately return -1 and  */
/* do nothing else                                                          */
/* the directions are in the form of a series of characters                 */
/* 'N', 'S', 'E', 'W', without spacing (and without the quotes).            */
/* There should be just a single line, without a terminating newline        */
/* character. In other words, to the vi editor, it is an incomplete file    */
/* you are to assume that you start from the top-most, left-most entry      */
/* of the maze.  Therefore, the first direction is always 'S'               */
/* similarly, the last direction is always 'S'                              */
/* the returned value should be the number of steps taken to reach          */
/* the right-most bottom exit.  It is typically not the shortest distance   */
/* Note that the maze can be generated by amaze, or some combinations of    */
/* the maze expansion functions that you have written in PE08               */
/* you may assume that the maze file, if it exists, is in the correct format*/

int find_path_from_top_entrance_to_bottom_exit(char *maze_file, char *directions_file)
{
    FILE *mfptr = fopen(maze_file, "r");
    if (mfptr == NULL) {
        return -1;
    }
    
    /* read the maze from mfptr */
    
    Maze *maze = _read_maze(mfptr);
    fclose(mfptr);
    if (maze == NULL) {
        return -1;
    }
    
    /* find the starting location, the top-most, left-most opening */
    int startr, startc;
    startr = 0;
    for (startc = 0; startc < maze->n_col; startc++) {
        if (maze->maze_array[startr][startc] == PATH) {
            break;
        }
    }
    
    /* find the ending location, the bottom-most, right most opening */
    int endr, endc;
    endr = maze->n_row-1;
    for (endc = maze->n_col-1; endc >= 0; endc--) {
        if (maze->maze_array[endr][endc] == PATH) {
            break;
        }
    }
    
    // no starting location, deallocate and return
    if (startc == maze->n_col) {
        _free_maze(maze);
        return -1;
    }
    // no ending location, deallocate and return
    if (endc == -1) {
        _free_maze(maze);
        return -1;
    }
    
    FILE *dfptr = fopen(directions_file, "w");
    if (dfptr == NULL) {
        _free_maze(maze);
        return -1;
    }
    int steps = _pathfinder_helper(maze, startr, startc, endr, endc, 0, dfptr);
    //_write_maze(visited_file, maze);
    fclose(dfptr);
    _free_maze(maze);
    return steps;
}

int dfs_shortest_path_directions(char *maze_file, char *directions_file,
                                 Coord source, Coord destination)
{
    FILE *fp = fopen(maze_file, "r");
    if(fp == NULL){
        return -1;
    }
    
    Maze* maze = _read_maze(fp);
    
    if(maze == NULL){
        _free_maze(maze);
        return -1;
    }
    
    if(!(is_path(maze, source.row, source.col)) || !(is_path(maze, destination.row, destination.col))){
        return -1;
    }
    
    if(source.row > maze-> n_row || source.col > maze-> n_col || destination.row > maze->n_row || destination.col > maze->n_col){
        return -1;
    }
    
    FILE *fp1 = fopen(directions_file, "w");
    if(fp1 == NULL){
        return -1;
    }
    
    int steps = find_path_from_top_entrance_to_bottom_exit(maze_file, directions_file);
    
    return steps;
}

int simulate_movement(char *maze_file, char *directions_file, char *visited_file,
                      Coord source, Coord destination)
{
    FILE *fp = fopen(maze_file, "r");
    if(fp == NULL){
        return -1;
    }
    FILE *fp1 = fopen(directions_file, "r");
    if(fp1 == NULL){
        return -1;
    }
    
    
    Maze* maze = _read_maze(fp);
    
    if(!(is_path(maze, source.row, source.col)) || !(is_path(maze, destination.row, destination.col))){
        return -1;
    }
    
    if(maze == NULL){
        _free_maze(maze);
        return -1;
    }
    
    if(source.row > maze-> n_row || source.col > maze-> n_col || destination.row > maze->n_row || destination.col > maze->n_col){
        return -1;
    }
    int c = 0;
    int ct =0;
    int row = source.row;
    int col = source.col;
    
    fseek(fp1, 0, SEEK_SET);
    maze->maze_array[row][col] = VISITED;
    while((c = fgetc(fp1)) != EOF)
        if(c == 'N'){
            maze->maze_array[--row][col] = VISITED;
            ct++;
        }
         if(c == 'S'){
            
            maze->maze_array[++row][col] = VISITED;
            ct++;
        }
         if(c == 'E'){
            
            maze->maze_array[row][++col] = VISITED;
            ct++;
        }
         if(c == 'W'){
            maze->maze_array[row][--col] = VISITED;
            ct++;
        }
        if(c != 'N' && c != 'S' && c != 'E' && c != 'W'){
          //  _write_maze(visited_file, maze);
            return -1;
        }
        if(maze->maze_array[row][col] == WALL){
            //    _write_maze(visited_file, maze);
            return -1;
        }

        _write_maze(visited_file, maze);
    }
    
    
    _free_maze(maze);
    
    fclose(fp);
    fclose(fp1);
    
    return ct;
}



#ifndef __SRC_INCLUDE_S21_MATRIX_H
#define __SRC_INCLUDE_S21_MATRIX_H
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#define SUCCESS 1
#define FAILURE 0
#define EPS 1e-7
enum errCode { OK, INCORRECT_MATRIX, CALC_ERROR, OTHER_ERROR };
typedef struct matrix_struct {
    double **matrix;
    int rows;
    int columns;
} matrix_t;

int s21_create_matrix(int rows, int columns, matrix_t *result);
void s21_remove_matrix(matrix_t *A);
int s21_eq_matrix(matrix_t *A, matrix_t *B);
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_mult_number(matrix_t *A, double number, matrix_t *result);
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
int s21_transpose(matrix_t *A, matrix_t *result);
int s21_calc_complements(matrix_t *A, matrix_t *result);
int s21_determinant(matrix_t *A, double *result);
int s21_inverse_matrix(matrix_t *A, matrix_t *result);
void GetMinorMatrix(matrix_t *A, int skipRow, int skipCol, matrix_t *result);
void SwapRows(matrix_t *A, int row_1, int row_2);
int MakeCopy(matrix_t *A, matrix_t *result);
int isCorrectMatrix(matrix_t *A);

//other func
int check_dimensions(int rows, int columns);
void print_matrix(matrix_t A);
void matrice_to_null(matrix_t *A);

#endif // _SRC_INCLUDE_S21_MATRIX_H




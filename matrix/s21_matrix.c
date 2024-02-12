#include "s21_matrix.h"

void print_matrix(matrix_t A){
    for (int i=0; i<A.rows; i++){
        for(int j = 0; j<A.columns;j++){
            printf("%lf", A.matrix[i][j]);
        }
        printf("\n");
    }
}
int check_dimensions(int rows,int columns) {
    int code_Err = 0;
    if (rows < 0 || columns < 0){
        code_Err = 1;
    }
    return code_Err;
}

void matrice_to_null(matrix_t *A){
    for (int i =0; i< A->rows; i++){
        for (int j = 0; j < A->columns; j++){
            A->matrix[i][j] = 0.;
        }
    }

}

int s21_create_matrix(int rows,int columns, matrix_t *result) {
    int code_Err = check_dimensions(rows,columns);
    if (!code_Err){
        result->rows = rows;
        result->columns = columns;
        result->matrix = (double **)malloc(rows * sizeof(double *));
        for (int i = 0; i < rows; i++){
            result->matrix[i] = (double *)malloc(columns * sizeof(double *));
        }
        for (int i = 0; i<rows; i++){
            for (int j = 0; j < columns; j++){
                result->matrix[i][j] = (double)rand()/RAND_MAX;
            }
        }
    }
    return code_Err;
}

void s21_remove_matrix(matrix_t *A) {
    for (int i = 0; i<A->rows; i++){
        free(A->matrix[i]);
    }
    free(A->matrix);
    A->rows = 0;
    A->columns = 0;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B){
    int code_Err = SUCCESS;
    if (A->rows != B->rows || A->columns != B->columns){
        code_Err = FAILURE;
    } else {
        for (int i = 0; i < A->rows && code_Err == SUCCESS; i++){
            for (int j; j < A->columns; j++){
                if (A->matrix[i][j] != B->matrix[i][j]){
                    code_Err = FAILURE;
                }
            }
        }
    }
    return code_Err;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result){
    int code_Err = 0;
    if (A->rows < 0 || A->columns < 0 || B->rows < 0 || B->columns < 0 || result->rows < 0 || result->columns < 0){
        code_Err = 1;
    }   else if (A->rows != B-> rows || A->columns != B->columns || 
    A->rows != result->rows || A->columns != result->columns) {
        code_Err = 2;
    } else {
        for (int i = 0; i < A->rows; i++){
            for (int j = 0; j< A->columns; j++){
                result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
            }
        }
    }
    return code_Err;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code_Err = 0;
  if (A->rows < 0 || A->columns < 0 || B->rows < 0 || B->columns < 0 ||
      result->rows < 0 || result->columns < 0) {
    code_Err = 1;
  } else if (A->rows != B->rows || A->columns != B->columns ||
             A->rows != result->rows || A->columns != result->columns) {
    code_Err = 2;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return code_Err;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int code_Err = 0;
  if (A->rows < 0 || A->columns < 0) {
    code_Err = 1;
  } else if (A->rows != result->rows || A->columns != result->columns) {
    code_Err = 2;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return code_Err;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int code_Err = 0;
  if (A->rows < 0 || A->columns < 0 || B->rows < 0 || B->columns < 0 ||
      result->rows < 0 || result->columns < 0) {
    code_Err = 1;
  } else if (A->columns != B->rows || A->rows != result->rows ||
             B->columns != result->columns) {
    code_Err = 2;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = 0.;
        for (int k = 0; k < B->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return code_Err;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int code_Err = 0;
  if (A->rows < 0 || A->columns < 0) {
    code_Err = 1;
  } else if (A->rows != result->columns || A->columns != result->rows) {
    code_Err = 2;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return code_Err;
}

int s21_calc_complements(matrix_t* A, matrix_t* result) {
  if (A->columns != A->rows) {
    return CALC_ERROR;
  }

  if (!isCorrectMatrix(A)) {
    return INCORRECT_MATRIX;
  }

  if (s21_create_matrix(A->rows, A->columns, result) != OK) {
    return OTHER_ERROR;
  }

  if (A->rows == 1) {
    result->matrix[0][0] = 1;
    return OK;
  }

  matrix_t minor = {0};
  for (int i = 0; i < result->rows; ++i) {
    for (int j = 0; j < result->columns; ++j) {
      GetMinorMatrix(A, i, j, &minor);
      double determinant;
      s21_determinant(&minor, &determinant);
      s21_remove_matrix(&minor);
      result->matrix[i][j] = determinant;

      if ((i + j) % 2 != 0) {
        result->matrix[i][j] = -(result->matrix[i][j]);
      }
    }
  }

  return OK;
}

int MakeCopy(matrix_t* A, matrix_t* result) {
  if (s21_create_matrix(A->rows, A->columns, result) != OK) {
    return OTHER_ERROR;
  }

  for (int i = 0; i < A->rows; ++i) {
    for (int j = 0; j < A->columns; ++j) {
      result->matrix[i][j] = A->matrix[i][j];
    }
  }

  return OK;
}

int isCorrectMatrix(matrix_t* A) {
  return A->columns >= 0 && A->rows >= 0 && A->matrix;
}

void SwapRows(matrix_t* A, int row_1, int row_2) {
  if (row_1 != row_2) {
    for (int i = 0; i < A->columns; ++i) {
      double temp = A->matrix[row_1][i];
      A->matrix[row_1][i] = A->matrix[row_2][i];
      A->matrix[row_2][i] = temp;
    }
  }
}
int s21_determinant(matrix_t* A, double* result) {
  if (A->columns != A->rows) {
    return CALC_ERROR;
  }

  *result = 1.0;
  matrix_t temp = {0};

  if (MakeCopy(A, &temp) != OK) {
    return OTHER_ERROR;
  }

  int size = A->rows;

  for (int i = 0; i < size; ++i) {
    int supportElem = i;

    for (int j = i + 1; j < size; ++j) {
      if (fabs(temp.matrix[j][i]) > fabs(temp.matrix[supportElem][i])) {
        supportElem = j;
      }
    }

    if (fabs(temp.matrix[supportElem][i]) < EPS) {
      *result = 0.0;
      s21_remove_matrix(&temp);
      return OK;
    }

    SwapRows(&temp, i, supportElem);

    *result *= temp.matrix[i][i];

    if (i != supportElem) {
      *result *= -1;
    }

    for (int j = i + 1; j < size; ++j) {
      double div = temp.matrix[j][i] / temp.matrix[i][i];

      for (int k = i; k < size; ++k) {
        temp.matrix[j][k] -= temp.matrix[i][k] * div;
      }
    }
  }

  s21_remove_matrix(&temp);

  return OK;
}

int s21_inverse_matrix(matrix_t* A, matrix_t* result) {
  if (!isCorrectMatrix(A)) {
    return INCORRECT_MATRIX;
  }

  if (A->columns != A->rows) {
    return CALC_ERROR;
  }

  double det;
  s21_determinant(A, &det);

  if (fabs(det) < EPS) {
    return CALC_ERROR;
  }

  matrix_t A_transpose;
  s21_transpose(A, &A_transpose);
  matrix_t A_calcComp;
  s21_calc_complements(&A_transpose, &A_calcComp);
  s21_mult_number(&A_calcComp, 1.0 / det, result);

  s21_remove_matrix(&A_transpose);
  s21_remove_matrix(&A_calcComp);
  return OK;
}

void GetMinorMatrix(matrix_t* A, int skipRow, int skipCol, matrix_t* result) {
  s21_create_matrix(A->rows - 1, A->columns - 1, result);

  int stepI = 0;

  for (int i = 0; i < result->rows; ++i) {
    if (i == skipRow) {
      stepI = 1;
    }

    int stepJ = 0;
    for (int j = 0; j < result->columns; ++j) {
      if (j == skipCol) {
        stepJ = 1;
      }

      result->matrix[i][j] = A->matrix[i + stepI][j + stepJ];
    }
  }
}
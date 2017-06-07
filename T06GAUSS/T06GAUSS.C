 /* FILE NAME: T06GAUSS.C
 * PROGRAMMER: NK5
 * DATE: 06.06.2017
 * PURPOSE: solve SLAE.
 */

#include <stdio.h>
#include <math.h>

#define MAX 4

double matrix[MAX][MAX], B[MAX], X[MAX];
int N, P[MAX];

/* swap int values function */
void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
} /* end 'Swap' funtion */
/* swap double values function */
void FSwap( double *A, double *B )
{
  double tmp = *A;

  *A = *B;
  *B = tmp;
} /* end 'FSwap' funtion */

/* load matrix */
void MatrixLoad( char* FileName )
{
  int i, j;
  FILE *F;

  F = fopen(FileName, "r");
  if (F == NULL)
    return;

  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &matrix[i][j]);
    fscanf(F, "%lf", &B[i]);
  }  
  fclose(F);
} /* end 'MatrixLoad' function */
/* start Gauss method */
void Gauss( void )
{
  int g, i, j, MAX_i, MAX_j, sign = 1;
  double result = 1, coef;
  FILE *F;
  double sum = 0;

  for (g = 0; g < N; g++)
    X[g] = 0;

  for (g = 0; g < N; g++)
  {
    /* start for */
    MAX_i = g;
    MAX_j = g;
    /* search max in matrix */
    for (i = g; i < N; i++)
      for (j = g; j < N; j++)
        if(fabs(matrix[MAX_i][MAX_j] < fabs(matrix[i][j])))
          MAX_i = i, MAX_j = j;

    /* check 0 in matrix */
    if (fabs(matrix[MAX_i][MAX_j]) < 1e-15)
    {
      result = 0;
      return;
    }

    /* replace line */
    if (MAX_i != g)
    {
      sign = -sign;
      for (i = g; i < N; i++)
        FSwap(&matrix[MAX_i][i], &matrix[g][i]);
      FSwap(&B[MAX_i], &B[g]);
    }

    for (j = g + 1; j < N; j++)
    {
      coef = matrix[j][g] / matrix[g][g];
      for (i = 0; i < N; i++)
        matrix[j][i] -= matrix[g][i] * coef;
      B[j] -= B[g] * coef;
    }
    /* end for */
  }

  for (i = N - 1; i >= 0; i--)
  {
    for (j = N - 1; j >= i; j--)
      sum += X[j] * matrix[i][j]; 
    X[i] = (B[i] - sum) / matrix[i][i];
    sum = 0;
  }

  for (g = 0; g < N; g++)
    result *= matrix[g][g];
  result *= sign;
  F = fopen("OUTPUT.TXT", "a");
  if (F == NULL)
    return;
  for (g = 0; g < N; g++)
    fprintf(F, "%f ", X[P[g]]);
  fprintf(F, "\n", NULL);
  fclose(F);
} /* end of 'Gauss' function */

/* main function */
void main( void )
{
  int i;

  MatrixLoad("INPUT.TXT");
  for (i = 0; i < N; i++)
    P[i] = i;
  Gauss();
}
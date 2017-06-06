  /* FILE NAME: T05DET.C
 * PROGRAMMER: NK5
 * DATE: 05.06.2017
 * PURPOSE: deturmined of matrix.
 */

#include <stdio.h>
#include <math.h>

#define MAX 42

double matrix[MAX][MAX], Det = 0;
int p[MAX], parity, N;

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
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &matrix[i][j]);
  fclose(F);
} /* end 'MatrixLoad' function */


/* print in output */
void PrintDet( void )
{
  FILE *F;

  F = fopen("OUTPUT.TXT", "a");
  if (F == NULL)
    return;
  fprintf(F, "%lf \n", Det);
  fclose(F);
} /* end of 'PrintDet' function*/

/* start Gauss method */
void Gauss( void )
{
  int g, i, j, MAX_i, MAX_j, sign = 1;
  double result = 1, coef;
  FILE *F;

  for (g = 0; g < N; g++)
  {
    /* start for */
    MAX_i = g;
    MAX_j = g;
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

    if (MAX_i != g)
    {
      sign = -sign;
      for (i = g; i < N; i++)
        FSwap(&matrix[MAX_i][i], &matrix [g][i]);
    }

    if (MAX_j != g)
    {
      sign = -sign;
      for (i = g; i < N; i++)
        FSwap(&matrix[i][MAX_j], &matrix[i][g]);
    }

    for (j = g + 1; j < N; j++)
    {
      coef = matrix[j][g] / matrix[g][g];
      for (i = 0; i < N; i++)
        matrix[j][i] -= matrix[g][i] * coef;
    }
    /* end for */
  }
  for (g = 0; g < N; g++)
    result *= matrix[g][g];
  result *= sign;
  F = fopen("OUTPUT.TXT", "a");
  if (F == NULL)
    return;
  fprintf(F, "%f \n", result);
  fclose(F);
} /* end of 'Gauss' function */

/* recursion */
void Go( int pos )
{
  int i, Saveparity, tmp;
  double  mtmp = 1;

  if (pos >= N)
  {
    for (i = 0; i < N; i++)
      mtmp *= matrix[i][p[i]];
    if (parity)
      Det -= mtmp;
    else
      Det += mtmp;
    return;
  }
  Go(pos + 1);
  Saveparity = parity;
  for (i = pos + 1; i < N; i++)
  {
    Swap(&p[pos], &p[i]);
    parity = !parity;
    Go(pos + 1);
  }
  tmp = p[pos];
  for (i  = pos + 1; i < N; i++)
    p[i - 1] = p[i];
  p[N - 1] = tmp;
  parity = Saveparity;
} /* end of 'Go' function*/

void main( void )
{
  int i;

  MatrixLoad("INPUT.TXT");
  for (i = 0; i < N; i++)
    p[i] = i;
  Go(0);
  PrintDet();
  Gauss();
}
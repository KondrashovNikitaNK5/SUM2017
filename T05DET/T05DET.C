  /* FILE NAME: T05DET.C
 * PROGRAMMER: NK5
 * DATE: 05.06.2017
 * PURPOSE: deturmined of matrix.
 */

#include <stdio.h>

#define max 10

double matrix[max][max];
int p[max];
int parity, N;
double Det = 0;

/* swap function */
void Swap( int* A, int* B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void MatrixLoad( char* FileName )
{
  int i, j;
  FILE *F;

  F = fopen(FileName, "r");
  if (F == NULL)
    return;

  fscanf(F, "%d", &N);
  if (N > max)
    N = max;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &matrix[i][j]);
  fclose(F);
}

void PrintDet( void )
{
  FILE *F;

  F = fopen("OUTPUT.TXT", "a");
  if (F == NULL)
    return;
  fprintf(F, "%lf \n", Det);
  fclose(F);
}

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
}

void main( void )
{
  int i;

  MatrixLoad("INPUT.TXT");
  for (i = 0; i < N; i++)
    p[i] = i;
  Go(0);
  PrintDet();
}
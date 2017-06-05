  /* FILE NAME: T04PERM.C
 * PROGRAMMER: NK5
 * DATE: 05.06.2017
 * PURPOSE: permitations.
 */

#include <stdio.h>

#define max 47

int p[max];
int parity, N;

/* swap function */
void Swap( int* A, int* B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

/* Print result in file PERM.txt */
void PrintPerm( void )
{
  int i;
  FILE *F;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < N; i++)
    fprintf(F, "%i", p[i]);
  fprintf(F, " - %s \n", parity ? "odd" : "even");
  fclose(F);
}

/* recursion */
void Go( int pos )
{
  int i, Saveparity, tmp;

  if (pos >= N)
  {
    PrintPerm();
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

  printf("ENTER: N\n");
  scanf("%i", &N);
  for (i = 0; i < N; i++)
    p[i] = i + 1;
  Go(0);
}
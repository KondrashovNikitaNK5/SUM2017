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
  int i;
  if (pos >= N)
  {
    PrintPerm();
    return;
  }
  for (i = pos; i < N; i++)
  {
    Swap(&p[pos], &p[i]);
    if (pos != i)
      parity = !parity;
    Go(pos + 1);
    Swap(&p[pos], &p[i]);
    if (pos != i)
      parity = !parity;
  }
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
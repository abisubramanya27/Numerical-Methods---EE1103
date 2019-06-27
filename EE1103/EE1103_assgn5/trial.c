#include<stdio.h>

int main() {

  FILE *f = fopen("logistic_map.pgm","w");
  fprintf(f, "P2\n# comment\n640 480\n255\n");
  for (int n=0 ; n<480 ; ++n)
  {
      for (int m=0 ; m<640 ; ++m) fprintf(f, "%d ", p[n][m]);
      fprintf(f, "\n");
  }
  fclose(f);
}

//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//FUNCTION DECLARATIONS

double lagrange(double *, int, int);

double newton(double *, int, int);

void find_cubic_spline_coeff(double *, double **, int);

double cubic_spline(double *,double **, int, int);

void downsample(double *, int);

int main() {

   //Handle to file "out1_test0.csv"

   FILE *f_data = fopen("out1_test0.csv","r");

   //N : Number of data points given

   int N = 0;

   char temp[100];

   //To check if file is opened succesfully

   if (!f_data) {

      printf("FILE DOESN'T EXIST IN THE PATH GIVEN");
      exit(1);

   }

   while (fgets(temp,100,f_data)) {

      N++;

   }

   //Seeking file pointer to the beginning of the file

   fseek(f_data,0,SEEK_SET);

   //Array to store the y data points

   double *y_data = malloc(sizeof(double) * N);

   //To check if enough memory is allocated

   if (y_data == NULL) {

      printf("ERROR ALLOCATING MEMORY!!");
      exit(1);

   }

   for (int i = 0; i < N; i++) {

      fscanf(f_data,"%lf",(y_data+i));

   }

   //Downsampling the data and putting in a file using which gnuplot is made

   downsample(y_data,N);

   double sq_sum = 0;

   //Calculating sq_sum for every data point except those for which graph is drawn(every fifth point)

   for (int i = 0; i < N; i++) {

      if (i % 5 != 0) {

         sq_sum += pow(y_data[i] - lagrange(y_data,i+1,N),2);

      }

   }

   //rme is root mean error

   //We wont take those points (every fifth point) as it is with those points the interpolation is obtained

   double lagrange_rme = sqrt(sq_sum / N);
   printf("%f\n",lagrange(y_data,2,N));

   printf("LAGRANGE ROOT MEAN ERROR IS: %f\n",lagrange_rme);

   sq_sum = 0;

   for (int i = 0; i < N; i++) {

      if (i % 5 != 0) {

         sq_sum += pow(y_data[i] - newton(y_data,i+1,N),2);

      }

   }

   double newton_rme = sqrt(sq_sum / N);

   printf("NEWTON ROOT MEAN ERROR IS: %f\n",newton_rme);

   //printf("%f\n",newton(y_data,6,N));

   double **spline_coeff = malloc(sizeof(double*) * 4);

   for (int i = 0; i < 4; i++) {

      spline_coeff[i] = malloc(sizeof(double) * (N/5));

   }

   sq_sum = 0;

   find_cubic_spline_coeff(y_data,spline_coeff,N);

   //We are only taking till N - 2 (501) which is the last point in the set of points we took to construct the cubic spline
   //Cubic spline can be used to find the value of intermediate points only within the set of points we took to construct it

   for (int i = 0; i < N - 1; i++) {

      if (i % 5 != 0) {

         sq_sum += pow(y_data[i] - cubic_spline(y_data,spline_coeff,i+1,N),2);

      }

   }

   double spline_rme = sqrt(sq_sum / N);

   printf("CUBIC SPLINE ROOT MEAN ERROR IS: %f\n",spline_rme);

   //Closing file

   fclose(f_data);

   //Releasing memory from the pointers

   free(y_data);

   return 0;

}

//FUNCTION DEFINTIONS

double lagrange(double *y_data,int val_x,int N) {

   //val_y is the value of y corresponding to value of x from lagrange interpolation

   double val_y = 0;

   //Taking every fifth value starting from the fifth value (which is in 4th index in array)

   for (int i = 0; i < N; i += 5) {

      //coeff gives each coefficient of the lagrange polynomial

      double coeff = y_data[i];

      for (int j = 0; j < N; j += 5) {

         if (i != j) {

            coeff *= (double)(val_x - j - 1) / (i - j) ;

         }
      }

      val_y += coeff;

   }

   return val_y;

}

double newton(double *y_data,int val_x,int N) {

   //val_y is the value of y corresponding to value of x from newton interpolation

   //dr[i] contains the denominator of f(x_i) in the coefficient

   double val_y = 0, *dr = malloc(sizeof(double) * (N/5 + 1));

   //To check if enough memory is allocated

   if (dr == NULL) {

      printf("ERROR ALLOCATING MEMORY!!");
      exit(1);

   }

   //Taking every fifth value starting from the fifth value (which is in 4th index in array)

   for (int i = 0; i < N; i += 5) {

      //coeff gives each unknown coefficient of the lagrange polynomial

      //term gives the part of each newton polynomial term without the unknown coefficient

      double coeff = 0, term = 1;
      dr[i  / 5] = 1;

      for (int j = 0; j < i ; j += 5) {

         //Updating denominator of f(x_j) for the new coefficient

         dr[j / 5] *= (j - i);

         coeff += (y_data[j] / dr[j / 5]);

         //Forming the new denominator of f(x_i) for the new coefficient

         dr[i / 5] *= (i - j);

         term *= (val_x - j - 1);

      }

      coeff += (y_data[i] / dr[i / 5]);
      val_y += coeff * term;

  }

  //Releasing memory from the pointers

  free(dr);

  return val_y;

}

//The tridiagonal matrix is solved using LU Decomposition
//The tridiagonal matrix is obtained by using natural cubic spline algorithm

void find_cubic_spline_coeff(double *y_data,double **coeff, int N) {

  //AX = B is the matrix equation

  double *h = malloc(sizeof(double) * (N/5)), *B = malloc(sizeof(double) * ((N/5) + 1)), **A = malloc(sizeof(double*) * ((N/5) + 1));

  //To check if enough memory is allocated

  if (h == NULL || B == NULL ||  A == NULL) {

     printf("ERROR ALLOCATING MEMORY!!");
     exit(1);

  }

  //h[i] contains x_(i+1) - x_i

  for (int  i = 0; i < (N - 5); i+=5) {

     h[i / 5] = 5;

  }

  for (int  i = 5; i < (N - 5); i+=5) {

     B[i / 5] = (3 * (y_data[i + 5] - y_data[i]) / h[i / 5]) - (3 * (y_data[i] - y_data[i - 5]) / h[(i - 5)/5]);

  }

  B[0] = 0;
  B[N/5] = 0;

  //FORMING MATRIX A

  for (int i = 0; i <= (N/5); i++) {

     A[i] = malloc(sizeof(double) * ((N/5) + 1));
     for(int j = 0; j <= (N/5); j++) {

        if ( (i == 0 && j == 0) || (i == (N/5)  && j == (N/5)) ) {

           A[i][j] = 1;
           continue;

        }
        else if(i != N && i != 0) {


           if (j == i - 1) {

              A[i][j] = h[i - 1];
              continue;

           }
           else if (j == i) {

              A[i][j] = 2 * (h[i - 1] + h[i]);
              continue;

           }
           else if (j == i + 1) {

              A[i][j] = h[i];
              continue;

           }

        }

        A[i][j] = 0;

     }
  }

  //DECOMPOSING PART

  for (int i = 0; i < (N/5); i++) {

     for (int j = i + 1; j <= (N/5); j++) {

        double factor = A[j][i] / A[i][i];

        for (int k = i + 1; k <= (N/5); k++) {

           A[j][k] = A[j][k] - (factor * A[i][k]);

        }
        A[j][i] = factor;

     }

  }

  //FORWARD SUBSTITUTION STEP

  double *D = malloc(sizeof(double) * ((N/5) + 1));

  //To check if enough memory is allocated

  if (D == NULL) {

     printf("ERROR ALLOCATING MEMORY!!");
     exit(1);

  }

  for (int i = 0; i <= (N/5); i++) {

     double sum = 0;
     for (int j = 0; j <= i - 1; j++) {

        sum += (A[j][i] * D[j]);

     }
     D[i] = sum;

  }

  //Releasing memory from the pointers

  free(D);
  free(B);

  //BACKWARD SUBSTITUTION STEP

  //In 2-D MATRIX coeff 0:a_i 1:b_i 2:c_i 3:d_i

  for (int i = 0; i <= (N/5); i++) {

     double sum = 0;
     for (int j = i + 1; j <= (N/5); j++) {

        sum += (A[i][j] * coeff[2][j]);

     }
     coeff[2][i] = sum;

     //Parallely assigning a_i to the coefficient matrix

     coeff[0][i] = y_data[5 * i];

  }

  //Finding other coefficients b_i,d_i

  for (int i = 0; i < (N/5); i++) {

     coeff[3][i] = (coeff[2][i + 1] - coeff[2][i]) / (3 * h[i]);
     coeff[1][i] = ((coeff[0][i + 1] - coeff[0][i]) / h[i]) - (h[i] * (coeff[2][i + 1] + 2 * coeff[2][i]) / 3);
  }

  //Releasing memory from the pointers

  free(h);
  free(A);

}

double cubic_spline (double *y_data, double** coeff, int val_x, int N) {

  //val_y is the value of y corresponding to value of x from cubic spline interpolation

  double val_y = coeff[0][(val_x - 1) / 5];

  for (int i = 1; i < 4; i++) {

     val_y += (coeff[i][(val_x - 1) / 5] * pow((val_x - (((val_x - 1) / 5) * 5 + 1)),i));

  }

  return val_y;

}

void downsample(double *y_data, int N) {

   FILE *downsampled_file = fopen("downsampled_file.txt", "w");

   //To check if file is opened succesfully

   if (!downsampled_file) {

      printf("FILE DOESN'T EXIST IN THE PATH GIVEN");
      exit(1);

   }

   for (int i = 0; i < N; i+=5) {

      fprintf(downsampled_file,"%d %f\n",i+1,y_data[i]);

   }

   //Closing file

   fclose(downsampled_file);

}

//END OF PROGRAM

/*

LINUX COMMANDS FOR GNUPLOT :

plot "downsampled_file.txt" title "downsampled graph" with linesp

*/

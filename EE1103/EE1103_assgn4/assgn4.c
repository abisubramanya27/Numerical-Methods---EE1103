//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//FUNCTION DECLARATIONS

double trapezoid_integrate(double **, double, double, double);

double romberg_integrate(double **, double, double, int);

void find_cubic_spline_coeff(double *, double **, int);

double cubic_spline(double **, double);

//MAIN FUNCTION

int main() {

	//Handle to file "out1_test0.csv"

   FILE *f_data = fopen("out1_test0.csv","r");

   //N : Number of data points given

   int N = 0;

   char temp[100];

   //To check if file is opened successfully

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

   //We are interpolating the data before using romberg integration as we need the value of function at any given point to calculate the area using romberg integration

   double **spline_coeff = malloc(sizeof(double*) * 4);

   for (int i = 0; i < 4; i++) {

      spline_coeff[i] = malloc(sizeof(double) * N);

   }

   //Finding the coefficients of cubic spline polynomial in different intervals

   find_cubic_spline_coeff(y_data,spline_coeff,N);

   //We are only taking area till N - 1 as we are interpolating the function only in the interval [1,N)

   double trap_area = trapezoid_integrate(spline_coeff,1,(double)N - 1,1);
   double romberg_area = romberg_integrate(spline_coeff,1,(double)N - 1,16);

   printf("AREA CALCULATED USING TRAPEZOID RULE (with constant width of 1) IS : %lf\n",trap_area);

   //Since I need O(h^16) error passing last parameter as 16

   printf("AREA CALCULATED USING ROMBERG INTEGRATION IS : %lf\n",romberg_area);

   printf("DIFFERENCE BETWEEN AREA CALCULATED BY ROMBERG AND TRAPEZOIDAL METHOD IS : %lf\n",fabs(romberg_area - trap_area));

   //Closing file

   fclose(f_data);

   //Releasing memory from pointers

   free(y_data);
   free(spline_coeff);

}

//FUNCTION DEFINITIONS

double trapezoid_integrate(double **coeff, double a, double b, double h) {

	double area = 0;
	double sum = 0;

  //Calculating area by constructing a trapezium with every consecutive datapoint with constant width h

  for (double i = a + h; i <= b - h; i += h) {

    sum += 2 * (cubic_spline(coeff,i));

	}

  area = (sum + cubic_spline(coeff,a) + cubic_spline(coeff,b)) * h / 2;
	return area;

}

double romberg_integrate(double **coeff, double a, double b, int approx) {

  //variable approx tells the order of error we desire (i.e) O(h^approx)

  //romberg_term is the 2D array used for calculating the recursion

  double **romberg_term = malloc(sizeof(double*) * (approx + 1) / 2);

  //(approx + 1) / 2 columns needed for O(h^approx) approximation

  //First column is O(h^2), second column O(h^4), third column O(h^6) and so on

  for (int i = 0; i < (approx + 1) / 2; i++) {

    romberg_term[i] = malloc(sizeof(double) * (approx + 1) / 2);
    romberg_term[i][0] = trapezoid_integrate(coeff,a,b,(b - a) / pow(2,i));

  }

  for (int j = 1; j < (approx + 1) / 2; j++) {

    for (int i = j; i < (approx + 1) / 2; i++)

       romberg_term[i][j] = ((pow(4,j) * romberg_term[i][j - 1]) - romberg_term[i - 1][j - 1]) / (pow(4,j) - 1);

  }

  double area = romberg_term[((approx + 1) / 2) - 1][((approx + 1) / 2) - 1];

  //Releasing memory from pointers

  free(romberg_term);

  //This term returned has the least error considering anything >= O(h^approx) as negligible

  return area;

}

//The tridiagonal matrix is solved using LU Decomposition

//The tridiagonal matrix is obtained by using natural cubic spline algorithm

void find_cubic_spline_coeff(double *y_data,double **coeff, int N) {

  //AX = B is the matrix equation

  double *h = malloc(sizeof(double) * N), *B = malloc(sizeof(double) * (N + 1)), **A = malloc(sizeof(double*) * (N + 1));

  //To check if enough memory is allocated

  if (h == NULL || B == NULL ||  A == NULL) {

     printf("ERROR ALLOCATING MEMORY!!");
     exit(1);

  }

  //h[i] contains x_(i+1) - x_i

  for (int  i = 0; i < N - 1; i++) {

     h[i] = 1;

  }

  for (int  i = 1; i < N - 1; i++) {

     B[i] = (3 * (y_data[i + 1] - y_data[i]) / h[i]) - (3 * (y_data[i] - y_data[i - 1]) / h[i - 1]);

  }

  B[0] = 0;
  B[N] = 0;

  //FORMING MATRIX A

  for (int i = 0; i <= N; i++) {

     A[i] = malloc(sizeof(double) * (N + 1));
     for(int j = 0; j <= N; j++) {

        if ( (i == 0 && j == 0) || (i == N  && j == N) ) {

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

  //DECOMPOSING PART - splitting A into a lower triangular and upper triangular matrix and storing the values of both in the same A matrix

  for (int i = 0; i < N; i++) {

     for (int j = i + 1; j <= N; j++) {

        double factor = A[j][i] / A[i][i];

        for (int k = i + 1; k <= N; k++) {

           A[j][k] = A[j][k] - (factor * A[i][k]);

        }
        A[j][i] = factor;

     }

  }

  //FORWARD SUBSTITUTION STEP -  solving for D[i] by using LD = B

  double *D = malloc(sizeof(double) * (N + 1));

  //To check if enough memory is allocated

  if (D == NULL) {

     printf("ERROR ALLOCATING MEMORY!!");
     exit(1);

  }

  for (int i = 0; i <= N; i++) {

     double sum = 0;
     for (int j = 0; j <= i - 1; j++) {

        sum += (A[i][j] * D[j]);

     }

     D[i] = B[i] - sum;

  }

  //BACKWARD SUBSTITUTION STEP - solving for c_i using UX = D

  //In 2-D MATRIX coeff 0:a_i 1:b_i 2:c_i 3:d_i

  for (int i = 0; i < N; i++) {

     double sum = 0;
     for (int j = i + 1; j <= N; j++) {

        sum += (A[i][j] * coeff[2][j]);

     }

     coeff[2][i] = (D[i] - sum) / A[i][i];

     //Parallely assigning a_i to the coefficient matrix

     coeff[0][i] = y_data[i];

  }

  //Releasing memory from the pointers

  free(D);
  free(B);

  //Finding other coefficients b_i,d_i

  for (int i = 0; i < N - 1; i++) {

     coeff[3][i] = (coeff[2][i + 1] - coeff[2][i]) / (3 * h[i]);
     coeff[1][i] = ((coeff[0][i + 1] - coeff[0][i]) / h[i]) - (h[i] * (coeff[2][i + 1] + 2 * coeff[2][i]) / 3);

  }

  //Releasing memory from the pointers

  free(h);
  free(A);

}

double cubic_spline (double** coeff, double val_x) {

  //val_y is the value of y corresponding to value of x from cubic spline interpolation

  double val_y = coeff[0][(int)val_x - 1];

  for (int i = 1; i < 4; i++) {

     val_y += (coeff[i][(int)val_x - 1] * pow((val_x - (int)(val_x)),i));

  }

  return val_y;

}

//END OF PROGRAM

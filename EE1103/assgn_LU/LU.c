/*
EE18B001,EE18B002,EE18B006
ASSIGNMENT 9 : USING LU DECOMPOSITION AND SOLVING A SET OF THREE EQUATIONS USING MATRIX METHOD
               ALSO INTERPOLATING A GIVEN SET OF POINTS IN A FILE USING CUBIC SPLINE INTERPOLATION, BY FINDING OUT COEFFICIENTS OF EACH CUBIC POLYNOMIAL BY LU DECOMPOSITION
Submitted on 10 November 2018
Version 1
*/

//DECLARATION OF HEADER FILES

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

//FUNCTION DEFINITIONS

//Function to find the L and U matrices
void findinglowerandupper(double a[3][3], double l[3][3], double u[3][3]);

//Function to multiply L and U and check if LU=A
void multiply(double a[3][3], double l[3][3], double u[3][3]);

//Function to solve the three equations given
void solver(double l[3][3], double u[3][3], double b[3], double x[3]);

//Function to find the coefficients of cubic polynomial joining every two points by LU DECOMPOSITION
void find_cubic_spline_coeff(double *,double **, int);

//Function to calculate the y value for a given x value using the cubic spline interpolation
double cubic_spline (double *, double**, int, int);


//MAIN FUNCTION

int main()
{
	printf("MENU\n  1)Solve a set of three equations\n  2)Interpolate the points given in a file by cubic spline\nEnter your choice: ");

	int c;
	scanf("%d",&c);

  //Solves a set of three equations by LU DECOMPOSITION METHOD

	if(c==1)
	{

		//declaration of matrix A,L,U,B and X
		double a[3][3];
		double l[3][3];
		double u[3][3];
		double b[3];
		double x[3];

		//declaration of variable f. f is a variable which stores the answer to the choice given to the user whether to solve a default set of three equations(i.e, the one given in assignment 9) or otherwise. The user needs to enter 1 if they need to solve the default set of equations.

		int f;


		printf("If you want to solve the equations\n	3 x1 - 0.1 x2 - 0.2 x3 = 7.85 \n	0.1 x1 + 7 x2 - 0.3 x3 = -19.3 \n	0.3 x1 - 0.2 x2 + 10 x3 = 71.4 \nPress 1: \n\nElse press any other number: ");
		scanf("%d",&f);

		//Inputting values for default set of equations

		if(f==1)
		{
			a[0][0]=3;
		 	a[0][1]=-0.1;
		 	a[0][2]=-0.2;
		 	a[1][0]=0.1;
		 	a[1][1]=7;
		 	a[1][2]=-0.3;
		 	a[2][0]=0.3;
		 	a[2][1]=-0.2;
		 	a[2][2]=10;
			b[0]=7.85;
			b[1]=-19.3;
			b[2]=71.4;
		}

		//If the user wants to create a new set of equations, they need to input values for matrix A and B

		else
		{

			for (int i = 0; i < 3; i++)
			 {
				for (int j = 0; j < 3; j++)
				{
					printf("ENTER THE VALUE OF %d ROW AND %d COLUMN: \n", i+1, j+1);
					scanf("%lf", &a[i][j]);
				}

		}

		 for (int i = 0; i < 3; i++)
		 {
			printf("ENTER THE VALUES FOR RHS OF EQUATION %d: ",i+1);
			scanf("%lf", &b[i]);
		 }

		}

		//calling function to find L and U
		findinglowerandupper(a, l, u);

		/*
		The below function can be called if it is required to check LU = A

		//calling function to find LU and check if A=LU
		multiply(a, l, u);

    */

		//calling function to solve the set of equations and printing the solution
		solver(l, u, b, x);

	}

  //Interpolates points in a given file by CUBIC SPLINE INTERPOLATION whose coefficients are obtained using LU DECOMPOSITION method

	else if(c==2)
	{
		  float sq_sum;

      char filename_path[250];

			printf("\n\nEnter the filename along with path (if file is not in the current working directory) which contains the points to be interpolated: ");
			scanf("%s",filename_path);

		  //Handle to file containing the points

	   	FILE *f_data = fopen(filename_path,"r");

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


	   double **spline_coeff = malloc(sizeof(double*) * 4);

	   for (int i = 0; i < 4; i++) {

	      spline_coeff[i] = malloc(sizeof(double) * (N/5));

	   }

	   sq_sum = 0;

	   find_cubic_spline_coeff(y_data,spline_coeff,N);

		 FILE *cubic_spline_file = fopen("file_cubicsp.txt","w");

	   //We are only taking till N - 2 (501) which is the last point in the set of points we took to construct the cubic spline

	   //Cubic spline can be used to find the value of intermediate points only within the set of points we took to construct it

	   for (int i = 0; i < N - 1; i++) {

			 double y_value = cubic_spline(y_data,spline_coeff,i+1,N);

	      if (i % 5 != 0) {

	         sq_sum += pow(y_data[i] - y_value,2);

	      }

        //Putting the points generated by cubic spline for x values varying by 1 from 1 to N-1

				fprintf(cubic_spline_file,"%d %lf\n",i+1,y_value);

	   }

		 //Closing files

	   fclose(f_data);
		 fclose(cubic_spline_file);

		 FILE* file_gnu_cubicsp = fopen("cubicspline.gp","w");    //Writing the gnuplot commands to a gnuplot script file
		 fprintf(file_gnu_cubicsp,"set term png\nset xlabel \"X\" textcolor lt 2\nset ylabel \"Y\" textcolor lt 4\nset output \"cubic_spline_graph.png\"\np \"file_cubicsp.txt\" u 1:2 title \"CUBIC SPLINE GRAPH\" lc 3 w lines\n");
     fclose(file_gnu_cubicsp);   //Closing the gnuplot script file so that we can execute those commands
		 system("gnuplot -p 'cubicspline.gp'");    //Using system function to run the commands in the gnuplot script file and generate the plot from the c program

		 remove("file_cubicsp.txt");
		 remove("cubicspline.gp");

	   double spline_rme = sqrt(sq_sum / N);

	   printf("CUBIC SPLINE ROOT MEAN ERROR IS: %f\n\n",spline_rme);
		 printf("CHECK THE CURRENT FOLDER FOR THE GRAPH!!\n");

	   //Releasing memory from the pointers

	   free(y_data);


	}

}


//FUNCTION DEFINITIONS

//Function to find the L and U matrices

void findinglowerandupper(double a[3][3], double l[3][3], double u[3][3])
{
	//First we equate each element of U matrix to the corresponding element of A matrix so that we can directly do the operations on U matrix to get the final upper matrix U

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			u[i][j] = a[i][j];
		}
	}

	//Finding the factors(fac1,fac2,fac3) to do row operations to yield upper matrix (to make the element in the lower triangle zero). Also doing the row operations to get final matrix U.

	double fac1, fac2, fac3;
	fac1 = u[1][0] / u[0][0];
	fac2 = u[2][0] / u[0][0];

	for (int i = 0; i < 3; i++)
	{
		u[1][i] -= u[0][i] * fac1;
		u[2][i] -= u[0][i] * fac2;
	}
	fac3 = u[2][1] / u[1][1];

	for (int i = 0; i < 3; i++)
	{
		u[2][i] -= u[1][i] * fac3;
	}

	//Finding L matrix where diagonal elements are equated to 1, upper triangle elements equated to 0 and lower triangle elements equated to the factors computed while finding matrix U (fac1,fac2,fac3)

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j)
			{
				l[i][j] = 1;
			}
			else if (i < j)
			{
				l[i][j] = 0;
			}
		}
	}

	l[1][0] = fac1;
	l[2][0] = fac2;
	l[2][1] = fac3;
}


//Function to multiply L and U and check if LU=A

void multiply(double a[3][3], double l[3][3], double u[3][3])
{
	//f is a test matrix which will be made to store the product matrix of L and U
	double f[3][3];

	//c is a variable which is used to check if f (LU) = A. If c=0 at the end of checking corresponding elements of f and a matrices, then we now for sure that A=LU.
	int c = 0;


	//Initializing each element of f matrix to 0 so that multiplication of matrices could be done using it

  for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
      	f[i][j]=0;
		}

	}

	//Finding LU and equating it to f matrix

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				f[i][j] += l[i][k] * u[k][j];
			}
		}
	}

	//This piece of code in comments could be used to print the values stored in matrix f and A

	/*
	  for(int i=0;i<3;i++)
	  {
		  printf("\n");

			for(int j=0;j<3;j++)
	      	printf("%lf\t",f[i][j]);

	  }

		printf('\n");

	  for(int i=0;i<3;i++)
	  {
			printf("\n");

	  	for(int j=0;j<3;j++)
	     		printf("%lf\t",a[i][j]);

	  }

	*/


	//Checking if matrix f equal to matrix A. If c=0, LU=A.

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (f[i][j] != a[i][j])
			{
				c++;
			}
		}
	}

	if (c == 0)
	{
		printf("\nLU equals A\n\n");
	}
	else
	{
		printf("\nLU not equals A\n\n");
	}

}


//Function to solve the three equations given

/*

We need to solve for x where Ax=B

Ax=B
(LU)x=B
L(Ux)=B
We consider a matrix y=Ux
=>Ly=B
Find y matrix, then using y=Ux, we can find matrix x.

*/

void solver(double l[3][3], double u[3][3], double b[3], double x[3])
{
	//Using forward substitution to find matrix y. We find each element of y matrix (beginning with y1) by expanding Ly and eqauting it to B.

	double y[3];
	y[0] = b[0];
	y[1] = b[1] - l[1][0] * y[0];
	y[2] = b[2] - l[2][0] * y[0] - l[2][1] * y[1];

	//Using backward substitution to find matrix x. We find each element of x (starting with x3) by expanding Ux and equating it to y.

	x[2] = y[2] / u[2][2];
	x[1] = (y[1] - u[1][2] * x[2]) / u[1][1];
	x[0] = (y[0] - u[0][1] * x[1] - u[0][2] * x[2]) / u[0][0];

	//Now that the solution matrix x is found, printing the solution to the user

	for (int i = 0; i < 3; i++)
	{
		printf("X%d:%lf\n", i, x[i]);
	}

}

//The tridiagonal matrix is solved using LU Decomposition

//The tridiagonal matrix is obtained by using natural cubic spline algorithm

/*
General form of the matrix which is solved by the LU Decomposition method  ---- REPORT

 --																																		  --   --    --						  --																										--
|  1     0               0              0      	 . . .							0     | |  c_0   |           |                           0														|
|  h_0   2(h_0 + h_1)    h_1            0     	 . . .							0     | |  c_1   |					 |       (3/h_1)*(a_2 - a_1) - (3/h_0)*(a_1 - a_0)				|
|  0     h_1             2(h_1 + h_2)   h_2   	 . . .							0	    | |  c_2   |           |       (3/h_2)*(a_3 - a_2) - (3/h_1)*(a_2 - a_1)				|
|  .     .               .              .     	 . . .							.     | |   .    |    ----   |                           .														|
|  .     .               .              .    	   . . .						  .     | |   .    |    ----   |                           .														|
|  .     .               .							.     	 . . .					    .     | |   .    |           |										     	 .														|
|  0     0               0							h_n-2    2(h_n-2 + h_n-1)   h_n-1 | |  c_n-1 |           |	 (3/h_n-1)*(a_n - a_n-1) - (3/h_n-2)*(a_n-1 - a_n-2)  |
|  0     0               0							0        . . .              1     | |  c_n   |           |     										   0														|
 -- 																																	  --   --    --             --																										--

*/
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


//END OF PROGRAM

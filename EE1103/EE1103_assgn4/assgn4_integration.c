//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//FUNCTION DECLARATIONS

double trapezoid_integrate(double *, int, int, int);
int max_power_two(int);
double romberg_integrate(double *, int, int);

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

   printf("AREA CALCULATED USING TRAPEZOID RULE IS : %lf\n",trapezoid_integrate(y_data,1,N,1));
   printf("AREA CALCULATED USING ROMBERG INTEGRATION IS : %lf\n",romberg_integrate(y_data,1,N));

   //Closing file

   fclose(f_data);

   //Releasing memory from pointer

   free(y_data);

}

//FUNCTION DEFINITIONS

double trapezoid_integrate(double *y_data, int a, int b, int h) {

	double area = 0, width = h;
	int prev_index = 0;

  int i = a - 1;

  //Calculating area by constructing a trapezium with every consecutive datapoint with constant width 1

  while (i < b - 1) {

    if ((i + width) >= b) {

      width = b - i - 1;
      i += width;

    }
    else

      i += width;

		area += (double)(width * (y_data[i] + y_data[prev_index])) / 2;
		prev_index = i;

	}

	return area;

}

//The below function calculates [logN], where [] : GIF

int max_power_two(int N) {

  int log_2 = -1, res = 1;
  for (;res <= N;res *= 2,log_2++);
  return log_2;

}

double romberg_integrate(double *y_data, int a, int b) {

  int log_2 = max_power_two(b - a);

  //romberg_term is the 2D array used for calculating the recursion

  double **romberg_term = malloc(sizeof(double*) * log_2);

  //Only two columns required for O(h^4) approximation
  //First column is O(h^2) and second column O(h^4)

  for (int i = 0; i < log_2; i++) {

    romberg_term[i] = malloc(sizeof(double) * 2);
    romberg_term[i][0] = trapezoid_integrate(y_data,a,b,(b - a) / pow(2,i));

  }

  for (int i = 1; i < log_2; i++) {

    romberg_term[i][1] = ((4 * romberg_term[i][0]) - romberg_term[i - 1][0]) / 3;

  }

  //Releasing memory from pointer

  free(romberg_term);

  //This term returned has the least error considering anything >= O(h^4) as negligible

  return(romberg_term[log_2 - 1][1]);

}

//END OF PROGRAM

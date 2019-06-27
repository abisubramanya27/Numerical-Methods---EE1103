//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Global Declaration of the parameters of the differential equation

double alpha, beta, gama, delta, stepsize;

//FUNCTION DECLARATIONS

//Function to solve differential equation using the lotka_volterra volterra model

void lotka_volterra(double , double);

//MAIN FUNCTION

int main(int argc,char **argv)
{
  //Check to make sure that arguments are passed as : alpha beta gama delta stepsize

  if(argc != 6)
  {
    printf("ARGUMENTS SHOULD BE OF THE FORM : %s alpha beta gama delta stepsize\n",argv[0]);
    exit(1);
  }

  else
  {
    double x0, y0;
    //Converting the data type of arguments that were passed from character to double

    alpha = (double)atof(argv[1]);
    beta = (double)atof(argv[2]);
    gama = (double)atof(argv[3]);
    delta = (double)atof(argv[4]);
    stepsize = (double)atof(argv[5]);

    //Obtaining initial values of x and y from the user

    printf("\nENTER Xo AND Yo PLEASE\n");
    scanf("%lf",&x0);
    scanf("%lf",&y0);

    //Calling the lotka_volterra function to solve the differential equations for x and y

    lotka_volterra(x0,y0);

  }

  return 0;

}

//FUNCTION DEFINITION

void lotka_volterra(double x_i,double y_i)
{
  //File to store the x and y values obtained after solving the differential equations

  FILE *fptr_phase,*fptr_x,*fptr_y;

  //To check if the files are being opened successfully

  if (!(fptr_phase = fopen("lotka_volterra_phase.csv","w"))) {

    printf("ERROR OPENING FILE!!\n");
    exit(1);

  }

  if (!(fptr_x = fopen("lotka_volterra_x.csv","w"))) {

    printf("ERROR OPENING FILE!!\n");
    exit(1);

  }

  if (!(fptr_y = fopen("lotka_volterra_y.csv","w"))) {

    printf("ERROR OPENING FILE!!\n");
    exit(1);

  }

 else
 {
   //dx and dy are dx/dt and dy/dt respectively

   double dx = alpha * x_i - beta * x_i * y_i;
   double dy = delta * x_i * y_i - gama * y_i;

   //x and y are the values derived from previous values x_i, y_i

   double x = x_i, y = y_i, t = 0;

   //Writing the initial x and y values on the file

   fprintf(fptr_phase,"%lf,%lf\n",x,y);
   fprintf(fptr_x,"%lf,%lf\n",t,x);
   fprintf(fptr_y,"%lf,%lf\n",t,y);

   //Running the loop some 10000 times

   for (int i = 1; i <= 10000; i++)
   {
     //Finding the subsequent x and y values by solving using the differential equation

     x = x_i + dx * stepsize;
     y = y_i + dy * stepsize;
     t += stepsize;

     //Updating the value of dx/dt and dy/dt

     dx = alpha * x - beta * x * y;
     dy = delta * x * y - gama * y;

     //writing the x and y values on the file

     fprintf(fptr_phase,"%lf,%lf\n",x,y);
     fprintf(fptr_x,"%lf,%lf\n",t,x);
     fprintf(fptr_y,"%lf,%lf\n",t,y);

     x_i = x;
     y_i = y;

   }

   //Closing files

   fclose(fptr_phase);
   fclose(fptr_x);
   fclose(fptr_y);

 }

}

//END OF PROGRAM

//GRAPH IS DRAWN WITH X0 = 0.9 and Y0 = 0.9 where X and Y quantify thousands

//alpha = 0.67 beta = 1.33 gama = delta = 1 stepsize = 0.01

//IN GRAPH X-AXIS : X     Y-AXIS : Y

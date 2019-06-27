//EE18B001 (ABISHEK.S), EE18B015 (KARTHIKEYAN.R), EE18B021 (NITHIN BABU)
//START OF PROGRAM

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void Random(long int*, long int, long int, long int);

int main(int argc,char **argv) {

   //Totally five arguments to main: PATH SEED_VALUE A C M

   if (argc != 5)

      printf ("THE LINUX COMMAND MUST BE OF THE FORM: %s SEED_VALUE A C M",argv[0]);

   else {

      long int seed_val = atol(argv[1]), A = atol(argv[2]), C = atol(argv[3]), M = atol(argv[4]);

      long int *count = malloc(M * sizeof(long int)) ,total_no = 0;

      long double mean = 0.0, std_dev = 0.0, sum = 0, sq_sum = 0;

      //File for storing the random numbers generated to plot histogram

      FILE *f_rand = fopen("f_rand.txt","w");

      //If enough memory isn't there to allocate to pointers, quitting program

      if (count == NULL) {
         printf("ERROR ALLOTTING MEMORY!!");
         exit(1);
      }

      //Initialising all elements of count array to 0

      for (long int i = 0; i < M; i++)

         count[i] = 0;

      //Running till quite a large number but not depending on M as
      //M can be too small in which case very small number of random numbers would be generated,
      //or M can be too large when computer may take long time to show the output

      for (long int i = 0; i < 1000000; i++) {

         Random(&seed_val,A,C,M);
         sum += seed_val;

         //seed_val range is from 0 to M - 1

         count[seed_val]++;

         total_no++;

         //Printing random numbers to console

         printf("%ld\n",seed_val);

      }

      mean = (long double)sum / total_no;

      //Calculating variance using basic definition

      for (long int i = 0; i < M; i++) {

         sq_sum += count[i] * (i - mean) * (i - mean);
      }

      for (long int i = 0; i < M; i++) {

         //Printing random number and their occurences in the file
         //Only printing those numbers which were generated atleast once,
         //as anyway the graph will show zero count for those numbers
         //and we can prevent too large memory usage by files

          if (count[i])

             fprintf(f_rand,"%ld %ld\n",i,count[i]);

      }

      long double quad_mean = (long double)sq_sum / total_no;

      std_dev = sqrt(sq_sum / total_no);

      //Printing the mean and standard deviation of the set of random numbers

      printf("The mean is: %Lf\nThe standard deviation is: %Lf\n",mean,std_dev);

      //Releasing the memory associated with all the pointers

      free(count);

      //Closing the file

      fclose(f_rand);

   }

      return 0;
}

//Passing seed_val by reference as we need waste storage to save previous seed values

void Random(long int *seed, long int A, long int C, long int M) {

   //Updating the seed value as the random number generated is the seed for subsequent random numbers

   *seed = ((*seed) * A + C) % M;

}

//END OF PROGRAM

/*

ANSWERS:

(a) CODE GIVEN ABOVE.

(b) GIVE VALUE OF M AS 2.

(c) The random numbers generated follow a repititive pattern and so the numbers are predictable after
    a point of time.If the values of A,C,M are not chosen properly the period of repitition is very
    small and numbers are easily predicatble.And if this happens it is a repititive number generator,
    no longer a random number generator.

    As we decrease M, the period of repitition keeps on decreasing, the randomness of numbers
    generated decreases.

*/

/*

LINUX COMMANDS FOR QUESTION 2 :

gnuplot
set term png
set output "quiz1_histogram.png"
plot "f_rand.txt" title "random numbers histogram" with linesp

*/

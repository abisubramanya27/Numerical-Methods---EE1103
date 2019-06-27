//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main(int argc,char **argv) {

  //Check to make sure that arguments are passed as : r_initial r_final x0

  if(argc != 4)
  {
    printf("ARGUMENTS SHOULD BE OF THE FORM : %s r_initial r_final x0\n",argv[0]);
    exit(1);
  }

  int width = 640, height = 480;
	double x_i = atof(argv[3]), r_i = atof(argv[1]), r_f = atof(argv[2]);
	int **pixel = malloc(sizeof(int*) * height);

  //Checking errors in allotting memory to pointer

	if (!pixel) {

		printf("ERROR ALLOTTING MEMORY!!\n");
		exit(1);

	}

	for (int i = 0; i < height; i++) {

		pixel[i] = malloc(sizeof(int) * width);

		for (int j = 0; j < width; j++) {

			pixel[i][j] = 0;

		}
	}

  //Taking 255 as limit of oscillation (saturation point)

	for (int j = 0; j < width; j++) {

		double r = r_i + (j / (double)width) * (r_f - r_i);
		double x = x_i;

    //Running the loop for some 20000 times maximum to get all the different points on the plot

		for (int i = 0; i < 20000; i++) {

			x = r * x * (1 - x);
      int index = (int)height * x;
			if (pixel[index][j] < 255)

			    pixel[index][j]++;

			else

				  break;
		}
	}

	FILE *img_file = fopen("logistic_map.png","w");

	//Checking error in opening file

	if (!img_file) {

		printf("ERROR OPENING FILE!!\n");
		exit(1);

	}

	fprintf(img_file, "P2\n%d %d\n255\n", width, height);

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			fprintf(img_file,"%d ",pixel[i][j]);

		}

		fprintf(img_file,"\n");

	}

	//Closing file

	fclose(img_file);

	//Releasing memory from pointer

	free(pixel);

}

//END OF PROGRAM

//The png file is obtained by taking the interesting range as 2.4(r_initial) <= r <= 4(r_final) and x0 as 0.5

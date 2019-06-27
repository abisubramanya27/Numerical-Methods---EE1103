//EE18B001, EE18B002, EE18B006

//START OF PROGRAM

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int main() {

	//ASSUMING 2.4 <= r <= 4

    int width = 480, height = 640;
	double x_i = 0.5, r_i = 2.4, r_f = 4;
	int *points = malloc(sizeof(int) * height);

    //Checking errors in allotting memory to pointer

	if (!points) {

		printf("ERROR ALLOTTING MEMORY!!\n");
		exit(1);

	}

	//Taking 25 as limit of oscillation (saturation point)

	/*for (int j = 0; j < width; j++) {

			points[j] = 0;
    }*/

    FILE *img_file = fopen("logistic_map.txt","w");

	//Checking error in opening file

	if (!img_file) {

		printf("ERROR OPENING FILE!!\n");
		exit(1);

	}

	for (int j = 0; j < width; j++) {

		double r = r_i + (j / (double)width) * (r_f - r_i);
		double x = x_i;
		for (int i = 0; i < height; i++) {

			x = r * x * (1 - x);
			fprintf(img_file,"%lf %lf\n",r,(height * x));

		}
	}

	//Closing file

	fclose(img_file);

	//Releasing memory from pointer

	free(points);

}

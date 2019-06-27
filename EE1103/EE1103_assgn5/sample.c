#include <stdio.h>

// Allocate a pixel array to store generated image
int p[800][800];

int main()
{
    // Algorithm parameters
    int w=640, h=480;
    int m, n, M=10000;
    double r, r_min=3, r_max=4;
    double x, x0=0.5;

    // Set every pixel to zero initially
    printf("Blanking canvas\n");
    for (n=0 ; n<h ; ++n) for (m=0 ; m<w ; ++m) p[n][m] = 0;

    // Calculate points and add to image.
    // Each new point inrements a single pixel's value.
    // Pixels saturate at 255.
    printf("Calculating and plotting points\n");
    for (n=0 ; n<w ; ++n)
    {
        // Calculate points for current r value
        r = r_min + (n/(double)w)*(r_max - r_min);
        printf("r = %lf\n", r);
        x = x0;
        for (m=0 ; m<M ; ++m)
        {
            x = r * x * (1 - x);
            if (p[(int)(h*x)][n] < 255) p[(int)(h*x)][n]++;
        }
    }

    // Write image to PGM file
    printf("Writing PGM file\n");
    FILE *f = fopen("graph.pgm", "w");
    fprintf(f, "P2\n# comment\n%d %d\n255\n", w, h);
    for (n=0 ; n<h ; ++n)
    {
        for (m=0 ; m<w ; ++m) fprintf(f, "%d ", p[n][m]);
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}

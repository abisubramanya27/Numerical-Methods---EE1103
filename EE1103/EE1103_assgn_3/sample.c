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

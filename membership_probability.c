//
//  main.c
//  this takes an input file containing the estimated parallax (plx)
//  the limits to be integrated between (lower_limit and upper_limit) and the error on the parallax
//  e_plx and integrates them numerically using the gsl qag integration package
//
//  Created by George Jerman on 29/09/2019.
// 
//  Note: any values for error in parallax of 0 should be scrubbed as this causes a divide by 0 error
//  Note: cannot deal with a 0 as a limit for some reason however this can be fixed by putting a 1 after lots of zeroes
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction
//, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_integration.h>

#define MAX_LENGTH  50 
#define NUM_DATA_COLUMMS 4
#define MAX_COL_WIDTH 15
#define WORKSPACE_SIZE 1000000
#define GSL_INTEG_GAUSS_61 6
#define EPSABS 1e-7
#define EPSREL 1e-7

double f(double x, void * params);
int size_of_file(FILE *source);

int main()
{
    
    FILE *source;
    FILE *destination;
    char source_file[MAX_LENGTH];
    char destination_file[MAX_LENGTH];


    //opening a user defined file to have the headers removed from it
    
    printf("Please input the file to be read\n");
    fgets(source_file, sizeof(source_file), stdin);
    
    // removes trailing newline from the filename
    
    strtok(source_file, "\n");
    source = fopen(source_file, "r");
    if (source == NULL)
    {
        perror("fopen");
        exit(1);
    }
    
    //opens a user defined output file
    
    printf("Please input the file to be written to\n");
    fgets(destination_file, sizeof(destination_file), stdin);
    
    // removes trailing newline from the filename
    
    strtok(destination_file, "\n");
    destination = fopen(destination_file, "w");

    //allocates the paramaters to arrays the size of the number of rows in the input file
    //int array_size = size_of_file(source);
    int array_size = size_of_file(source);
    
    double * plx = malloc(sizeof(double)*array_size);
    //double plx[array_size];
    double * lower_limit = malloc(sizeof(double)*array_size);
    //double lower_limit[array_size];
    double * upper_limit = malloc(sizeof(double)*array_size);
    //double upper_limit[array_size];
    double * e_plx = malloc(sizeof(double)*array_size);
    //double e_plx[array_size];
    
    char header[MAX_COL_WIDTH];

    
    int i,j;
    
    //takes the columns from the input file and stores them in the arrays declared above
    
    for (i=0; i<array_size; i++)
    {
        if (i==0) {
            j = fscanf(source, "%s %*s %*s %*s %*s",header);
            if (header[0]!='#') {
                printf("Header not of expected format / is missing\n");
                exit(1);
            }
        }
        else {
            j = fscanf(source, "%lf %lf %lf %lf", &plx[i], &lower_limit[i], &upper_limit[i], &e_plx[i]);
            if (j!=NUM_DATA_COLUMMS) {
                printf("%s", "Number of columns does not match input data");
                fclose(source);
                return 1;
            }
        }
    }
    fclose(source);

    //defines the gsl integration workspace structure (see the gsl docs for further info
    gsl_integration_workspace * w = gsl_integration_workspace_alloc(WORKSPACE_SIZE);
    double probability, error;
    int forcount;
    
    

    gsl_function F;
    F.function = &f;
    //iterates the integration over each member of the arrays
    //for loop starts at 1 to skip header
    for (forcount=1;forcount<array_size;forcount++) {
        double alpha = e_plx[forcount];
        F.params = &alpha;
    //uses the qag integration from the gsl library
        gsl_integration_qag(&F, lower_limit[forcount]-plx[forcount], upper_limit[forcount]-plx[forcount], EPSABS, EPSREL, WORKSPACE_SIZE, GSL_INTEG_GAUSS61, w, &probability, &error);

        fprintf (destination, "%.18lf %.18lf %.18lf %.18lf\n",plx[forcount], probability, e_plx[forcount], error);
     }


    gsl_integration_workspace_free (w);
    fclose(destination);
    free(plx);
    free(e_plx);
    free(lower_limit);
    free(upper_limit);
    return 0;
}

//retrieves the size of the input file and sets the array sizes to that.
int size_of_file(FILE *source)
{
    int c=0;
    int nl=0;
    fseek(source, 0, SEEK_SET);
    while ((EOF != (c=getc(source))))
    {
        if (c == '\n') {
            nl++;
            //printf("%d\n",nl);
        }
    }
    fseek(source, 0, SEEK_SET);
    return nl;
}

//defines the gaussian function to be integrated
double f (double x, void * params) {
        double alpha = *(double *) params;
        double f = (1/(alpha * sqrt(2*M_PI)) * exp(-(pow(x, 2)/(2*pow(alpha, 2))))) ;
        return f;
}

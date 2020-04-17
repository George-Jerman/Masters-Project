//
//  Output_Extractor.c
//  Gaussian Integrator
//
//  Created by George Jerman on 18/10/2019.
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction
//, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//This program reads in files in the format outputted by the membership probability calculator and returns a
//file that contains only members with a membership probability above a specified value. The output file format
//allows it to be read straight into the topcat plotting software.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_FILE_NAME 50
#define CUT_VAL 0.637 //The probabilty below which data points will be cut

int size_of_file(FILE *input);

int main(void)
{
    FILE * input;
    FILE * output;
    char input_filename[MAX_FILE_NAME];
    char output_filename[MAX_FILE_NAME];
    printf("Please input the filename\n");
    fgets(input_filename, MAX_FILE_NAME, stdin);
    strtok(input_filename, "\n");
    input = fopen(input_filename, "r");
    if (input == NULL)
    {
        perror("fopen");
        exit(1);
    }
    
    printf("Please input the output filename\n");
    fgets(output_filename, MAX_FILE_NAME, stdin);
    strtok(output_filename, "\n");
    output = fopen(output_filename, "w");
    
    int i,f;
    int array_size = size_of_file(input);
    double *plx = malloc(sizeof(double)*array_size);
    double *prob = malloc(sizeof(double)*array_size);
    double *prob_err = malloc(sizeof(double)*array_size);
    double *sigma = malloc(sizeof(double)*array_size);
    //double plx[array_size];
    //double prob[array_size];
    //double prob_err[array_size];
    //double sigma[array_size];
    
    for (i=0; i<array_size; i++) {
        f = fscanf(input,"%lg %lg %lg %lg", &plx[i], &prob[i],&sigma[i] ,&prob_err[i]);
        if (f != 4) {
            printf("Wrong number of columns.\n");
            exit(1);
        }
    }
    fclose(input);
    for (i=0; i<array_size; i++) {
        if (prob[i] > CUT_VAL) {
            plx[i] = plx[i];
            prob[i] = prob[i];
            prob_err[i] = prob_err[i];
            sigma[i] = sigma[i];
        }
        else
        {
            plx[i] = 0;
            prob[i]=0;
            prob_err[i]=0;
            sigma[i]=0;
        }
    }
    
    fprintf(output, "# Plx probability e_plx prob_error\n");
    for (i=0; i<(array_size); i++) {
        if (plx[i] != 0 ) {
            fprintf(output, "%.18lg %.18lg %.18lg %.18lg\n", plx[i], prob[i],sigma[i] ,prob_err[i]);
        }
        else{
            ;
        }
        }
    fclose(output);
    free(plx);
    free(prob);
    free(sigma);
    free(prob_err);
    return 0;
}

    

int size_of_file(FILE *input)
{
    int c=0;
    int nl=0;
    fseek(input, 0, SEEK_SET);
    while ((EOF != (c=getc(input))))
    {
        if (c == '\n') {
            nl++;
            //printf("%d\n",nl);
        }
    }
    fseek(input, 0, SEEK_SET);
    return nl;
}

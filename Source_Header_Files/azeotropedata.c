/* This file reads through the .txt file "azeotrope_data.text"
 and warns the user if the mixture has an azeotrope */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "main.h"
extern int species;
int warning;

void azeotrope_search(void)
{
    int i,j;
    warning = 0;
    float boiling_point_a, boiling_point_b, fraction_a, boiling_point_azeotrope;
    char component_a[50];
    char component_b[50];
    FILE *file_ptr;
    for(i = 0; i < species ;i++)
    {
        file_ptr = fopen("azeotrope_data.txt", "r");
        if(file_ptr == NULL)
        {
            printf("Could Not Open azeotrope_data.txt\nEXIT PROGRAM\n");
            exit(9);
        }
        while(fscanf(file_ptr, "%s %s %f %f %f %f", component_a, component_b, &boiling_point_a, &boiling_point_b, &boiling_point_azeotrope, &fraction_a) != EOF)
        {
            if(strcmp(f_array[i]-> chemical, component_a) == 0 )
            {
                for(j = 0; j < species; j++)
                {
                    if(strcmp(f_array[j]-> chemical, component_b) == 0 && j != i)
                    {
                        strcpy(f_array[j]-> az_ptr -> chemical_a, component_a);
                        strcpy(f_array[j]-> az_ptr -> chemical_b, component_b);
                        f_array[j]-> az_ptr -> boiling_temperature_a = boiling_point_a;
                        f_array[j]-> az_ptr -> boiling_temperature_b = boiling_point_b;
                        f_array[j]-> az_ptr -> fraction_a = fraction_a;
                        f_array[j]-> az_ptr -> fraction_b = 1 - fraction_a;
                        f_array[j]-> az_ptr -> azeotrope_boiling_temperature = boiling_point_azeotrope;
                        if(warning == 0)
                        {
                            warning = 1;
                        }
                        break;
                    }
                }
            }
            if(strcmp(f_array[i]-> chemical, component_b) == 0 )
            {
                for(j = 0; j < species; j++)
                {
                    if(strcmp(f_array[j]-> chemical, component_a) == 0 && j != i)
                    {
                        f_array[j]-> az_ptr = malloc(sizeof(azeotrope_struct*));
                        strcpy(f_array[j]-> az_ptr -> chemical_a, component_a);
                        strcpy(f_array[j]-> az_ptr -> chemical_b, component_b);
                        f_array[j]-> az_ptr -> boiling_temperature_a = boiling_point_a;
                        f_array[j]-> az_ptr -> boiling_temperature_b = boiling_point_b;
                        f_array[j]-> az_ptr -> fraction_a = fraction_a;
                        f_array[j]-> az_ptr -> fraction_b = 1 - fraction_a;
                        f_array[j]-> az_ptr -> azeotrope_boiling_temperature = boiling_point_azeotrope;
                        if(warning == 0)
                        {
                            warning = 1;
                        }
                        break;
                    }
                }
            }
        }
        fclose(file_ptr);
    }
    return;
}

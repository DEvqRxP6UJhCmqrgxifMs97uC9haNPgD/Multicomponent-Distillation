/* This file places data from files and calculated
 results into the correct data stucture. This file
 also allocates memory for the data structures
 dynamically */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main.h"

extern int models_done;
extern char light_key[50];
extern char heavy_key[50];
extern antoine_struct *a_ptr;
extern unsigned long species;
extern unsigned long numer_of_antoine_data_for_species[100];

long double total_pressure;
void remove_links(void);
void azeotrope_creation(void);
void structure_creation(void);
void feed_array_creation(void);
void insert_antoine_data(void);
void flash_array_creation(void);
void batch_array_creation(void);
void antoine_array_creation(void);
void fractional_distillation_array_creation(void);

void structure_creation(void)
{
    feed_array_creation();
    antoine_array_creation();
    remove_links();
    azeotrope_creation();
    return;
}

void feed_array_creation(void)
{
    int i;
    antoine_struct *new_a_ptr = a_ptr;
    f_array = NULL;
    f_array = (feed_struct**)malloc(sizeof(feed_struct **)*(species+1));
    if(f_array == NULL)
    {
        printf("INSUFFICIENT MEMORY TO CREATE FEED ARRAY\nEXIT PROGRAM\n");
        exit(9);
    }
    for(i = 0; i < species; i++)
    {
        f_array[i] = (feed_struct*)malloc(sizeof(feed_struct));
        if(f_array[i] == NULL)
        {
            printf("INSUFFICIENT MEMORY TO ADD FEED STRUCTURE\nEXIT PROGRAM\n");
            exit(9);
        }
        if(i == 0)
        {
            strcpy(f_array[i] -> chemical, a_ptr -> chemical);
        }
        if(i != 0)
        {
            if(strcmp(f_array[i-1] -> chemical, a_ptr -> chemical) != 0)
            {
                strcpy(f_array[i] -> chemical, a_ptr -> chemical);
            }
            else
            {
                i--;
            }
        }
        a_ptr=a_ptr->next;
    }
    a_ptr = new_a_ptr;
    f_array[i] = NULL;
    return;
}

void antoine_array_creation(void)
{
    int i, j;
    antoine_struct *new_a_ptr = a_ptr;
    for(i = 0; i < species; i++)
    {
        a_array = NULL;
        a_array = (antoine_struct**)malloc(sizeof(antoine_struct **) * numer_of_antoine_data_for_species[i]);
        if(a_array == NULL)
        {
            printf("INSUFFICIENT MEMORY TO CREATE ANTOINE ARRAY\nEXIT PROGRAM\n");
            exit(9);
        }
        f_array[i] -> a_array = a_array;
        for(j = 0; j < numer_of_antoine_data_for_species[i]; j++)
        {
            f_array[i] -> a_array[j] = a_ptr;
            a_ptr = a_ptr -> next;
        }
        f_array[i] -> a_array[j] = NULL;
    }
    a_ptr = new_a_ptr;
    f_array[i] = NULL;
    return;
}

void azeotrope_creation(void)
{
    int i;
    azeotrope_struct *az_ptr;
    for(i = 0; i < species; i++)
    {
       az_ptr = (azeotrope_struct*)malloc(sizeof(azeotrope_struct));
        if( az_ptr == NULL)
        {
            printf("INSUFFICIENT MEMORY TO ADD AZEOTROPE STRUCTURE\nEXIT PROGRAM\n");
            exit(9);
        }
        f_array[i] -> az_ptr = az_ptr;
    }
    return;
}

void flash_array_creation(void)
{
    int i, j;
    for(i = 0; i < species; i++)
    {
        v_array = NULL;
        v_array = (vaporization_struct**)malloc(sizeof(vaporization_struct **) * 3.0);
        if(v_array == NULL)
        {
            printf("INSUFFICIENT MEMORY TO CREATE VAPORIZATION ARRAY\nEXIT PROGRAM\n");
            exit(9);
        }
        f_array[i] -> v_array = v_array;
        for(j = 0; j < 2; j++)
        {
            f_array[i] -> v_array[j] = (vaporization_struct *)malloc(sizeof(vaporization_struct));
            if(f_array[i] == NULL)
            {
                printf("INSUFFICIENT MEMORY TO ADD VAPORIZATION STRUCTURE\nEXIT PROGRAM\n");
                exit(9);
            }
            strcpy(f_array[i] -> v_array[j] -> chemical, f_array[i] -> chemical);
            f_array[i] -> v_array[j] -> total_pressure = total_pressure;
        }
        f_array[i] -> v_array[j] = NULL;
    }
    return;
}

void batch_array_creation(void)
{
    int i, j;
    b_array = NULL;
    for(i = 0; i < species; i++)
    {
        b_array = (batch_struct**)malloc(sizeof(batch_struct **) * 2.0);
        if(b_array == NULL)
        {
            printf("INSUFFICIENT MEMORY TO CREATE BATCH ARRAY\nEXIT PROGRAM\n");
            exit(9);
        }
        f_array[i] -> b_array = b_array;
        for(j = 0; j < 1; j++)
        {
            f_array[i] -> b_array[j] = (batch_struct *)malloc(sizeof(batch_struct));
            if(f_array[i] == NULL)
            {
                printf("INSUFFICIENT MEMORY TO ADD BATCH STRUCTURE\nEXIT PROGRAM\n");
                exit(9);
            }
            strcpy(f_array[i] -> b_array[j] -> chemical, f_array[i] -> chemical);
            f_array[i] -> b_array[j] -> total_pressure = total_pressure;
        }
        f_array[i] -> b_array[j] = NULL;
    }
    return;
}

void fractional_distillation_array_creation(void)
{
    int i, j;
    fd_array = NULL;
    for(i = 0; i < species; i++)
    {
        fd_array = (fractional_distillation_struct**)malloc(sizeof(fractional_distillation_struct **) * 2.0);
        if(fd_array == NULL)
        {
            printf("INSUFFICIENT MEMORY TO ADD DATA\nEXIT PROGRAM\n");
            exit(9);
        }
        f_array[i] -> fd_array = fd_array;
        for(j = 0; j < 1; j++)
        {
            f_array[i] -> fd_array[j] = (fractional_distillation_struct *)malloc(sizeof(fractional_distillation_struct));
            if(f_array[i] == NULL)
            {
                printf("INSUFFICIENT MEMORY TO ADD DATA\nEXIT PROGRAM\n");
                exit(9);
            }
            
        }
        f_array[i] -> fd_array[j] = NULL;
    }
    return;
}

void remove_links(void)
{
    int i, j;
    for(i = 0; i < species; i++)
    {
        for(j = 0; j < numer_of_antoine_data_for_species[i]; j++)
        {
            f_array[i] -> a_array[j]->next = NULL;
        }
        f_array[i] -> a_array[j] = NULL;
    }
    return;
}

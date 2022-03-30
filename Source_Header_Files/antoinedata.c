/* This file reads through the .txt file "antoine_data.txt"
 and extracts the correct antoine coefficients for the problem */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

extern int models_done;
extern void feed_user(void);
extern void print_page_clean(void);
extern void structure_creation(void);
extern int user_yes_no (void);

antoine_struct *a_ptr;
char rough_search[50];
char a_user_input[50];
int longest_chemical = 0;
unsigned long species = 0;
unsigned long temp_counter = 0;
unsigned long numer_of_antoine_data_for_species[100] = {0};
void a_rough_add(long double a, long double b, long double c, long double t_min, long double t_max);
void a_rough_insert(FILE *file_ptr);
void rough_data_read(void);
void numer_of_antoine_data_for_species_reversal(void);

void data_user(void)
{
    int i;
    printf("Step 1: Input Feed Chemical Composition\n");
    if(species == 0)
    {
        goto first_input;
    }
    while(user_yes_no() == 1)
    {
    first_input: printf("Enter Chemical Component %lu: ",species+1);
        scanf("%s", a_user_input);
        for(i = 0; i < strlen(a_user_input); i++)
        {
            a_user_input[i] = tolower(a_user_input[i]);
        }
        /*antoine_options = 0;*/
        temp_counter = 0;
        species++;
        rough_data_read();
        printf("\nWould You Like To Add Another Chemical Component? ");
    }
    print_page_clean();
    numer_of_antoine_data_for_species_reversal();
    structure_creation();
    feed_user();
    return;
}

void rough_data_read(void)
{
    int i = 0;
    int found_chemical = 0;
    FILE *file_ptr;
    char previous = '\0';
    file_ptr = fopen("antoine_data.txt", "r");
    if(file_ptr == NULL)
    {
        printf("Error Opening antoine_data.text\nEXIT PROGRAM\n");
        exit(9);
    }
    while(fscanf(file_ptr, "%c", &rough_search[i]) != EOF)
    {
        rough_search[i] = tolower(rough_search[i]);
        if(rough_search[i] != a_user_input[i]) {
            if(i == 0)
            {
                previous = rough_search[i];
            }
            i = 0;
            memset(rough_search, 0, sizeof(rough_search));
        }
        if(rough_search[i] != ' ' && a_user_input[i] == '\0')
        {
            i = 0;
            memset(rough_search, 0, sizeof(rough_search));
        }
        if(rough_search[i] == a_user_input[i]) {
            i++;
        }
        if(rough_search[i] == '\0' && a_user_input[i] == '\0' && previous == '\n')
        {
            found_chemical = 1;
            temp_counter++;
            a_rough_insert(file_ptr);
            fclose(file_ptr);
            i = 0;
            memset(rough_search, 0, sizeof(rough_search));
        }
    }
    if(found_chemical == 0)
    {
        printf("Chemical Was Not Found In Database.\n");
        species--;
        fclose(file_ptr);
    }
    if(found_chemical == 1)
    {
        numer_of_antoine_data_for_species[species-1] = temp_counter;
        if(strlen(a_user_input) > longest_chemical)
        {
            longest_chemical = strlen(a_user_input);
        }
    }
    found_chemical = 0;
    return;
}

void a_rough_insert(FILE *file_ptr)
{
    long double a, b, c, t_min, t_max;
    fscanf(file_ptr, "%Lf %Lf %Lf %Lf %Lf", &a, &b, &c, &t_min, &t_max);
    fclose(file_ptr);
    a_rough_add(a, b, c, t_min, t_max);
    return;
}

void a_rough_add(long double a, long double b, long double c, long double t_min, long double t_max)
{
    antoine_struct *new_a_ptr = NULL;
    new_a_ptr = (antoine_struct*)malloc(sizeof(antoine_struct));
    if(new_a_ptr == NULL)
    {
       printf("INSUFFICIENT MEMORY TO ADD DATA\nEXIT PROGRAM\n");
        exit(9);
    }
    new_a_ptr -> next = a_ptr;
    a_ptr = new_a_ptr;
    strcpy(a_ptr -> chemical, rough_search);
    a_ptr -> antoine_a = a;
    a_ptr -> antoine_b = b;
    a_ptr -> antoine_c = c;
    a_ptr -> t_min = t_min;
    a_ptr -> t_max = t_max;
    return;
}

void numer_of_antoine_data_for_species_reversal(void)
{
    int i;
    unsigned long temp;
    for(i = 0; i < species/2; i++)
    {
        temp = numer_of_antoine_data_for_species[i];
        numer_of_antoine_data_for_species[i] = numer_of_antoine_data_for_species[species-1-i];
        numer_of_antoine_data_for_species[species-1-i] = temp;
    }
    return;
}

/* This file evaluates user inputs and ensures
 they are correct and returns the confirmation
 values */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "user.h"

extern char chemical[50];
extern int species;
extern long double x_feed;
extern long double total_pressure;

int models_done = -1;
int model_choice (void);
int user_yes_no (void);
int user_batch_chemical(char *chemical);
long double pos_frac_check (long double temp_answer);
long double pos_number_check(long double temp_answer);
void not_valid_yes_no (void);
void convert_pressure(void);

int model_choice (void)
{
    
    if(models_done == -1)
    {
        printf ("Is this a flash system: ");
        if (user_yes_no () == 1)
        {
            models_done = 1 * models_done;
            return 1;
        }
    }
    
    if(models_done % 2 != 0 && species == 2)
    {
        if( models_done < 0)
        {
            printf ("Is This A Batch Distillation System: ");
        }
        else
        {
            printf("Would You Like To Remodel The System As A Batch Distillation System? ");
        }
        if (user_yes_no () == 1)
        {
            models_done = 2 * models_done;
            return 2;
        }
    }
    
    models_done = 2 * models_done;
    
    if(models_done % 3 != 0)
    {
        if( models_done < 0 )
        {
            printf ("Is This A Continuous Multicomponent Distillation System: ");
        }
        else
        {
            printf("Would You Like To Remodel The System As A Fractional Distillation System? ");
        }
        if (user_yes_no () == 1)
        {
            models_done = 3 * models_done;
            return 3;
        }
    }
    models_done = 3 * models_done;
    return 0;
}

int user_yes_no (void)
{
    char yes_no[5];
    char yes[] = "yes";
    char no[] = "no";
    int i;
    scanf("%s", yes_no);
    for (i = 0; yes_no[i] != '\0'; i++)
    {
        yes_no[i] = tolower (yes_no[i]);
    }
    if (strcmp (yes_no, yes) != 0 && strcmp (yes_no, no) != 0)
    {
        not_valid_yes_no ();
    }
    if (strcmp (yes_no, yes) == 0)
    {
        return 1;
    }
    if (strcmp (yes_no, no) == 0)
    {
        return 0;
    }
    return -999;
}

void not_valid_yes_no (void)
{
    do
    {
        printf ("Please enter either yes or no: ");
    } while (user_yes_no () == -999);
    
    return;
}

long double pos_frac_check(long double temp_answer)
{
    int i = 0;
    if (temp_answer > 0 && temp_answer < 1)
    {
        return temp_answer;
    }
    while (temp_answer < 0 || temp_answer > 1 || temp_answer == 0)
    {
        printf ("Please Enter A Valid Fraction (Number Between 0 And 1): ");
        scanf ("%Lf", &temp_answer);
        printf("\n");
        if (temp_answer > 0 && temp_answer < 1)
        {
            return temp_answer;
        }
        if(i > 20)
        {
            printf("Valid Fraction Was Not Input\nEXIT PROGRAM\n");
            exit(0);
        }
        i++;
    }
    return 0;
}

long double pos_number_check(long double temp_answer)
{
    return temp_answer;
}

int user_batch_chemical(char *chemical)
{
    int i;
    for(i = 0; f_array[i] != NULL; i++)
    {
        if(strcmp(f_array[i] -> chemical, chemical) == 0)
        {
            return i;
        }
    }
    return -1;
}

void convert_pressure(void)
{
    int i;
    char pressure_units[50];
    char mmgh[] = "mmgh";
    char atm[] = "atm";
    char bar[] = "bar";
    char torr[] = "torr";
    char pa[] = "pa";
    char kpa[] = "kpa";
    char psi[] = "psi";
    char psf[] = "psf";
    char inh2o[] = "inh2o";
    
    printf("    mmHg\n");
    printf("    atm\n");
    printf("    bar\n");
    printf("    Torr\n");
    printf("    Pa\n");
    printf("    kPa\n");
    printf("    psi\n");
    printf("    psf\n");
    printf("    inH2O\n");
    for( ; ; )
    {
        printf("What Are The Units Of The System Pressure? ");
        scanf("%s", pressure_units);
        for(i = 0; i < strlen(pressure_units); i++)
        {
            pressure_units[i] = tolower(pressure_units[i]);
        }
        if(strcmp(pressure_units, mmgh) == 0)
        {
            return;
        }
        if(strcmp(pressure_units, atm) == 0)
        {
            total_pressure *= 760;
            return;
        }
        if(strcmp(pressure_units, bar) == 0)
        {
            total_pressure /= 0.0013332239;
            return;
        }
        if(strcmp(pressure_units, torr) == 0)
        {
            total_pressure *= 1.0000001618554;
            return;
        }
        if(strcmp(pressure_units, pa) == 0)
        {
            total_pressure *= 133.322;
            return;
        }
        if(strcmp(pressure_units, kpa) == 0)
        {
            total_pressure *= (760 / 101.325);
            return;
        }
        if(strcmp(pressure_units, psi) == 0)
        {
            total_pressure *= 51.7149326;
            return;
        }
        if(strcmp(pressure_units, psf) == 0)
        {
            total_pressure *= 0.359131;
            return;
        }
        if(strcmp(pressure_units, inh2o) == 0)
        {
            total_pressure *= 1.8683201548767;
            return;
        }
        else
        {
            printf("Please Enter One Of The Listed Units.\n");
        }
    }
    return;
}

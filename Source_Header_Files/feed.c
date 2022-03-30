/* This file gets feed conditions from the user */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main.h"

extern int models_done;
extern int longest_chemical;
extern long double total_pressure;
extern void in_bounds_antoine_data(long double temperature);
extern long double pos_frac_check(long double);
extern long double pos_number_check(long double temp_answer);
extern long double in_bounds_temperature_data[300];

void feed_user(void);
void boiling_point_pure_substance(void);

void feed_user(void)
{
    int i,j;
    long double feed_frac, feed_mass;
    if(abs(models_done) == 1)
    {
        printf("How Many Moles Are Fed To The Column: ");
    }
    if(abs(models_done) == 2)
    {
        printf("How Many Moles Are Charged To The Still: ");
    }
    if(models_done % 3 == 0)
    {
    printf("What Is The Molar Feed Rate: ");
    }
    scanf("%Lf", &feed_mass);
    feed_mass = pos_number_check(feed_mass);
    for(i = 0; f_array[i] != NULL; i++)
    {
        printf("What Fraction Of The Feed Is %s", f_array[i]->chemical);
        if(longest_chemical >= 11)
        {
            for(j=0; j < (longest_chemical-strlen(f_array[i]->chemical));j++)
            {
                printf(" ");
            }
        }
        else
        {
            for(j=0; j < 11-strlen(f_array[i]->chemical);j++)
            {
                printf(" ");
            }
        }
        printf(": ");
        scanf("%Lf", &feed_frac);
        feed_frac = pos_frac_check(feed_frac);
        f_array[i] -> total_feed_mass = feed_mass;
        f_array[i] -> feed_frac = feed_frac;
        f_array[i] -> feed_mass = f_array[i] -> feed_frac * feed_mass;
    }
    return;
}

void boiling_point_pure_substance(void)
{
    int i;
    int failed = 0;
    long double temperature;
    long double saturated_pressure;
    static int pure_boiling_reattempt;
    
    for(i = 0; f_array[i] != NULL; i++)
    {
        saturated_pressure = 0;
        for(temperature = -273.15; saturated_pressure < total_pressure; temperature = temperature + .005)
        {
            in_bounds_antoine_data(temperature);
            if((temperature + in_bounds_temperature_data[i*5+2]) > 0)
            {
                saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
            }
            if(temperature > 5555)
            {
                printf("There Was An Error Calculating The Boiling Point Of Pure %s.\nThe Model Limit Is 5555 Celsius\n", f_array[i] -> chemical);
                if(pure_boiling_reattempt == 0)
                {
                    pure_boiling_reattempt++;
                    failed = i;
                    printf("Attempting Recalculation\n");
                    boiling_point_pure_substance();
                }
                if(pure_boiling_reattempt != 0 && temperature > 5555)
                {
                    printf("PURE %s BOILING TEMPERATURE RECALCULATION ATTEMPT FAILED.\n EXIT PROGRAM\n", f_array[i] -> chemical);
                    exit(9);
                }
                break;
            }
        }
        if(pure_boiling_reattempt == -999)
        {
            printf("The Recalculation Of %s's Boiling Point Was A Success\n", f_array[failed] -> chemical);
        }
        f_array[i] -> pure_boiling_temperature = temperature;
    }
    return;
}

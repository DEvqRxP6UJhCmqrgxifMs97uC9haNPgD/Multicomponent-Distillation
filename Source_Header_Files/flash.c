/* This file does the flash calculation on
 the provided feed stream. It also gets user
 input about the system */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "main.h"

extern int light_location;
extern int heavy_location;
extern int models_done;
extern char chemical[50];
extern char light_key[50];
extern char heavy_key[50];
extern long double x_bottom_light;
extern long double x_bottom_heavy;
extern long double x_distillate_light;
extern long double x_distillate_heavy;
extern long double mass_bottom;
extern long double mass_distillate;
extern long double pos_frac_check(long double);
extern long double pos_number_check(long double);

extern int species;
extern long double total_pressure;
extern void flash_array_creation(void);
extern int user_yes_no(void);
extern int user_batch_chemical(char *chemical);
extern void convert_pressure(void);
extern void boiling_point_pure_substance(void);

void flash_user(void);
void boiling_equilibrium_temperature_calculation(void);
void dew_equilibrium_temperature_calculation(void);
void dew_equilibrium_composition(void);
void boiling_equilibrium_composition(void);
void vaporization_calculation(void);
void vaporization_composition_function(void);
void in_bounds_antoine_data(long double temperature);

long double fraction_vaporized;
long double boiling_temperature, dew_temperature, vaporization_temperature, dew_distribution_ratio, boiling_distribution_ratio;
long double in_bounds_temperature_data[300] = {0};

/*long double temperature = -273.15;*/
int dew_reattempt = 0;
int boil_reattempt = 0;
int vaporization_reattempt = 0;

void flash_user(void)
{
    printf("\nWhat Is The Total Pressure Of The System: ");
    scanf("%Lf", &total_pressure);
    total_pressure = pos_number_check(total_pressure);
    printf("Is This Pressure In mmHg? ");
    if(user_yes_no() == 0)
    {
        convert_pressure();
    }
    printf("What Fraction Of The Feed Vaporized     : ");
    scanf("%Lf", &fraction_vaporized);
    fraction_vaporized = pos_frac_check(fraction_vaporized);
    boiling_point_pure_substance();
    flash_array_creation();
    boiling_equilibrium_temperature_calculation();
    dew_equilibrium_temperature_calculation();
    dew_equilibrium_composition();
    boiling_equilibrium_composition();
    vaporization_calculation();
    vaporization_composition_function();
    return;
}

void boiling_equilibrium_temperature_calculation(void)
{
    int i = 0;
    long double summed = 0;
    long double saturated_pressure, distribution_ratio, temperature;
    mass_bottom = 0;
    
   if(models_done % 3 == 0)
    {
        light_location = user_batch_chemical(light_key);
        heavy_location = user_batch_chemical(heavy_key);
        for(i=0; f_array[i] != NULL; i++)
        {
            if(f_array[heavy_location] -> pure_boiling_temperature < f_array[i] -> pure_boiling_temperature)
            {
                mass_bottom += f_array[i] -> feed_mass;
            }
        }
        mass_bottom += x_bottom_heavy*f_array[heavy_location]-> feed_mass;
        mass_bottom += x_bottom_light*f_array[light_location]-> feed_mass;
        for(i=0; f_array[i] != NULL; i++)
        {
            if(f_array[heavy_location] -> pure_boiling_temperature < f_array[i] -> pure_boiling_temperature)
            {
               f_array[i]-> fd_array[0] -> bottoms_frac = f_array[i] -> feed_mass/mass_bottom;
            }
        }
        f_array[heavy_location]-> fd_array[0] -> bottoms_frac = x_bottom_heavy*f_array[heavy_location]-> feed_mass/mass_bottom;
        f_array[light_location]-> fd_array[0] -> bottoms_frac = x_bottom_light*f_array[light_location]-> feed_mass/mass_bottom;
    } 
    
    for(temperature = -273.15; summed < 1; temperature = temperature + .005)
    {
        summed = 0;
        in_bounds_antoine_data(temperature);
        for(i = 0; i < species; i++)
        {
            if((temperature + in_bounds_temperature_data[i*5+2]) > 0 && models_done % 3 != 0)
            {
                saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
                distribution_ratio = saturated_pressure / total_pressure;
                summed += f_array[i] -> feed_frac * distribution_ratio;
            }
            else if((temperature + in_bounds_temperature_data[i*5+2]) > 0 && models_done % 3 == 0)
            {
                if(f_array[light_location] -> pure_boiling_temperature <= f_array[i] -> pure_boiling_temperature)
                {
                    
                    saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
                    distribution_ratio = saturated_pressure / total_pressure;
                    if (i == heavy_location)
                    {
                        summed += ((f_array[heavy_location]-> feed_mass * x_bottom_heavy)/mass_bottom)* distribution_ratio;
                    }
                    if (i == light_location)
                    {
                        summed += ((f_array[light_location]-> feed_mass * x_bottom_light)/mass_bottom)* distribution_ratio;
                    }
                    if (i != light_location && i != heavy_location)
                    {
                        summed += (f_array[i]-> feed_mass /mass_bottom)* distribution_ratio;
                    }
                }
            }
            else
            {
                saturated_pressure = 1E-10;
                distribution_ratio = saturated_pressure / total_pressure;
                summed += f_array[i] -> feed_frac * distribution_ratio;
            }
        }
        if(temperature > 5555)
        {
            printf("There Was An Error Calculating The Boiling Temperature.\nThe Model Limit Is 5555 Celsius\n");
            if(boil_reattempt == 0)
            {
                boil_reattempt = -999;
                printf("Attempting Recalculation\n");
                boiling_equilibrium_temperature_calculation();
            }
            if(boil_reattempt != 0 && temperature > 5555)
            {
                printf("BUBBLE TEMPERATURE RECALCULATION ATTEMPT FAILED.\n EXIT PROGRAM\n");
                exit(9);
            }
            break;
        }
    }
    boiling_temperature = temperature;
    if(models_done % 3 == 0)
    {
        for(i = 0; i < species; i++)
        {
            f_array[i]->fd_array[0]->boiling_antoine_a = in_bounds_temperature_data[i*5];
            f_array[i]->fd_array[0]->boiling_antoine_b = in_bounds_temperature_data[i*5+1];
            f_array[i]->fd_array[0]->boiling_antoine_c = in_bounds_temperature_data[i*5+2];
            f_array[i]->v_array[0]->boiling_antoine_min_temp = in_bounds_temperature_data[i*5+3];
            f_array[i]->v_array[0]->boiling_antoine_max_temp = in_bounds_temperature_data[i*5+4];
        }
    }
    if(models_done % 3 != 0)
    {
        for(i = 0; i < species; i++)
        {
            f_array[i]->v_array[0]->boiling_antoine_a = in_bounds_temperature_data[i*5];
            f_array[i]->v_array[0]->boiling_antoine_b = in_bounds_temperature_data[i*5+1];
            f_array[i]->v_array[0]->boiling_antoine_c = in_bounds_temperature_data[i*5+2];
            f_array[i]->v_array[0]->boiling_antoine_min_temp = in_bounds_temperature_data[i*5+3];
            f_array[i]->v_array[0]->boiling_antoine_max_temp = in_bounds_temperature_data[i*5+4];
        }
    }
}

void dew_equilibrium_temperature_calculation(void)
{
    int i = 0;
    long double summed = 999;
    long double temperature;
    long double saturated_pressure, distribution_ratio;
    mass_distillate = 0;
    
    if(models_done % 3 == 0)
    {
        light_location = user_batch_chemical(light_key);
        heavy_location = user_batch_chemical(heavy_key);
        for(i=0; f_array[i] != NULL; i++)
        {
            if(f_array[light_location] -> pure_boiling_temperature > f_array[i] -> pure_boiling_temperature)
            {
                mass_distillate += f_array[i] -> feed_mass;
            }
        }
        mass_distillate = mass_distillate + x_distillate_heavy*f_array[heavy_location]-> feed_mass;
        mass_distillate = mass_distillate + x_distillate_light*f_array[light_location]->feed_mass;
        for(i=0; f_array[i] != NULL; i++)
        {
            if(f_array[light_location] -> pure_boiling_temperature > f_array[i] -> pure_boiling_temperature)
            {
                f_array[i]-> fd_array[0] -> distillate_frac = f_array[i] -> feed_mass/mass_distillate;
            }
        }
        f_array[heavy_location]-> fd_array[0] -> distillate_frac = x_distillate_heavy*f_array[heavy_location]-> feed_mass/mass_distillate;
        f_array[light_location]-> fd_array[0] -> distillate_frac = x_distillate_light*f_array[light_location]-> feed_mass/mass_distillate;
    }
    
    for(temperature = -273.15; summed > 1; temperature = temperature + 0.005)
    {
        summed = 0;
        in_bounds_antoine_data(temperature);
        for(i = 0; i < species; i++)
        {
            if((temperature + in_bounds_temperature_data[i*5+2]) > 0 && models_done % 3 != 0)
            {
                saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
                distribution_ratio = saturated_pressure / total_pressure;
                summed += f_array[i] -> feed_frac / distribution_ratio;
            }
            else if((temperature + in_bounds_temperature_data[i*5+2]) > 0 && models_done % 3 == 0)
            {
                if(f_array[heavy_location] -> pure_boiling_temperature >= f_array[i] -> pure_boiling_temperature)
                {
                    
                    saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
                    distribution_ratio = saturated_pressure / total_pressure;
                    if (i == heavy_location)
                    {
                        summed += ((f_array[heavy_location]-> feed_mass * x_distillate_heavy)/mass_distillate)/ distribution_ratio;
                    }
                    if (i == light_location)
                    {
                         summed += ((f_array[light_location]-> feed_mass * x_distillate_light)/mass_distillate)/ distribution_ratio;
                    }
                    if (i != light_location && i != heavy_location)
                    {
                        summed += (f_array[i]-> feed_mass /mass_distillate)/ distribution_ratio;
                    }
                }
            }
            else
            {
                saturated_pressure = 1E-10;
                distribution_ratio = saturated_pressure / total_pressure;
                summed += f_array[i] -> feed_frac / distribution_ratio;
            }
        }
        if(temperature > 5555)
        {
            printf("There Was An Error Calculating The Dew Temperature.\nThe Model Limit Is 5555 Celsius\n");
            if(dew_reattempt == 0)
            {
                printf("Attempting Recalculation\n");
                dew_reattempt = -999;
                dew_equilibrium_temperature_calculation();
            }
            if(temperature > 5555 && dew_reattempt != 0)
            {
                printf("DEW TEMPERATURE RECALCULATION ATTEMPT FAILED.\n EXIT PROGRAM\n");
                exit(9);
            }
            break;
        }
    }
    dew_temperature = temperature;
    if(models_done % 3 == 0)
    {
        for(i = 0; i < species; i++)
        {
            f_array[i]->fd_array[0]->dew_antoine_a = in_bounds_temperature_data[i*5];
            f_array[i]->fd_array[0]->dew_antoine_b = in_bounds_temperature_data[i*5+1];
            f_array[i]->fd_array[0]->dew_antoine_c = in_bounds_temperature_data[i*5+2];
            f_array[i]->v_array[0]->dew_antoine_min_temp = in_bounds_temperature_data[i*5+3];
            f_array[i]->v_array[0]->dew_antoine_max_temp = in_bounds_temperature_data[i*5+4];
        }
    }
    if(models_done % 3 != 0)
    {
        for(i = 0; i < species; i++)
        {
            f_array[i]->v_array[0]->dew_antoine_a = in_bounds_temperature_data[i*5];
            f_array[i]->v_array[0]->dew_antoine_b = in_bounds_temperature_data[i*5+1];
            f_array[i]->v_array[0]->dew_antoine_c = in_bounds_temperature_data[i*5+2];
            f_array[i]->v_array[0]->dew_antoine_min_temp = in_bounds_temperature_data[i*5+3];
            f_array[i]->v_array[0]->dew_antoine_max_temp = in_bounds_temperature_data[i*5+4];
        }
    }
    return;
}


void dew_equilibrium_composition(void)
{
    int i;
    long double saturated_pressure;
    in_bounds_antoine_data(dew_temperature);
    for(i = 0; i < species; i++)
    {
        saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+dew_temperature));
        strcpy(f_array[i] -> v_array[0] -> type, "Equilibrium");
        f_array[i] -> v_array[0] -> vaporization_frac = -999;
        f_array[i] -> v_array[0] -> vaporization_temperature = -999;
        f_array[i] -> v_array[0] -> dew_temperature = dew_temperature;
        f_array[i] -> v_array[0] -> liquid_frac = f_array[i] -> feed_frac * total_pressure / saturated_pressure;
    }
    return;
}

void boiling_equilibrium_composition(void)
{
    int i;
    long double saturated_pressure;
    in_bounds_antoine_data(boiling_temperature);
    for(i = 0; i < species; i++)
    {
        saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+boiling_temperature));
        f_array[i] -> v_array[0] -> boiling_temperature = boiling_temperature;
        f_array[i] -> v_array[0] -> vapor_frac = f_array[i] -> feed_frac * saturated_pressure/total_pressure;
    }
    return;
}


void vaporization_calculation(void)
{
    
    int i = 0;
    long double summed = 1;
    long double temperature;
    long double saturated_pressure, distribution_ratio;
    
    for(temperature = -273.15; summed > 0; temperature = temperature + 0.005)
    {
        summed = 0.0;
        in_bounds_antoine_data(temperature);
        for(i = 0; i < species; i++)
        {
            if((temperature + in_bounds_temperature_data[i*5+2]) > 0)
            {
                saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+temperature));
            }
            else
            {
                summed = 999;
                break;
            }
            distribution_ratio = saturated_pressure / total_pressure;
            summed += (f_array[i] -> feed_frac * (1.0-distribution_ratio))/(1.0+fraction_vaporized*(distribution_ratio - 1.0));
        }
        if(temperature > 5555)
        {
            printf("There Was An Error Calculating The Vaporization Temperature.\nThe Model Limit Is 5555 Celsius\n");
            if(vaporization_reattempt == 0)
            {
                printf("Attempting Recalculation\n");
                vaporization_reattempt = -999;
                vaporization_calculation();
            }
            if(temperature > 5555 && vaporization_reattempt != 0)
            {
                printf("VAPORIZATION TEMPERATURE RECALCULATION ATTEMPT FAILED.\n EXIT PROGRAM\n");
                exit(9);
            }
            break;
        }
    }
    vaporization_temperature = temperature;
    return;
}

void vaporization_composition_function(void)
{
    int i;
    long double saturated_pressure, distribution_ratio;
    in_bounds_antoine_data(vaporization_temperature);
    for(i = 0; i < species; i++)
    {
        saturated_pressure = pow(10, in_bounds_temperature_data[i*5]-in_bounds_temperature_data[i*5+1]/(in_bounds_temperature_data[i*5+2]+vaporization_temperature));
        distribution_ratio = saturated_pressure / total_pressure;
        strcpy(f_array[i] -> v_array[1] -> type, "Rachford-Rice");
        f_array[i] -> v_array[1] -> vaporization_frac = fraction_vaporized;
        f_array[i] -> v_array[1] -> vaporization_temperature = vaporization_temperature;
        f_array[i] -> v_array[1] -> dew_temperature = -999;
        f_array[i] -> v_array[1] -> liquid_frac = f_array[i] -> feed_frac/ (1 + fraction_vaporized*(distribution_ratio-1));
        f_array[i] -> v_array[1] -> boiling_temperature = -999;
        f_array[i] -> v_array[1] -> vapor_frac = f_array[i] -> v_array[1] -> liquid_frac * distribution_ratio;
        f_array[i]->v_array[1]->vaporization_antoine_min_temp = in_bounds_temperature_data[i*5+3];
        f_array[i]->v_array[1]->vaporization_antoine_max_temp = in_bounds_temperature_data[i*5+4];
    }
    return;
}

void in_bounds_antoine_data(long double temperature)
{
    int i, j;
    int max_model = 0;
    int min_model = 0;
    long double lowest_temperature = -999;
    long double highest_temperature = 1000000;
    memset(in_bounds_temperature_data, 0, sizeof(in_bounds_temperature_data));
    for(i = 0; f_array[i] != NULL; i++)
    {
        for(j = 0; f_array[i]->a_array[j] != NULL; j++)
        {
            /* Temperature is between min and max (all good break)*/
            if(f_array[i] -> a_array[j]->t_min < temperature && f_array[i] -> a_array[j]->t_max > temperature)
            {
                in_bounds_temperature_data[i*5] = f_array[i] -> a_array[j]-> antoine_a;
                in_bounds_temperature_data[i*5+1] = f_array[i] -> a_array[j]->antoine_b;
                in_bounds_temperature_data[i*5+2] = f_array[i] -> a_array[j]->antoine_c;
                in_bounds_temperature_data[i*5+3] = f_array[i] -> a_array[j]->t_min;
                in_bounds_temperature_data[i*5+4] = f_array[i] -> a_array[j]->t_max;
                break;
            }
            /*Temperature is closest to min (but still under it)*/
            if(f_array[i] -> a_array[j]->t_min > temperature && f_array[i] -> a_array[j]-> t_min > lowest_temperature && max_model == 0)
            {
                in_bounds_temperature_data[i*5] = f_array[i] -> a_array[j]->antoine_a;
                in_bounds_temperature_data[i*5+1] = f_array[i] -> a_array[j]->antoine_b;
                in_bounds_temperature_data[i*5+2] = f_array[i] -> a_array[j]->antoine_c;
                in_bounds_temperature_data[i*5+3] = f_array[i] -> a_array[j]->t_min;
                in_bounds_temperature_data[i*5+4] = f_array[i] -> a_array[j]->t_max;
                f_array[i] -> a_array[j]-> t_min = lowest_temperature;
                min_model = 1;
            }
            /*3) Temperature is closest to max (but still over)*/
            if(f_array[i] -> a_array[j]->t_max < temperature && f_array[i] -> a_array[j]-> t_max < highest_temperature && min_model == 0)
            {
                in_bounds_temperature_data[i*5] = f_array[i] -> a_array[j]->antoine_a;
                in_bounds_temperature_data[i*5+1] = f_array[i] -> a_array[j]->antoine_b;
                in_bounds_temperature_data[i*5+2] = f_array[i] -> a_array[j]->antoine_c;
                in_bounds_temperature_data[i*5+3] = f_array[i] -> a_array[j]->t_min;
                in_bounds_temperature_data[i*5+4] = f_array[i] -> a_array[j]->t_max;
                max_model = 1;
            }
        }
        min_model = 0;
        max_model = 0;
    }
    return;
}


















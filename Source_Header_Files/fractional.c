/* This file does a multicomponent fractional
 distillation calculation using the FUGK method */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "fractional.h"

extern void azeotrope_search(void);
extern void convert_pressure(void);
long double x_distillate_light, x_distillate_heavy, x_bottom_light, x_bottom_heavy;

void fractional_user(void)
{
    int i = 0;
    long double q_line, reflux_ratio, r_min;
    long double trace_k[300];
    printf("What Is The Light Key? ");
    scanf("%s", light_key);
    while(user_batch_chemical(light_key) == -1)
    {
        printf("Chemical Was Not Originally Input Into The Model. Please Enter A Chemical Already Input Into The Model. ");
        scanf("%s", light_key);
        printf("\n");
        if(i > 5)
        {
            printf("A Valid Chemical Was Not Input\n EXIT PROGRAM\n");
            exit(9);
        }
    }
    i = 0;
    printf("What Fraction Of %s Is Recovered In The Distillate? ", light_key);
    scanf("%Lf", &x_distillate_light);
    x_distillate_light = pos_frac_check(x_distillate_light);
    x_bottom_light = 1 - x_distillate_light;
    printf("What Is The Heavy Key? ");
    scanf("%s", heavy_key);
    while(user_batch_chemical(heavy_key) == -1)
    {
        printf("Chemical Was Not Originally Input Into The Model. Please Enter A Chemical Already Input Into The Model. ");
        scanf("%s", heavy_key);
        printf("\n");
        if(i > 5)
        {
            printf("A Valid Chemical Was Not Input\n EXIT PROGRAM\n");
            exit(9);
        }
    }
    i = 0;
    printf("What Fraction Of %s Remains In The Bottoms? ", heavy_key);
    scanf("%Lf", &x_bottom_heavy);
    x_bottom_heavy = pos_frac_check(x_bottom_heavy);
    x_distillate_heavy = 1 - x_bottom_heavy;
    if(models_done > 0)
    {
        printf("Would You Like To Use The Same Total System Pressure Used In The Flash Calculation? ");
        if(user_yes_no() == 0)
        {
            printf("What Is The Pressure Of Your System? ");
            scanf("%Lf", &total_pressure);
            printf("Is This Pressure In mmHg? ");
            if(user_yes_no() == 0)
            {
                convert_pressure();
            }
            boiling_point_pure_substance();
            
        }
    }
    else
    {
        printf("What Is The Pressure Of Your System? ");
        scanf("%Lf", &total_pressure);
        printf("Is This Pressure In mmHg? ");
        if(user_yes_no() == 0)
        {
            convert_pressure();
        }
        boiling_point_pure_substance();
    }
    printf("What Is The Value Of q? ");
    scanf("%Lf", &q_line);
    fractional_distillation_array_creation();
    alpha_gmean(trace_k);
    fenske();
    redistribution(trace_k);
    r_min = underwood(q_line);
    printf("\nYou Can Input A Reflux Ratio OR Use 1.4X The Minimium Reflux Ratio\n");
    printf("Would You Like To Input A Reflux Ratio? ");
    if(user_yes_no() == 1)
    {
        printf("What Is Your Reflux Ratio? ");
        scanf("%Lf", &reflux_ratio);
        while(reflux_ratio < r_min)
        {
            printf("The Reflux Ratio Must Be Equal To Or Greater Than The Minium Reflux Ratio : %Lf\n", r_min);
            printf("Please Enter A Valid Reflux Ratio: ");
            scanf("%Lf", &reflux_ratio);
            printf("\n");
            if(i > 20)
            {
                printf("A Valid Reflux Ratio Was Not Input\n EXIT PROGRAM\n");
                exit(9);
            }
        }
    }
    else
    {
        reflux_ratio = r_min * 1.4;
    }
    gilliland(reflux_ratio, r_min);
    kirkbride();
    return;
}

void alpha_gmean(long double *trace_k)
{
    int k;
    long double a, b, c;
    long double light_key_k = -999;
    long double heavy_key_k = -999;
    long double alpha_top;
    long double alpha_bottom;
    long double alpha;
    
    boiling_equilibrium_temperature_calculation();
    dew_equilibrium_temperature_calculation();
    in_bounds_antoine_data(dew_temperature);
    light_location = user_batch_chemical(light_key);
    heavy_location = user_batch_chemical(heavy_key);
    for(k = 0; k < species; k++)
    {
        a = in_bounds_temperature_data[k*5];
        b = in_bounds_temperature_data[k*5+1];
        c = in_bounds_temperature_data[k*5+2];
        trace_k[k] = pow(10, a-b/(dew_temperature+c))/total_pressure;
        if(k == light_location)
        {
            light_key_k = trace_k[k];
        }
        if(k == heavy_location)
        {
            heavy_key_k = trace_k[k];
        }
    }
    alpha_top = light_key_k/heavy_key_k;
    in_bounds_antoine_data(boiling_temperature);
    for( ;k < species*2 ;k++)
    {
        a = in_bounds_temperature_data[(k-species)*5];
        b = in_bounds_temperature_data[(k-species)*5+1];
        c = in_bounds_temperature_data[(k-species)*5+2];
        trace_k[k] = pow(10, a-b/(boiling_temperature+c))/total_pressure;
        if((k - species) == light_location)
        {
            light_key_k = trace_k[k];
        }
        if((k - species) == heavy_location)
        {
            heavy_key_k = trace_k[k];
        }
    }
    alpha_bottom = light_key_k/heavy_key_k;
    alpha = sqrt(alpha_top*alpha_bottom);
    for(k = 0; k < species; k++)
    {
        strcpy(f_array[k] -> fd_array[0] -> light_key, light_key);
        strcpy(f_array[k] -> fd_array[0] -> heavy_key, heavy_key);
            f_array[k] -> fd_array[0] -> total_pressure = total_pressure;
        f_array[k] -> fd_array[0] -> total_distillate_mass = mass_distillate;
        f_array[k] -> fd_array[0] -> total_bottom_mass = mass_bottom;
        f_array[k] -> fd_array[0] -> boiling_temperature = boiling_temperature;
        f_array[k] -> fd_array[0] -> dew_temperature = dew_temperature;
        f_array[k] -> fd_array[0] -> alpha = alpha;
        strcpy(f_array[k] -> fd_array[0] -> chemical, f_array[k] -> chemical);
        f_array[k] -> fd_array[0] -> distillate_mass = f_array[k] -> fd_array[0] -> total_distillate_mass*f_array[k] -> fd_array[0]-> distillate_frac;
        f_array[k] -> fd_array[0] -> bottoms_mass = f_array[k] -> fd_array[0] -> total_bottom_mass*f_array[k] -> fd_array[0] -> bottoms_frac;
    }
    return;
}

void fenske(void)
{
    int i;
    long double n_min;
    n_min = log((x_distillate_light/x_bottom_light)*(x_bottom_heavy/x_distillate_heavy))/log(f_array[0] -> fd_array[0] -> alpha);
    for(i = 0; i < species; i++)
    {
        f_array[i] -> fd_array[0] -> n_min = n_min;
    }
    return;
}

void redistribution(long double *trace_k)
{
    int i;
    long double alpha_trace;
    long double bottom_heavy_key_mass = f_array[heavy_location] -> fd_array[0] -> bottoms_mass;
    long double distillate_heavy_key_mass = f_array[heavy_location] -> fd_array[0] -> distillate_mass;
    long double bottoms_trace;
    long double distillate_trace;
    long double bottoms_mass = 0;
    long double distillate_mass = 0;
    
    for(i = 0; i < species; i++)
    {
        if(i != heavy_location && i != light_location)
        {
            alpha_trace = (trace_k[i+species]/trace_k[heavy_location+species] + trace_k[i]/trace_k[heavy_location])/2;
            bottoms_trace = f_array[i] -> feed_mass/(1+(distillate_heavy_key_mass/bottom_heavy_key_mass)*pow(alpha_trace,f_array[i] -> fd_array[0] -> n_min));
            distillate_trace = (f_array[i] -> feed_mass*(distillate_heavy_key_mass/bottom_heavy_key_mass)*pow(alpha_trace,f_array[i] -> fd_array[0] -> n_min))/(1+(distillate_heavy_key_mass/bottom_heavy_key_mass)*pow(alpha_trace,f_array[i] -> fd_array[0] -> n_min));
            if(bottoms_trace < distillate_trace)
            {
                distillate_trace = f_array[i]->feed_mass - bottoms_trace;
                f_array[i] -> fd_array[0] -> distillate_mass = distillate_trace;
                f_array[i] -> fd_array[0] -> bottoms_mass = bottoms_trace;
            }
            if(bottoms_trace > distillate_trace)
            {
                bottoms_trace = f_array[i]-> feed_mass - distillate_trace;
                f_array[i] -> fd_array[0] -> distillate_mass = distillate_trace;
                f_array[i] -> fd_array[0] -> bottoms_mass = bottoms_trace;
            }
            
        }
    }
     for(i = 0; i < species; i++)
     {
         distillate_mass += f_array[i] -> fd_array[0] -> distillate_mass;
         bottoms_mass += f_array[i] -> fd_array[0] -> bottoms_mass;
     }
    for(i=0; i < species; i++)
    {
        f_array[i] -> fd_array[0] -> distillate_frac = f_array[i] -> fd_array[0] -> distillate_mass/ distillate_mass;
        f_array[i] -> fd_array[0] -> total_distillate_mass = distillate_mass;
        
        f_array[i] -> fd_array[0] -> bottoms_frac = f_array[i] -> fd_array[0] -> bottoms_mass/ bottoms_mass;
        f_array[i] -> fd_array[0] -> total_bottom_mass = bottoms_mass;
    }
    return;
}

long double underwood(long double q_line)
{
    int i;
    int j = 0;
    int one_below_location = 999;
    long double theta_array[2] = {0};
    long double guess = 0;
    long double r_min;
    long double a, b, c, k_heavy_key, k_light_key, k_i_key, i_hk_alpha;
    long double one_below_hk = 999;
    long double theta;
    long double average_temperature = (dew_temperature+boiling_temperature)/2;
    in_bounds_antoine_data(average_temperature);
    a = in_bounds_temperature_data[heavy_location*5];
    b = in_bounds_temperature_data[heavy_location*5+1];
    c = in_bounds_temperature_data[heavy_location*5+2];
    k_heavy_key = pow(10, a-b/(average_temperature+c))/total_pressure;
    a = in_bounds_temperature_data[light_location*5];
    b = in_bounds_temperature_data[light_location*5+1];
    c = in_bounds_temperature_data[light_location*5+2];
    k_light_key = pow(10, a-b/(average_temperature+c))/total_pressure;
    for(i = 0; i < species; i++)
    {
        if(i != heavy_location)
        {
            if (f_array[heavy_location] -> pure_boiling_temperature - f_array[i] -> pure_boiling_temperature < 0)
            {
                if(fabsl(f_array[heavy_location] -> pure_boiling_temperature - f_array[i]->pure_boiling_temperature) < one_below_hk)
                {
                    one_below_hk =  f_array[i] -> pure_boiling_temperature;
                    one_below_location = i;
                }
            }
        }
    }
    a = in_bounds_temperature_data[one_below_location*5];
    b = in_bounds_temperature_data[one_below_location*5+1];
    c = in_bounds_temperature_data[one_below_location*5+2];
    one_below_hk = pow(10, a-b/(average_temperature+c))/total_pressure;
    for(theta = one_below_hk/k_heavy_key; theta < k_light_key/k_heavy_key; theta = theta + 0.01)
    {
        guess = 0;
        for(i = 0; i <species; i++)
        {
            a = in_bounds_temperature_data[i*5];
            b = in_bounds_temperature_data[i*5+1];
            c = in_bounds_temperature_data[i*5+2];
            k_i_key = pow(10, a-b/(average_temperature+c))/total_pressure;
            i_hk_alpha =  k_i_key/k_heavy_key;
            guess += (i_hk_alpha*f_array[i]->feed_frac)/(i_hk_alpha-theta);
        }
        if(fabsl(guess + q_line - 1) < 0.05)
        {
            theta_array[j] = theta;
            j++;
        }
    }
    for(j=0; j < 2; j++)
    {
        guess = 0;
        for(i = 0; i < species; i++)
        {
            a = in_bounds_temperature_data[i*5];
            b = in_bounds_temperature_data[i*5+1];
            c = in_bounds_temperature_data[i*5+2];
            k_i_key = pow(10, a-b/(average_temperature+c))/total_pressure;
            i_hk_alpha =  k_i_key/k_heavy_key;
            guess += (i_hk_alpha*f_array[i] -> fd_array[0]-> distillate_frac)/(i_hk_alpha-theta_array[j]);
        }
        r_min = guess - 1;
        if(guess - 1 > r_min)
        {
            r_min = guess - 1;
        }
    }
    for(i=0;i<species;i++)
    {
        f_array[i] -> fd_array[0] -> r_min = r_min;
    }
    return r_min;
}

void gilliland(long double reflux_ratio, long double r_min)
{
    int i;
    long double right_hand_side, number_of_stages;
    right_hand_side= 0.75*(1-pow((reflux_ratio-r_min)/(reflux_ratio-1),0.567));
    number_of_stages = (f_array[0]->fd_array[0] -> n_min + right_hand_side)/(1-right_hand_side);
    for(i = 0; i< species; i++)
    {
        f_array[i] -> fd_array[0] -> reflux_ratio = reflux_ratio;
        f_array[i] -> fd_array[0] -> number_of_stages = number_of_stages;
    }
    return;
}

void kirkbride(void)
{
    int i;
    long double reflux_stripping_ratio, number_of_reflux_stages, number_of_stripping_stages;
    reflux_stripping_ratio = pow((f_array[heavy_location] -> feed_frac/f_array[light_location] -> feed_frac)*pow((f_array[light_location] -> fd_array[0] -> bottoms_frac/f_array[heavy_location] -> fd_array[0] -> distillate_frac),2)*(f_array[0] -> fd_array[0] -> total_bottom_mass/f_array[0]->fd_array[0] -> total_distillate_mass), 0.206);
    number_of_stripping_stages = f_array[0]->fd_array[0]->number_of_stages/(reflux_stripping_ratio+1);
    number_of_reflux_stages = f_array[0]->fd_array[0]->number_of_stages - number_of_stripping_stages;
    for(i = 0; i < species; i++)
    {
        f_array[i] -> fd_array[0] -> feed_locations = number_of_reflux_stages;
    }
    return;
}

/* This file prints the calculated results contained
 in the data structures to the screen */

#include <stdio.h>
#include <string.h>
#include "main.h"

extern int longest_chemical;
void print_azeotrope_warning(void);
void print_azeotrope(void);
void print_flash(void);
void print_batch(void);
void print_page_clean(void);
void print_fractional(void);
void print_page_clean(void)
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    return;
}

void print_azeotrope_warning(void)
{
    printf("WARNING AZEOTROPE MIXTURE DETECTED IN FEED\nThis Program Uses The Antoine Equation and Raoult's Law To Determine The Vapor Pressure Of The Feed And The Vapor Pressure Of The Feeds' Components. The Antoine Equation Models The Saturated Vapor Pressure Of Pure Components As A Function Of Temperature. Raoult's Law Then Uses The Saturated Vapor Pressures Of The Pure Feed Components (Calculated By The Antoine Equation) And The Fraction Of The Components In The Feed, To Calculate The Partial Pressure Of Each Component In The Mixture. These Partial Pressures Are Then Divided By The Total System Pressure Specified By The User To Calculate The Distribution Coefficient Of Each Component. These Distribution Coefficients Are Then Used To Model How The Feed Mixture Will Seperate As Energy Is Added To The System. Raoult's Law Assumes The Mixture Is Ideal. Azeotropes Are Not Ideal Mixtures. As Such The Resulting Calculations For The Input System Could Vary Significantly From Real World Results.\n\n");
    print_azeotrope();
    return;
}

void print_azeotrope(void)
{
    int i, j;
    printf("AZEOTROPE WARNING. AZEOTROPE DATA FOR COMPONENTS IN FEED MIXTURE\n");
    for(i = 0; f_array[i] != NULL; i++)
    {
        if(strcmp(f_array[i] -> chemical, f_array[i] -> az_ptr->chemical_a) == 0)
        {
            printf("Azeotrope Boiling Temperature: %.2f Celsius\n", f_array[i] -> az_ptr-> azeotrope_boiling_temperature);
            printf("Chemical");
            for(j=0;j<longest_chemical-strlen("Chemical");j++)
            {
                printf(" ");
            }
            printf(": Azeotrope Composition\n");
            printf("%s", f_array[i] -> az_ptr -> chemical_a);
            for(j=0;j<longest_chemical-strlen(f_array[i] -> az_ptr -> chemical_a);j++)
            {
                printf(" ");
            }
            printf(": %.2f\n", f_array[i] -> az_ptr->fraction_a);
            printf("%s", f_array[i] -> az_ptr -> chemical_b);
            for(j=0;j < longest_chemical-strlen(f_array[i] -> az_ptr -> chemical_b);j++)
            {
                printf(" ");
            }
            printf(": %.2f\n\n", f_array[i] -> az_ptr->fraction_b);
        }
    }
    return;
}

void print_flash(void)
{
    int i, j;
    printf("\n\nFLASH DISTILLATION RESULTS USING EQUILIBRIUM AND RACHFORD-RICE\n");
    printf("      Equilibrium Results\n");
    printf("                   Temperature Results\n");
    printf("Dew Temperature    : %.2Lf Celsius\n", f_array[0]->v_array[0]->dew_temperature);
    /* The Data Structure Needs To Be Modified To Allow This Feature
    printf("Antoine Data Used       : A Constant  B Constant  C Constant  Min Temperature (C)  Max Temperature (C)\n");
    printf("                          %.4Lf  %.4Lf  %.4Lf  %3.2Lf  %3.2Lf\n", f_array[0]->v_array[0]->dew_antoine_a,f_array[0]->v_array[0]->dew_antoine_b,f_array[0]->v_array[0]->dew_antoine_c,f_array[0]->v_array[0]->dew_antoine_min_temp,f_array[0]->v_array[0]->dew_antoine_max_temp);
    */
    printf("Boiling Temperature: %.2Lf Celsius\n", f_array[0]->v_array[0]->boiling_temperature);
    /* The Data Structure Needs To Be Modified To Allow This Feature
    printf("                     %.4Lf  %.4Lf  %.4Lf  %3.2Lf  %3.2Lf\n", f_array[0]->v_array[0]->boiling_antoine_a,f_array[0]->v_array[0]->boiling_antoine_b,f_array[0]->v_array[1]->boiling_antoine_c,f_array[0]->v_array[0]->boiling_antoine_min_temp,f_array[0]->v_array[0]->boiling_antoine_max_temp);
     */
    printf("Chemical       : Liquid                        Vapor\n");
    for(i = 0; f_array[i] != NULL; i++)
    {
        printf("%s", f_array[i]->v_array[0]->chemical);
        for(j=0;j<15-strlen(f_array[i]->v_array[0]->chemical);j++)
        {
            printf(" ");
        }
        printf(": %.2Lf", f_array[i]->v_array[0]->liquid_frac);
        for(j=0;j<26;j++)
        {
            printf(" ");
        }
        printf("%.2Lf\n", f_array[i]->v_array[0]->vapor_frac);
    }
    
    
    printf("\n     Rachford-Rice Results\n");
    printf("Feed Fraction Vaporized : %.2Lf\n", f_array[0]->v_array[1]->vaporization_frac);
    printf("                        Temperature Results\n");
    printf("Vaporization Temperature: %.2Lf Celsius\n", f_array[0]->v_array[1]->vaporization_temperature);
    /* The Data Structure Needs To Be Modified To Allow This Feature
    printf("Antoine Data Used       : A Constant  B Constant  C Constant  Min Temperature (C)  Max Temperature (C)\n");
    printf("%.4Lf  %.4Lf  %.4Lf  %3.2Lf  %3.2Lf\n", f_array[0]->v_array[1]->vaporization_antoine_a,f_array[0]->v_array[1]->vaporization_antoine_b,f_array[0]->v_array[1]->vaporization_antoine_c,f_array[0]->v_array[1]->vaporization_antoine_min_temp,f_array[0]->v_array[1]->vaporization_antoine_max_temp);
     */
    printf("Chemical       : Liquid                        Vapor\n");
    for(i = 0; f_array[i] != NULL; i++)
    {
        printf("%s", f_array[i]->v_array[1]->chemical);
        for(j=0;j<15-strlen(f_array[i]->v_array[1]->chemical);j++)
        {
            printf(" ");
        }
        printf(": %.2Lf", f_array[i]->v_array[1]->liquid_frac);
        for(j=0;j<26;j++)
        {
            printf(" ");
        }
    printf("%.2Lf\n", f_array[i]->v_array[1]->vapor_frac);
    }

    return;
}

void print_batch(void)
{
    /* This is not done yet */
    return;
}

void print_fractional(void)
{
    int i,j;
    printf("\n\nMULTICOMPONENT FRACTIONAL DISTILLATION RESULTS USING F.U.G.K\n");
    printf("Temperature At Top Of Column   : %.2Lf Celsius\n", f_array[0]->fd_array[0]->dew_temperature);
    printf("Temperature At Bottom Of Column: %.2Lf Celsius\n", f_array[0]->fd_array[0]->boiling_temperature);
    printf("Light Key      : %s\n", f_array[0]->fd_array[0]->light_key);
    printf("Heavy Key      : %s\n", f_array[0]->fd_array[0]->heavy_key);
    printf("Total Feed Rate: %.2Lf (mol/hr)\n\n", f_array[0]->total_feed_mass);
    printf("Chemical       : Distillate %.2Lf (mol/hr)     Bottoms %.2Lf (mol/hr)\n",f_array[0]->fd_array[0]->total_distillate_mass,f_array[0]->fd_array[0]->total_bottom_mass);
    for(i=0;f_array[i]!=NULL;i++)
    {
        printf("%s",f_array[i]->fd_array[0]->chemical);
        for(j=0;j<15-strlen(f_array[i]->fd_array[0]->chemical);j++)
            {
                printf(" ");
            }
        printf(": %.2Lf",f_array[i]->fd_array[0]->distillate_frac);
        for(j=0;j<26;j++)
        {
            printf(" ");
        }
        printf("%.2Lf\n",f_array[i]->fd_array[0]->bottoms_frac);
    }
    printf("\nThe Minimium Reflux Ratio At Total Reflux    : %.2Lf\n", f_array[0]->fd_array[0]->r_min);
    printf("Minimium Number Of Stages At Total Reflux    : %.2Lf\n", f_array[0]->fd_array[0]->n_min);
    printf("Number Of Stages For A Reflux Ratio Of %.2Lf  : %.2Lf\n",f_array[0]->fd_array[0]->reflux_ratio, f_array[0]->fd_array[0]->number_of_stages);
    printf("Feed Tray Location For A Reflux Ratio Of %.2Lf: %.2Lf\n\n\n",f_array[0]->fd_array[0]->reflux_ratio, f_array[0]->fd_array[0]->feed_locations);
    return;
}

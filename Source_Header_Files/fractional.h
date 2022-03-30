/* This header file contains functions and
 variables used by the fractional.c file */

#ifndef fractional_h
#define fractional_h
extern int user_yes_no(void);
extern long double pos_frac_check(long double);
extern int species;
extern int models_done;
extern long double total_pressure;
extern void fractional_distillation_array_creation(void);
extern void in_bounds_antoine_data(long double temperature);
extern int user_batch_chemical(char *chemical);
extern int chosen_chemical_location(void);
extern long double in_bounds_temperature_data[300];

void alpha_gmean(long double *trace_k);
void fenske(void);
void redistribution(long double *trace_k);
long double underwood(long double q_line);
void gilliland(long double reflux_ratio, long double r_min);
void kirkbride(void);
extern void boiling_point_pure_substance(void);
extern void initial_material_balence(void);
extern void dew_equilibrium_temperature_calculation(void);
extern void boiling_equilibrium_temperature_calculation(void);
extern long double boiling_temperature;
extern long double dew_temperature;
long double mass_bottom, mass_distillate;
int light_location;
int heavy_location;
char heavy_key[50];
char light_key[50];


#endif /* fractional_h */

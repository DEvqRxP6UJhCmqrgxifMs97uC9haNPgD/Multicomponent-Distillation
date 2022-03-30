/* This header file contains the data structures used
 across the program and their variables */
#ifndef main_h
#define main_h
typedef struct azeotrope_struct {
    char chemical_a[50];
    char chemical_b[50];
    float boiling_temperature_a;
    float boiling_temperature_b;
    float fraction_a;
    float fraction_b;
    float azeotrope_boiling_temperature;
} azeotrope_struct;

typedef struct vaporization_struct {
    char type[50];
    char chemical[50];
    long double total_pressure;
    long double vaporization_temperature;
    long double vaporization_frac;
    long double boiling_temperature;
    long double dew_temperature;
    long double liquid_frac;
    long double vapor_frac;
    long double dew_antoine_a;
    long double dew_antoine_b;
    long double dew_antoine_c;
    long double dew_antoine_min_temp;
    long double dew_antoine_max_temp;
    long double boiling_antoine_a;
    long double boiling_antoine_b;
    long double boiling_antoine_c;
    long double boiling_antoine_min_temp;
    long double boiling_antoine_max_temp;
    long double vaporization_antoine_a;
    long double vaporization_antoine_b;
    long double vaporization_antoine_c;
    long double vaporization_antoine_min_temp;
    long double vaporization_antoine_max_temp;
} vaporization_struct;

typedef struct batch_struct {
    char chemical[50];
    long double bubble_temperature;
    long double total_pressure;
    long double liquid_residual;
    long double vapor_distillate;
} batch_struct;

typedef struct fractional_distillation_struct {
    char light_key[50];
    char heavy_key[50];
    long double total_pressure;
    long double total_distillate_mass;
    long double total_bottom_mass;
    long double boiling_temperature;
    long double dew_temperature;
    long double alpha;
    long double n_min;
    long double r_min;
    long double number_of_stages;
    long double reflux_ratio;
    long double feed_locations;
    char chemical[50];
    long double distillate_mass;
    long double distillate_frac;
    long double bottoms_mass;
    long double bottoms_frac;
    long double dew_antoine_a;
    long double dew_antoine_b;
    long double dew_antoine_c;
    long double dew_antoine_min_temp;
    long double dew_antoine_max_temp;
    long double boiling_antoine_a;
    long double boiling_antoine_b;
    long double boiling_antoine_c;
    long double boiling_antoine_min_temp;
    long double boiling_antoine_max_temp;
} fractional_distillation_struct;

typedef struct antoine_struct {
    char chemical[50];
    long double antoine_a;
    long double antoine_b;
    long double antoine_c;
    long double t_min;
    long double t_max;
    struct antoine_struct *next;
} antoine_struct;

typedef struct feed_struct {
    char chemical[50];
    long double pure_boiling_temperature;
    long double total_feed_mass;
    long double feed_frac;
    long double feed_mass;
    antoine_struct **a_array;
    azeotrope_struct *az_ptr;
    vaporization_struct **v_array;
    batch_struct **b_array;
    fractional_distillation_struct **fd_array;
} feed_struct;

antoine_struct **a_array;
feed_struct **f_array;
vaporization_struct **v_array;
batch_struct **b_array;
fractional_distillation_struct **fd_array;

extern int model_choice (void);
extern void antoine_user (void);
extern antoine_struct *a_ptr;
extern void feed_user(void);
extern void flash_user(void);
/*extern void batch_user(void);*/

#endif /* main_h */

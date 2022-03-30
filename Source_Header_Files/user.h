/* This header file contains functions used by
 user.c */

#ifndef user_h
#define user_h

int user_yes_no (void);
int model_choice (void);
void not_valid_yes_no (void);
long double pos_frac_check (long double temp_answer);
long double not_valid_frac (long double temp_answer);


#endif /* user_h */

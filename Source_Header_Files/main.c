/* The main function interacts with the user
 and calls the appropriate functions */
#include <stdlib.h>
#include "stdio.h"
#include "main.h"

extern int user_yes_no (void);
extern int warning;
extern int models_done;
extern void data_user(void);
extern void print_flash(void);
extern void fractional_user(void);
extern void print_page_clean(void);
extern void print_fractional(void);
extern void azeotrope_search(void);
extern void batch_array_creation(void);
extern void print_azeotrope_warning(void);

int print_flash_marker = 0;
int print_batch_marker = 0;
int print_fractional_marker = 0;
void fractional_distillation_array_creation(void);

int main (int argc, char *argv[])
{
    static int first_run;
    if(first_run == 0)
    {
    data_user();
    azeotrope_search();
        first_run++;
    }
    switch (model_choice())
    {
        case 1 :
            flash_user();
            models_done = abs(models_done);
            print_flash_marker = 1;
            printf("\nWould You Like To Analysis This System Using Another Model? ");
            if(user_yes_no() == 0)
            {
                break;
            }
            main(argc, argv);
        case 2 :
            /* batch_user(); */
            models_done = abs(models_done);
            print_batch_marker = 1;
            printf("\nWould You Like To Analysis This System Using Another Model? ");
            if(user_yes_no() == 0)
            {
                break;
            } 
            main(argc, argv);
        case 3 :
            fractional_user();
            models_done = abs(models_done);
            print_fractional_marker = 1;
            printf("\nWould You Like To Analysis This System Using Another Model? ");
            if(user_yes_no() == 0)
            {
                break;
            }
            main(argc, argv);
        default :
            printf("\nNo More Models Are Avaliable\n");
            if(models_done == -1)
            {
                exit(0);
            }
            break;
    }
    print_page_clean();
    if(warning == 1)
    {
        print_azeotrope_warning();
    }
    if(print_flash_marker == 1)
    {
        print_flash();
    }
    if(print_batch_marker == 1)
    {
       /* print_batch();*/
    }
    if(print_fractional_marker == 1)
    {
        print_fractional();
    }
    exit(0);
}

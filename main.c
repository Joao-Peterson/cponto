#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <cmdf.h>
#include <doc.h>
#include <doc_csv.h>
#include <doc_print.h>



// operation enumerator
typedef enum{
    op_start = 1,
    op_next,
    op_stop,
    op_erase
}op_t;



// check if file / dir exists
int check_fs(char *filename_directory);

// converts time_t to decimal representation in days
double time_to_spreadsheet_date_time(time_t time);

// get time in decimal representation in days
double time_spreadsheet(void);

// options
cmdf_option options[];

// custom function to handle cli options
int parse_options(char key, char *arg, int arg_pos, void *extern_user_variables_struct);



// to indicate that the first argument passed is a option
int first_cmd = 1;

// task to be stored
char *task = "#### NULL TASK ####";

// operation to be executed
op_t op_code = 0;



// main
int main(int argc, char **argv){

    cdmf_parse_options(options, &parse_options, argc, argv, PARSER_FLAG_PRINT_ERRORS_STDOUT | 
        PARSER_FLAG_USE_PREDEFINED_OPTIONS, NULL);

    char *csv_filename = "c_ponto.csv";

    doc *csv;

    // check cponto csv file on new one
    if(!check_fs(csv_filename)){
        csv = doc_new(
            "csv", dt_obj,
            ";"
        );

        doc_csv_save(csv, csv_filename);
    }
    // create new one
    else if(op_code == op_erase){

        printf("\aYou are about to erase the \"%s\" file. Do you wish to continue? (y or n) (default=no)\n", csv_filename);

        int ans = getchar(); 

        switch(ans){
            case 'y':
                {
                    csv = doc_new(
                        "csv", dt_obj,
                        ";"
                    );

                    doc_csv_save(csv, csv_filename);

                    exit(0);
                }
            break;
                
            case 'n':
            default:
                exit(0);
            break;
        }
    }
    // open already existing one
    else{
        csv = doc_csv_open(csv_filename); 

        if(csv == NULL){
            printf("\aFile: \"%s\" was corrupted", csv_filename);

            csv = doc_new(
                "csv", dt_obj,
                ";"
            );

            doc_csv_save(csv, csv_filename);
        }
    }

    // get cur time for spreadsheet
    double time = time_spreadsheet();

    // save based on operation
    switch(op_code){
        
        case op_start:
            {
                int last = doc_get_size(csv, ".");

                if(last != 0){
                    
                    last -= 1;

                    int i = 0;
                    char buffer[50];

                    snprintf(buffer, 50, "[%i][0]", last);
                    double start = doc_get(csv, buffer, double);
                    snprintf(buffer, 50, "[%i][1]", last);
                    double end = doc_get(csv, buffer, double);

                    if(start == end){
                        printf("\aYour last task has not yet been concluded.\n");
                        printf("Do you wish to \'c\'ontinue normally, ");
                        printf("stop the previous and create a \'n\'ext one or ");
                        printf("\'e\'xit? (c or n or e) (default=exit)\n");

                        int ans = getchar(); 

                        switch(ans){
                            case 'n':
                                {
                                    doc_set(csv, buffer, double, time);
                                }
                            break;

                            case 'c':
                            break;
                                
                            case 'e':
                            default:
                                exit(0);
                            break;
                        }
                    }

                }

                doc_add(csv, ".", 
                    "linha", dt_obj,
                        "hora", dt_double, time,
                        "hora2", dt_double, time,
                        "atividae", dt_const_string, task, (size_t)(strlen(task) + 1),
                    ";"
                );

                doc_csv_save(csv, csv_filename);
            }
            break;

        case op_next:
            {
                int last = doc_get_size(csv, ".");

                if(last == 0){
                    printf("\aYou are trying to register a next task, but this is going to be the first task.\n");
                    printf("Do you wish to create a new task? (y or n) (default=y).\n");

                    int ans = getchar(); 

                    switch(ans){
                        default:
                        case 'y':
                            break;
                            
                        case 'n':
                            exit(0);
                        break;
                    }
                }
                else{
                    last -= 1;

                    int i = 0;
                    for(doc_loop(linha, csv)){
                        if(i == last){
                            doc_set(linha, "[1]", double, time);        
                        }

                        i++;
                    }
                }

                doc_add(csv, ".", 
                    "linha", dt_obj,
                        "hora", dt_double, time,
                        "hora2", dt_double, time,
                        "atividae", dt_const_string, task, (size_t)(strlen(task) + 1),
                    ";"
                );

                doc_csv_save(csv, csv_filename);
            }
            break;

        case op_stop:
            {
                int i = 0;
                int last = doc_get_size(csv, ".");

                if(last == 0){
                    printf("\aYou are trying to stop a previous task, but there are no tasks on register.\n");
                    exit(0);
                }

                last -= 1;

                for(doc_loop(linha, csv)){
                    if(i == last){
                        doc_set(linha, "[1]", double, time);        
                    }

                    i++;
                }

                doc_csv_save(csv, csv_filename);
            }
            break;
    }

    printf("\a");
    doc_delete(csv, ".");

    return 0;
}



// check if file / dir exists
int check_fs(char *filename_directory){
    FILE *file = fopen(filename_directory, "r+");

    if(file == NULL){
        switch(errno){
            case ENOENT:
            case ENOTDIR:
            case EACCES:
            case EINVAL:
            default:
                return 0;
                break;

            case EEXIST:
            case EISDIR:
                return 1;
                break;
        }
    }
    else{
        fclose(file);
        return 1;
    }
}

// converts time_t to decimal representation in days
double time_to_spreadsheet_date_time(time_t time){
    // break down to get UTC offset
    struct tm* time_s = localtime(&time);

    // sum the offset to the seconds
    time += time_s->tm_gmtoff;

    // divide by seconds in a day to get a decimal date/time representation.
    // spresheets use a time base as of 30/12/1899, UNIX time count as of 1/1/1970,
    // 25569 is the diference is seconts to account for this offset.
    return (double)(((double)time / (24*60*60)) + 25569); 
}

// get time in decimal representation in days
double time_spreadsheet(void){
    return time_to_spreadsheet_date_time(time(NULL));
}

// options
cmdf_option options[] = {
    {"start", 's', OPTION_OPTIONAL, 1, "Start new task entry."},
    {"next" , 'n', OPTION_OPTIONAL, 1, "Stops previous task and start another one."},
    {"stop" , 'q', OPTION_OPTIONAL, 0, "Stops the previous task ending the period."},
    {"end" ,  'E', OPTION_ALIAS, 0, NULL},
    {"new" ,  'N', OPTION_OPTIONAL, 0, "Erases current file and create a new one."},
    {"erase", 'e', OPTION_ALIAS,    0, NULL},
    {0}
};

// custom function to handle cli options
int parse_options(char key, char *arg, int arg_pos, void *extern_user_variables_struct){

    switch(key){
        case 's':
                task = arg;  
                op_code = op_start;
            break;

        case 'n':
                task = arg;  
                op_code = op_next;
            break;

        case 'q':
        case 'E':
                task = arg;  
                op_code = op_stop;
            break;

        case 'e':
        case 'N':
                op_code = op_erase;
            break;
            
        case 0:
            if(first_cmd){
                if (!strcmp(arg, "start"))
                {
                    op_code = op_start;
                }
                else if (!strcmp(arg, "next"))
                {
                    op_code = op_next;
                }
                else if (!strcmp(arg, "end") || !strcmp(arg, "stop"))
                {
                    op_code = op_stop;
                }
                else if (!strcmp(arg, "new") || !strcmp(arg, "erase"))
                {
                    op_code = op_erase;
                }
                else
                {
                    printf("An option has to be passed!\n Usage: ./cponto [start, next, stop, end, new, erase] \"TASK\"\n");
                    exit(0);
                }

                first_cmd = 0;
            }
            else{
                task = arg;
            }

            break;

        default:
            printf("Corrupted command line arguments!");
            exit(-1);
            break;
    }

    return 0;
}

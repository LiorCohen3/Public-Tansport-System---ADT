#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************************************************************/
/*******************************  ---  Helpers  ---  ******************************************************************/
/**********************************************************************************************************************/

#define TABLE(DO)                    \
    DO(SCHEDULE_SUCCESS)             \
    DO(SCHEDULE_NULL_ARG)            \
    DO(SCHEDULE_OUT_OF_MEMORY)       \
    DO(SCHEDULE_ALREADY_EXISTS)      \
    DO(SCHEDULE_INVALID_LINE_TYPE)   \
    DO(SCHEDULE_INVALID_LINE_NUMBER) \
    DO(SCHEDULE_INVALID_PRICE)       \
    DO(SCHEDULE_LINE_DOESNT_EXIST)   \
    DO(SCHEDULE_STATION_DOESNT_EXIST)

#define CODE_AS_STR(code) \
#code,

#define CODE_AS_STRUCT(code) \
    code,

const char *code_strings[] = {
    TABLE(CODE_AS_STR)};

typedef enum
{
    TABLE(CODE_AS_STRUCT)
        LAST_ERROR
} schedule_error_code;

void print_error_code_func(schedule_error_code code, const char *func)
{
    FILE *out = stderr;
    if (code < SCHEDULE_SUCCESS || code >= LAST_ERROR)
    {
        fprintf(out, "Invalid error code %d in function %s\n", code, func);
        exit(1);
    }
    fprintf(out, "[%s]: %s\n", func, code_strings[code]);
}

/**********************************************************************************************************************/
/*******************************  ---  Definitions for your usage  ---  *************************************************/
/**********************************************************************************************************************/
typedef enum line_type
{
    BUS,
    TRAIN,
    METRO
} line_type;

typedef struct station
{
    unsigned int ID;
    char *name;
    struct station *next_station;
} station;

typedef struct line
{
    unsigned int line_number;
    enum line_type type;
    float cost;
    struct line *next_line;
    station *first_station;
    unsigned int num_of_stations;
} line;

typedef struct schedule_line_node_s
{
    line *first_line;
} schedule_line_node_t;

/**********************************************************************************************************************/

/* Use the following macro to print the error_codes (SCHEDULE_SUCCESS as well) */

/* MACRO: schedule_print_error_code
 *      Prints the error code to stderr
 * Example:
 *      // Your code here
 *      // ...
 *      print_error_code(SCHEDULE_OUT_OF_MEMORY);
 *      // ...
 *      // Continue of your code
 *
 */
#define print_error_code(code) \
    print_error_code_func(code, __func__)
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/*************************************  ---  Functions you need to implement --- **************************************/
/**********************************************************************************************************************/

/* function: schedule_create
 * parameters:
 *      None
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_OUT_OF_MEMORY
 * return value:
 *      - Pointer to a new allocated linked list
 *      - NULL incase of failure
 */
schedule_line_node_t *schedule_create()
{
    schedule_line_node_t *new_schedule;
    new_schedule = (schedule_line_node_t *)calloc(1, sizeof(schedule_line_node_t));
    if (new_schedule == NULL)
    {
        print_error_code(SCHEDULE_OUT_OF_MEMORY);
        return NULL;
    }
    new_schedule->first_line = NULL;
    print_error_code(SCHEDULE_SUCCESS);
    return new_schedule;
}
/**********************************************************************************************************************/
void free_station(station *to_free)
{
    char *to_free_station_name = to_free->name;
    free(to_free_station_name);
    free(to_free);
    return;
}
void free_line(line *to_free)
{
    station *to_free_station = to_free->first_station;
    while (to_free_station != NULL)
    {
        to_free->first_station = to_free->first_station->next_station;
        free_station(to_free_station);
        to_free_station = to_free->first_station;
    }
    free(to_free);
    return;
}
/*
 * function: schedule_destroy
 * parameters:
 *      schedule - Pointer to the linked list to be destroyed
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_NULL_ARG
 * return value:
 *      None
 */

void schedule_destroy(schedule_line_node_t *head)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return;
    }
    line *to_free_line = head->first_line;
    while (to_free_line != NULL)
    {
        free_line(to_free_line);
        head->first_line = head->first_line->next_line;
        to_free_line = head->first_line;
    }
    free(head);
    print_error_code(SCHEDULE_SUCCESS);
    return;
}
/**********************************************************************************************************************/

/*
 * function: schedule_add_line
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_type - Type of the line to be added
 *      line_number - Number of the line to be added
 *      price - Price of the line
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_OUT_OF_MEMORY
 *      SCHEDULE_NULL_ARG
 *      SCHEDULE_INVALID_LINE_TYPE
 *      SCHEDULE_INVALID_LINE_NUMBER
 *      SCHEDULE_ALREADY_EXISTS
 *      SCHEDULE_INVALID_PRICE
 * return value:
 *      - Pointer to the new head of the list
 *     - NULL incase of failure
 */
schedule_line_node_t *schedule_add_line(schedule_line_node_t *head, line_type type, unsigned int line_number, float price)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return NULL;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        if(temp->line_number == line_number)
        {
            print_error_code(SCHEDULE_ALREADY_EXISTS);
            return NULL;
        }
       temp= temp->next_line;
    }
    if (type < 0 || type > 2)
    {
        print_error_code(SCHEDULE_INVALID_LINE_TYPE);
        return NULL;
    }
    if (line_number < 0 || line_number > 100)
    {
        print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
        return NULL;
    }
    if (price <= 0)
    {
        print_error_code(SCHEDULE_INVALID_PRICE);
        return NULL;
    }
    line *new_line;
    new_line = (line*) calloc(1,sizeof(line));
    if (new_line == NULL)
    {
        print_error_code(SCHEDULE_OUT_OF_MEMORY);
        return NULL;
    }
    new_line->cost = price;
    new_line->type = type;
    new_line->line_number = line_number;
    new_line->first_station = NULL;
    new_line->next_line = head->first_line;
    head->first_line = new_line;
    print_error_code(SCHEDULE_SUCCESS);
    return head;
}
/**********************************************************************************************************************/

/*
 * function: schedule_remove_line
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to be removed
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_NULL_ARG
 *      SCHEDULE_LNVALID_LINE_NUMBER
 *      SCHEDULE_LINE_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *     - NULL incase of failure
 */
schedule_line_node_t *schedule_remove_line(schedule_line_node_t *head, unsigned int line_number)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return NULL;
    }
    if (line_number < 0 || line_number > 100)
    {
        print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
        return NULL;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        if(temp->line_number == line_number)
        {
            line *to_free_line = temp;
            head->first_line = head->first_line->next_line;
            free_line(to_free_line);
            print_error_code(SCHEDULE_SUCCESS);
            return head;
        }
        if(temp->next_line != NULL && temp->next_line->line_number == line_number)
        {
            line *to_free_line = temp->next_line;
            temp->next_line = temp->next_line->next_line;
            free_line(to_free_line);
            print_error_code(SCHEDULE_SUCCESS);
            return head;
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
}
/**********************************************************************************************************************/

/*
 * function: schedule_add_station
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to add station to
 *      station_name - Name of the station to be added
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_NULL_ARG
 *      SCHEDULE_OUT_OF_MEMORY
 *      SCHEDULE_INVALID_LINE_NUMBER
 *      SCHEDULE_LINE_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *      - NULL incase of failure
 */
schedule_line_node_t *schedule_add_station(schedule_line_node_t *head, unsigned int line_number, const char *station_name)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return NULL;
    }
    if (line_number < 0 || line_number > 100)
    {
        print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
        return NULL;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        if(temp->line_number == line_number)
        {
            station *new_station = (station*)calloc(1,sizeof(station));
            if (new_station == NULL)
            {
                print_error_code(SCHEDULE_OUT_OF_MEMORY);
                return NULL;
            }
            new_station->name = (char*)calloc(sizeof(char),strlen(station_name)+1);
            if (new_station->name == NULL)
            {
                free(new_station);
                print_error_code(SCHEDULE_OUT_OF_MEMORY);
                return NULL;
            }
            new_station->next_station = NULL;
            new_station->ID = temp->num_of_stations;
            strcpy(new_station->name,station_name);
            station *curr_station = temp->first_station;
            if (curr_station == NULL)
            {
                temp->first_station = new_station;
            }
            else
            {
                while (curr_station->next_station != NULL)
                {
                    curr_station = curr_station->next_station;
                }
                curr_station->next_station = new_station;
            }
            temp->num_of_stations++;
            print_error_code(SCHEDULE_SUCCESS);
            return head;
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
}
/**********************************************************************************************************************/

/*
 * function: schedule_remove_station
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to remove station from
 *      station_index - The pointion of the station in the bus route (starting at 0)
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_NULL_ARG
 *      SCHEDULE_INVALID_LINE_NUMBER
 *      SCHEDULE_LINE_DOESNT_EXIST
 *      SCHEDULE_STATION_DOESNT_EXIST
 * return value:
 *      - Pointer to the new head of the list
 *      - NULL incase of failure
 */
schedule_line_node_t *schedule_remove_station(schedule_line_node_t *head, unsigned int line_number, unsigned int station_index)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return NULL;
    }
    if (line_number < 0 || line_number > 100)
    {
        print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
        return NULL;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        if(temp->line_number == line_number)
        {
            station *temp_station = temp->first_station;
            while (temp_station != NULL)
            {
                if (temp_station->ID == station_index)
                {
                    station *to_free_station = temp_station;
                    temp->first_station = temp_station->next_station;
                    free_station(to_free_station);
                    print_error_code(SCHEDULE_SUCCESS);
                    return head;
                }
                if(temp_station->next_station != NULL && temp_station->next_station->ID == station_index)
                {
                    station *to_free_station = temp_station->next_station;
                    temp_station->next_station = temp_station->next_station->next_station;
                    free_station(to_free_station);
                    print_error_code(SCHEDULE_SUCCESS);
                    return head;
                }
                temp_station = temp_station->next_station;
            }
            print_error_code(SCHEDULE_STATION_DOESNT_EXIST);
            return NULL;
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return NULL;
}
/**********************************************************************************************************************/
void print_line(line *printline)
{
    switch (printline->type)
    {
    case 0:
        printf("===Line %u. Type: Bus. Price: %.2f$===\n",printline->line_number,printline->cost);
        break;
    case 1:
        printf("===Line %u. Type: Train. Price: %.2f$===\n",printline->line_number,printline->cost);
        break;
    case 2:
        printf("===Line %u. Type: Metro. Price: %.2f$===\n",printline->line_number,printline->cost);
        break;
    }
}
/*
 * function: schedule_get_station_index
 * parameters:
 *      schedule - Pointer to the linked list
 *      line_number - Number of the line to print it's path
 * errors:
 *      SCHEDULE_SUCCESS
 *      SCHEDULE_NULL_ARG
 *      SCHEDULE_INVALID_LINE_NUMBER
 * return value:
 *      - None
 */
void schedule_print_line_route(schedule_line_node_t *head, unsigned int line_number)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return;
    }
    if (line_number < 0 || line_number > 100)
    {
        print_error_code(SCHEDULE_INVALID_LINE_NUMBER);
        return;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        if(temp->line_number == line_number)
        {
            print_line(temp);
            station *temp_station = temp->first_station;
            int counter = 1;
            while (temp_station != NULL)
            {
                printf("Station %d: %s\n",counter,temp_station->name);
                temp_station = temp_station->next_station;
                counter++;
            }
            printf("======================================\n");
            print_error_code(SCHEDULE_SUCCESS);
            return;
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_LINE_DOESNT_EXIST);
    return;
}
/**********************************************************************************************************************/
/*
 * function: schedule_print_all_lines
 * parameters:
 *     schedule - Pointer to the linked list
 * errors:
 *      SCHEDULE_SUCCESS
 *     SCHEDULE_NULL_ARG
 * return value:
 *     - None
 */
void schedule_print_all_lines(schedule_line_node_t *head)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        {
            print_line(temp);
            station *temp_station = temp->first_station;
            if (temp_station != NULL)
            {
                printf("First Station: %s\n",temp_station->name);
                while (temp_station->next_station != NULL)
                {
                    temp_station = temp_station->next_station;
                }
                printf("Last Station: %s\n",temp_station->name);
            }
            else
            {
                printf("First Station: N/A\n");
                printf("Last Station: N/A\n");
            }
            printf("======================================\n");
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_SUCCESS);
    return;
}
/**********************************************************************************************************************/

/*
 * function: schedule_print_lines_for_station
 * parameters:
 *    schedule - Pointer to the linked list
 *    from - Name of the origin station
 *    to - Name of the destination station
 * errors:
 *     SCHEDULE_SUCCESS
 *    SCHEDULE_NULL_ARG
 * return value:
 *    - None
 */
void schedule_print_lines_for_stations(schedule_line_node_t *head, const char *from, const char *to)
{
    if (head == NULL)
    {
        print_error_code(SCHEDULE_NULL_ARG);
        return;
    }
    line *temp = head->first_line;
    while (temp != NULL)
    {
        station *temp_station = temp->first_station;
        while (temp_station != NULL)
        {
            station *first;
            if (strcmp(temp_station->name,from) == 0)
            {
                first = temp_station;
                while (temp_station != NULL)
                {
                    if(strcmp(temp_station->name,to) == 0)
                    {
                        print_line(temp);
                        while (strcmp(first->name,to) != 0)
                        {
                            printf("%s\n",first->name);
                            first=first->next_station;
                        }
                        printf("%s\n",first->name); 
                        printf("======================================\n");
                    }
                    temp_station = temp_station->next_station;
                }
                break;
            }
            temp_station = temp_station->next_station;
        }
       temp= temp->next_line;
    }
    print_error_code(SCHEDULE_SUCCESS);
    return;
}
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

int main(){}
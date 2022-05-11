#include "schedule.h"
#include <stdio.h>
#include <stdlib.h>
Schedule scheduleCreate(){
    Schedule new_schedule;
    new_schedule = (Schedule)calloc(1, sizeof(Schedule));
    if (new_schedule == NULL)
    {
        return NULL;
    }
    return new_schedule;
}

void scheduleDestroy(Schedule schedule){
    
}

ScheduleResult scheduleAddLine(Schedule schedule, ScheduleLineType type, int number,
				const char *description, double price){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    // line *temp = schedule->first_line;
    // while (temp != NULL)
    // {
    //     if(temp->line_number == line_number)
    //     {
    //         return SCHEDULE_LINE_ALREADY_EXISTS;
    //     }
    //    temp= temp->next_line;
    // }
    if (type < 0 || type > 2)
    {
        return SCHEDULE_INVALID_LINE_TYPE;
    }
    if (line_number < 0 || line_number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
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
    new_line->next_line = schedule->first_line;
    schedule->first_line = new_line;
    print_error_code(SCHEDULE_SUCCESS);
    return schedule;
        
}
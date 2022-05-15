#include "schedule_line.h"

struct schedule_line_s
{
    ScheduleLineType type;
    int number;
    const char *description;
    double price;
    ScheduleStationList station_list;
};

ScheduleLine schedule_line_create(ScheduleLineType type, int number, const char *description, double price)
{
    ScheduleLine new_line;
    new_line = (ScheduleLine)calloc(1,sizeof(struct schedule_line_s));
    if (new_line == NULL)
    {
        return NULL;
    }
    new_line->description = (char*)calloc(strlen(description)+1,sizeof(char));
    new_line->description = description;
    new_line->number = number;
    new_line->type = type;
    new_line->price = price;
    ScheduleLineResult result = stationListCreate(&new_line->station_list);
    if (result)
    {
        return NULL;
    }
    return new_line;
}
ScheduleLineResult schedule_line_destroy(ScheduleLine line)
{
    if (line == NULL || line->station_list == NULL)
    {
        return SCHEDULE_LINE_BAD_ARGUMENTS;
    }
    stationListDestroy(line->station_list);
    free((char*)line->description);
    free(line);
    return SCHEDULE_LINE_SUCCESS;
}
ScheduleLineResult schedule_line_get_stations(ScheduleLine line, ScheduleStationList *stations)
{
    if (line == NULL || stations == NULL)
    {
        return SCHEDULE_LINE_BAD_ARGUMENTS;
    }
    *stations = line->station_list;
    return SCHEDULE_LINE_SUCCESS;
}
ScheduleLineResult schedule_line_add_station(ScheduleLine line, ScheduleStation station)
{
    if (line == NULL || station == NULL)
    {
        return SCHEDULE_LINE_BAD_ARGUMENTS;
    }
    ScheduleLineResult result = stationListInsertLast(line->station_list,station);
    if (result)
    {
        return result;
    }
    return SCHEDULE_LINE_SUCCESS;
}
ScheduleLineResult schedule_line_get_details(ScheduleLine line,ScheduleLineType *type /* out */,int *number /* out */,char **description /* out */,double *price /* out */)
{
    if (line == NULL)
    {
        return SCHEDULE_LINE_BAD_ARGUMENTS;
    }
    if (type != NULL)
    {
        *type = line->type;
    }
    if (number != NULL)
    {
        *number = line->number;
    }
    if (description != NULL)
    {
        *description = (char*)line->description;
    }
    if (price != NULL)
    {
        *price = line->price;
    }
    return SCHEDULE_LINE_SUCCESS;
}

int print_line(ScheduleLine line){
    if (line == NULL)
    {
        printf("Unable to print line. NULL argument\n");
        return -1;
    }
    int result;
    ScheduleLineType curr_line_type = line->type;
    int curr_line_number = line->number;
    double curr_line_price = line->price;
    char *curr_line_description = (char*)line->description;
    ScheduleStationList stations_head = line->station_list;
    if (stations_head == NULL)
    {
        return -1;
    }
    stationListGotoHead(stations_head);
    FILE *output;
    output = stdout;
    ScheduleStation first_station, last_station;
    stationListGetCurrent(stations_head,&first_station);
    char *first_station_name,*last_station_name;
    schedule_station_get_name(first_station,&first_station_name);
    int num_of_stations = stationListGetNumElements(stations_head);
    if(num_of_stations == 0)
    {
        first_station_name = "N/A";
        last_station_name = "N/A";
    }
    else
    {
        do
        {
            result = stationListGotoNext(stations_head);
        } while (result != 1);
        stationListGetCurrent(stations_head,&last_station);
        schedule_station_get_name(last_station,&last_station_name);
    }
    return (schedulePrintLine(output,curr_line_type,curr_line_number,curr_line_description,first_station_name,last_station_name,curr_line_price));
}
#include "schedule.h"
struct schedule_s
{
	ScheduleLineList line_list;
};

Schedule scheduleCreate(){
    Schedule new_list;
    new_list = (Schedule)calloc(1,sizeof(struct schedule_s));
    if (new_list == NULL)
    {
        printf("Out of Memory\n");
        return NULL;
    }
    new_list->line_list = (ScheduleLineList)calloc(1,sizeof(ScheduleLineList));
    LineListResult result = lineListCreate(&(new_list->line_list));
    if(result)
    {
        printf("Out of Memory\n");
        free(new_list);
        return NULL;
    }
    return new_list;
}

void scheduleDestroy(Schedule schedule){
    lineListDestroy(schedule->line_list);
    free(schedule);
}

ScheduleResult scheduleAddLine(Schedule schedule, ScheduleLineType type, int number,
				const char *description, double price){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    if (type < 0 || type > 2)
    {
        return SCHEDULE_INVALID_LINE_TYPE;
    }
    if (number < 0 || number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
    }
    if (type == SCHEDULE_LINE_METRO || type == SCHEDULE_LINE_TRAIN)
    {
        if (strcmp(description,"Suburban") && strcmp(description,"Direct") && strcmp(description,"Intercity"))
        {
            return SCHEDULE_INVALID_TRAIN_TYPE;
        }
    }
    if (price <= 0)
    {
        return SCHEDULE_INVALID_PRICE;
    }
    ScheduleLineList temp = schedule->line_list;
    LineListResult result = lineListGotoHead(temp);
    if (result)
    {
        return result;
    }
    result = lineListFind(temp,number);
    if (result == LINE_LIST_SUCCESS)
    {
        return SCHEDULE_LINE_ALREADY_EXISTS;
    }
    ScheduleLine new_line = schedule_line_create(type, number, description, price);
    result = lineListInsertLast(schedule->line_list,new_line);
    // schedule_line_destroy(new_line);
    if (result)
    {
        return result;
    }
    return SCHEDULE_SUCCESS;
}

ScheduleResult scheduleRemoveLine(Schedule schedule, int number){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    if (number < 0 || number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
    }
    ScheduleLineList temp = schedule->line_list; 
    ScheduleResult result= lineListFind(temp,number);
    if (result == 0)
    {
        result = lineListRemoveCurrent(temp);
        if (result == 0)
        {
            return SCHEDULE_SUCCESS;
        }
        else if (result == 2)
        {
            return SCHEDULE_NULL_ARG;
        }
        else
        {
            return SCHEDULE_NO_LINES;
        }
        
    }
    if (result == 1)
    {
        return SCHEDULE_LINE_DOESNT_EXIST;
    }
    else if (result == 2)
    {
        return SCHEDULE_NULL_ARG;
    }
    else
    {
        return SCHEDULE_LINE_OUT_OF_MEMORY;
    }
}

ScheduleResult scheduleAddStationToLine(Schedule schedule, int number, const char *station, int time){
    if (schedule == NULL || station == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    if (number < 0 || number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
    }
    ScheduleLineList temp = schedule->line_list;
    lineListGotoHead(temp); 
    ScheduleResult result = lineListFind(temp,number);
    if (result == 1)
    {
        return SCHEDULE_LINE_DOESNT_EXIST;
    }
    ScheduleStation new_station = schedule_station_create(station,time);
    if (new_station == NULL)
    {
        return SCHEDULE_OUT_OF_MEMORY;
    }
    ScheduleLine curr_line;
    result = lineListGetCurrent(temp,&curr_line);
    if (result)
    {
        if (result == 1)
        {
            return SCHEDULE_LINE_DOESNT_EXIST;
        }
        if (result == 2)
        {
            return SCHEDULE_NULL_ARG;
        }
    }
    ScheduleStationList stations_head;
    result = schedule_line_get_stations(curr_line,&stations_head);
    if (result == 2)
    {
        return SCHEDULE_NULL_ARG;
    }
    stationListGotoHead(stations_head);
    int num_of_stations = stationListGetNumElements(stations_head);
    if (num_of_stations)
    {
        do
        {
            result = stationListGotoNext(stations_head);
        } while (result != 1);
        ScheduleStation last_station;
        int last_station_time;
        stationListGetCurrent(stations_head,&last_station);
        schedule_station_get_time(last_station,&last_station_time);
        if (last_station_time > time)
        {
            return SCHEDULE_INVALID_TIME;
        }
    }
    result = stationListInsertLast(stations_head,new_station);
    schedule_station_destroy(new_station);
    if (result == 0)
    {
        return SCHEDULE_SUCCESS;
    }
    if (result == 2)
    {
        return SCHEDULE_NULL_ARG;
    }
    return SCHEDULE_OUT_OF_MEMORY;
}

ScheduleResult scheduleRemoveStationFromLine(Schedule schedule, int number, int index){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    if (number < 0 || number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
    }
    ScheduleLineList temp = schedule->line_list;
    lineListGotoHead(temp); 
    ScheduleResult result = lineListFind(temp,number);
    if (result == 1)
    {
        return SCHEDULE_LINE_DOESNT_EXIST;
    }
    ScheduleLine curr_line;
    result = lineListGetCurrent(temp,&curr_line);
    if (result)
    {
        if (result == 1)
        {
            return SCHEDULE_LINE_DOESNT_EXIST;
        }
        if (result == 2)
        {
            return SCHEDULE_NULL_ARG;
        }
    }
    ScheduleStationList stations_head;
    result = schedule_line_get_stations(curr_line,&stations_head);
    if (result == 2)
    {
        return SCHEDULE_NULL_ARG;
    }
    stationListGotoHead(stations_head);
    int num_of_stations = stationListGetNumElements(stations_head);
    if (index != -1 && index >= num_of_stations)
    {
        return SCHEDULE_STATION_DOESNT_EXIST;
    }
    if (index ==  -1)
    {
        do
        {
            result = stationListGotoNext(stations_head);
        } while (result != 1);
    }
    else
    {
        for (int i = 0; i < index; i++)
        {
            result = stationListGotoNext(stations_head);
        }
    }
    stationListRemoveCurrent(stations_head);
    return SCHEDULE_SUCCESS;
}

ScheduleResult scheduleReportStationsForLine(Schedule schedule, int number){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    if (number < 0 || number > 9999)
    {
        return SCHEDULE_INVALID_LINE_NUMBER;
    }
    ScheduleLineList temp = schedule->line_list;
    lineListGotoHead(temp); 
    ScheduleResult result = lineListFind(temp,number);
    if (result == 1)
    {
        return SCHEDULE_LINE_DOESNT_EXIST;
    }
    ScheduleLine curr_line;
    result = lineListGetCurrent(temp,&curr_line);
    if (result)
    {
        if (result == 1)
        {
            return SCHEDULE_LINE_DOESNT_EXIST;
        }
        if (result == 2)
        {
            return SCHEDULE_NULL_ARG;
        }
    }
    ScheduleStationList stations_head;
    result = schedule_line_get_stations(curr_line,&stations_head);
    if (result == 2)
    {
        return SCHEDULE_NULL_ARG;
    }
    stationListGotoHead(stations_head);
    int num_of_stations = stationListGetNumElements(stations_head);
    if (num_of_stations == 0)
    {
        return SCHEDULE_STATION_DOESNT_EXIST;
    }
    ScheduleLineType curr_line_type;
    int curr_line_number;
    double curr_line_price;
    char *curr_line_description;
    schedule_line_get_details(curr_line,&curr_line_type,&curr_line_number,&curr_line_description,&curr_line_price);
    if (print_line(curr_line) == -1)
    {
        return SCHEDULE_NULL_ARG;
    }
    return SCHEDULE_SUCCESS;
}

ScheduleResult scheduleReportLines(Schedule schedule, ScheduleLineType type){
    if (schedule == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    ScheduleLineList line_list = schedule->line_list;
    ScheduleResult result  = lineListGotoHead(line_list);
    int num_of_lines = lineListGetNumElements(line_list);
    if (num_of_lines == 0)
    {
        return SCHEDULE_NO_LINES;
    }
    if (result)
    {
        return SCHEDULE_NO_LINES;
    }
    do
    {
        ScheduleLineType curr_line_type;
        ScheduleLine curr_line;
        lineListGetCurrent(line_list,&curr_line);
        schedule_line_get_details(curr_line,&curr_line_type,NULL,NULL,NULL);
        if (curr_line_type == type || type == SCHEDULE_LINE_ALL)
        {
            print_line(curr_line);
        }
        result = lineListGotoNext(line_list);
        
    } while (result != 1);
    return SCHEDULE_SUCCESS;
}
ScheduleResult scheduleReportLinesBetweenStations(Schedule schedule, const char *from, const char *to){
    if (schedule == NULL || from == NULL || to == NULL)
    {
        return SCHEDULE_NULL_ARG;
    }
    ScheduleLineList line_list = schedule->line_list;
    ScheduleStationList station_list;
    int num_of_lines = lineListGetNumElements(line_list);
    if (num_of_lines == 0)
    {
        return SCHEDULE_NO_LINES;
    }
    lineListSortBy(line_list,LINE_LIST_SORT_BY_PRICE);
    ScheduleResult result = lineListGotoHead(line_list);
    if (result)
    {
        return SCHEDULE_NULL_ARG;
    }
    ScheduleLine curr_line;
    result = lineListGetCurrent(line_list,&curr_line);
    if (result)
    {
        return SCHEDULE_NULL_ARG;
    }
    int print_count = 0;
    do
    {
        schedule_line_get_stations(curr_line,&station_list);
        result = stationListGotoHead(station_list);
        if (stationListGetNumElements(station_list))
        {
            do
            {
                ScheduleStation curr_station;
                char *first,*last;
                stationListGetCurrent(station_list,&curr_station);
                schedule_station_get_name(curr_station,&first);
                if ((strcmp(first,from) == 0))
                {
                    do
                    {
                        stationListGetCurrent(station_list,&curr_station);
                        schedule_station_get_name(curr_station,&last);
                        if ((strcmp(last,to) == 0))
                        {
                            print_line(curr_line);
                            print_count++;
                        }
                    } while ((stationListGotoNext(station_list) != 1));
                    break;
                }
            } while ((stationListGotoNext(station_list) != 1));
            break;
        }
    } while ((lineListGotoNext(line_list)!= 1));
    printf("P:%d\n",print_count);
    if (print_count)
    {
        return SCHEDULE_SUCCESS;
    }
    return SCHEDULE_STATION_DOESNT_EXIST;
}

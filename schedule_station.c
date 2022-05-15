#include "schedule_station.h"

struct schedule_station_s{
    const char *name;
    int time;
};

ScheduleStation schedule_station_create(const char *station, int time)
{
    ScheduleStation new_station;
    new_station = (ScheduleStation)calloc(1,sizeof(struct schedule_station_s));
    if (new_station == NULL)
    {
        return NULL;
    }
    new_station->name = (char*)calloc(strlen(station)+1,sizeof(char));
    if (new_station->name == NULL)
    {
        free(new_station);
        return NULL;
    }
    new_station->name = station;
    new_station->time = time;
    return new_station;
}
ScheduleStationResult schedule_station_destroy(ScheduleStation station)
{
    if (station == NULL)
    {
        return SCHEDULE_STATION_BAD_ARGUMENTS;
    }
    free((char*)station->name);
    free(station);
    return SCHEDULE_STATION_SUCCESS;
}
ScheduleStationResult schedule_station_get_name(ScheduleStation station, char **station_name)
{
    if (station == NULL || station_name == NULL)
    {
        return SCHEDULE_STATION_BAD_ARGUMENTS;
    }
    *station_name = (char*)station->name;
    return SCHEDULE_STATION_SUCCESS;
}

ScheduleStationResult schedule_station_get_time(ScheduleStation station, int *time)
{
    if (station == NULL || time == NULL)
    {
        return SCHEDULE_STATION_BAD_ARGUMENTS;
    }
    *time = station->time;
    return SCHEDULE_STATION_SUCCESS;
}
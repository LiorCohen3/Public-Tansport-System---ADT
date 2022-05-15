prog:main.o schedule.o schedule_line.o schedule_station.o
	gcc -std=c99 -Wall -pedantic-errors *.o -L. -lprog2 -o scheduler
main.o: main.c schedule.h pr2ex4.h schedule_line_list.h schedule_line.h schedule_station_list.h schedule_station.h
	gcc -c -std=c99 -Wall -pedantic-errors -L. -lprog2 main.c
schedule.o: schedule.c schedule.h pr2ex4.h schedule_line_list.h schedule_line.h schedule_station_list.h schedule_station.h
	gcc -c -std=c99 -Wall -pedantic-errors -L. -lprog2 schedule.c
schedule_line.o: schedule_line.c schedule_line.h pr2ex4.h schedule_station_list.h schedule_station.h
	gcc -c -std=c99 -Wall -pedantic-errors -L. -lprog2 schedule_line.c
schedule_station.o: schedule_station.c schedule_station.h pr2ex4.h
	gcc -c -std=c99 -Wall -pedantic-errors -L. -lprog2 schedule_station.c

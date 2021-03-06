#include "map.h"

void setRooms(Map *m)
{
	for (int i=0; i<MAP_SIZE(*m); i++) {
		if (m->map[i] == ROOM) {
			int *p = (int*)malloc(sizeof(int));
			*p = i;
			arraylist_add(m->rooms, (void*)p);
		}
	}
}

Map createMap(int width, int height, char *map)
{
	Map m;
	m.width = width;
	m.height = height;
	m.map = (char*)malloc(sizeof(char)*MAP_SIZE(m));
	assert(m.map);
	m.mutexs = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*MAP_SIZE(m));
	assert(m.mutexs);
	m.rooms = arraylist_create();

	for (int i=0; i<MAP_SIZE(m); i++) {
		pthread_mutex_init(&m.mutexs[i], NULL);
	}

	strncpy(m.map, map, MAP_SIZE(m));

	setRooms(&m);

	return m;
}

void deleteMap(Map *map)
{
	for (int i=0; i<MAP_SIZE(*map); i++) {
		pthread_mutex_destroy(&map->mutexs[i]);
	}
	map->width = map->height = 0;
	free(map->map);
	map = NULL;
	free(map->mutexs);
	map->mutexs = NULL;
	arraylist_destroy(map->rooms);
}

Map readMapFromFile(char *filename)
{
	int width, height;

	FILE *fp;
	fp = fopen(filename, "r+");
	if (fp == NULL) ERR("Cannot open map file\n");

	fscanf(fp, "%d %d", &width, &height);
	char map[width*height];
	for (int i=0; i<width*height; i++) {
		fscanf(fp, "%c", &map[i]);
		if (isspace(map[i])) i--;
	}

	fclose(fp);

	return createMap(width, height, map);
}

int indexOnMap(Map map, int x, int y)
{
	return map.width*y+x;
}

int getRandomRoom(Map *m)
{
	return *((int*)arraylist_get(m->rooms, rand() % arraylist_size(m->rooms)));
}

void printRooms(Map *map)
{
	for (int i=0; i<arraylist_size(map->rooms); i++) {
		printf("#%d\t%d\n", i, *((int*)arraylist_get(map->rooms, i)));
	}
}

void printMap(Map map)
{
	if (MAP_SIZE(map) < 1) {
		printf("Map is not loaded\n");
		return;
	}
	printf("%d %d\n", map.width, map.height);
	for (int j=0; j<map.height; j++) {
		for (int i=0; i<map.width; i++) {
			printf("%c", map.map[indexOnMap(map, i, j)]);
		}
		printf("\n");
	}

	printf("Rooms:\n");

	printRooms(&map);
}

int getCartesian(Map *map, int index, int *x, int *y)
{
	return 0;
}
char getField(int x, int y)
{
	return 0;
}

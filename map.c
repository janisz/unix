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
	m.mutexs = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*MAP_SIZE(m));
	m.rooms = arraylist_create();

	for (int i=0; i<MAP_SIZE(m); i++) {
		pthread_mutex_init(&m.mutexs[i], NULL);
	}
	if (m.map == 0 || m.mutexs == 0) ERR("Cannot alocate memory for map");

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
	free(map->mutexs);
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

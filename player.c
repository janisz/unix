#include "player.h"

#include "datastructs/src/arraylist.h"

Player createPlayer(const char *nick, int attribute, Position position, int descriptor)
{
	Player p;
	strncpy(p.nick, nick, NICK_LENGTH);
	p.attribute = attribute;
	p.position = position;
	descriptor = descriptor;
	return p;
}

void disposePlayer(Player player)
{
	close(player.descriptor);
}

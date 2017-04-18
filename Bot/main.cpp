

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Netica.h"
#include "NeticaEx.h"
#include "global.h"
#include "Bot.h"

int main (void){
	char mesg[MESG_LEN_ns];
	int res;

	std::srand(std::time(0));
	
	try {
		// Inicializa Netica
		env = NewNeticaEnviron_ns(nullptr, nullptr, nullptr);
		res = InitNetica2_bn(env, mesg);
		std::cout << mesg << std::endl;
		if (res < 0)  return -1;

		// Crea el bot
		Bot bot("../Net/bot.dne");
		bot.AskForNodeState("S_t");
		while (true) {
			bot.exec({
				"Health", "Weapon", "OpponentsWeapon", "HeardNoise",
				"NearEnemies", "PackWeapon", "PackHealth"
			});
		}

	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what();
	}

	res = CloseNetica_bn(env, mesg);
	std::cout << "\n" << mesg << std::endl;
	return (res < 0) ? -1 : 0;
}




#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Netica.h"
#include "NeticaEx.h"

extern environ_ns *env;

void CheckError(int line) {
	if (GetError_ns(env, ERROR_ERR, nullptr)) {
		char buffer[1024] = { '\0' };
		const char* msg = ErrorMessage_ns(GetError_ns(env, ERROR_ERR, nullptr));
		sprintf_s(buffer, "%s\tAt line %d", msg, line);
		throw std::runtime_error(buffer);
	}
}

const char* selectNodeState(const node_bn *node) {
	int states = GetNodeNumberStates_bn(node);
	int selected = -1;

	std::wcout << "\n" << GetNodeName_bn(node) << " state:" << std::endl;
	for (int i = 0; i < states; ++i) {
		std::cout << "\t" << i << "- " << GetNodeStateName_bn(node, i) << std::endl;
	}
	while ((selected < 0) || (selected >= states)) {
		std::cout << "Select state: ";
		std::cin >> selected;
	}
	return GetNodeStateName_bn(node, selected);
}

const char* AskInitialState(net_bn *net) {
	return selectNodeState(GetNode("S_t", net));
}

void setSensors(net_bn *net) {
	char buffer[128] = { '\0' };

	strcpy(buffer, selectNodeState(GetNode("Health", net)));
	EnterFinding("Health", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("Weapon", net)));
	EnterFinding("Weapon", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("OpponentsWeapon", net)));
	EnterFinding("OpponentsWeapon", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("HeardNoise", net)));
	EnterFinding("HeardNoise", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("NearEnemies", net)));
	EnterFinding("NearEnemies", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("PackWeapon", net)));
	EnterFinding("PackWeapon", buffer, net);

	strcpy(buffer, selectNodeState(GetNode("PackHealth", net)));
	EnterFinding("PackHealth", buffer, net);
}

int selectState(double beliefs[], int size) {
	double random = std::rand() / double(RAND_MAX);
	double inf_limit = 0;
	double sup_limit = 0;

	for (int i = 0; i < size; ++i) {
		sup_limit += beliefs[i];
		if ((random > inf_limit) && (random < sup_limit))
			return i;
		inf_limit += beliefs[i];
	}
	return -1;
}

void step(net_bn *net) {
	const int kStates = 6;
	char buffer[128] = { '\0' };
	double beliefs[kStates];
	int new_state = -1;
	
	node_bn* S_t_next = GetNode("S_t_next", net);
	for (int i = 0; i < kStates; ++i) {
		strcpy(buffer, GetNodeStateName_bn(S_t_next, i));
		beliefs[i] = GetNodeBelief("S_t_next", buffer, net);
	}

	while (new_state == -1)
		new_state = selectState(beliefs, kStates);

	std::cout << "\nNew state will be: " << GetNodeStateName_bn(S_t_next, new_state);
	std::cout << " its probability was: " << beliefs[new_state] << std::endl;

	RetractNodeFindings_bn(GetNode("S_t", net));
	RetractNodeFindings_bn(GetNode("Health", net));
	RetractNodeFindings_bn(GetNode("Weapon", net));
	RetractNodeFindings_bn(GetNode("OpponentsWeapon", net));
	RetractNodeFindings_bn(GetNode("HeardNoise", net));
	RetractNodeFindings_bn(GetNode("NearEnemies", net));
	RetractNodeFindings_bn(GetNode("PackWeapon", net));
	RetractNodeFindings_bn(GetNode("PackHealth", net));

	strcpy(buffer, GetNodeStateName_bn(S_t_next, new_state));
	EnterFinding("S_t", buffer, net);
}

int main (void){
	net_bn *net;
	node_bn *s_t, *s_t_next, *health, *weapon, *opponents_weapon, *heard_noise, *near_enemies, *pack_weapon, *pack_health;
	char mesg[MESG_LEN_ns];
	int res;

	std::srand(std::time(0));
	
	try {
		env = NewNeticaEnviron_ns(nullptr, nullptr, nullptr);
		res = InitNetica2_bn(env, mesg);

		std::cout << mesg << std::endl;
		if (res < 0)  return -1;
	
		net = NewNet_bn("BotNet", env);
		CheckError(__LINE__);

		s_t				 = NewNode_bn("S_t", 6, net);
		s_t_next		 = NewNode_bn("S_t_next", 6, net);
		health			 = NewNode_bn("Health", 2, net);
		weapon			 = NewNode_bn("Weapon", 2, net);
		opponents_weapon = NewNode_bn("OpponentsWeapon", 2, net);
		heard_noise		 = NewNode_bn("HeardNoise", 2, net);
		near_enemies	 = NewNode_bn("NearEnemies", 2, net);
		pack_weapon		 = NewNode_bn("PackWeapon", 2, net);
		pack_health		 = NewNode_bn("PackHealth", 2, net);
		CheckError(__LINE__);

		SetNodeStateNames_bn(s_t,      "Attack, SearchWeapon, SearchEnergy, Explore, Flee, DetectDanger");
		SetNodeStateNames_bn(s_t_next, "Attack, SearchWeapon, SearchEnergy, Explore, Flee, DetectDanger");
		SetNodeStateNames_bn(health, "High, Low");
		SetNodeStateNames_bn(weapon, "Armed, Unarmed");
		SetNodeStateNames_bn(opponents_weapon, "Armed, Unarmed");
		SetNodeStateNames_bn(heard_noise, "Yes, No");
		SetNodeStateNames_bn(near_enemies, "Many, Few");
		SetNodeStateNames_bn(pack_weapon, "Yes, No");
		SetNodeStateNames_bn(pack_health, "Yes, No");
		CheckError(__LINE__);

		AddLink_bn(s_t, s_t_next);
		AddLink_bn(s_t_next, health);
		AddLink_bn(s_t_next, weapon);
		AddLink_bn(s_t_next, opponents_weapon);
		AddLink_bn(s_t_next, heard_noise);
		AddLink_bn(s_t_next, near_enemies);
		AddLink_bn(s_t_next, pack_weapon);
		AddLink_bn(s_t_next, pack_health);
		CheckError(__LINE__);
		
		// S_t
		double val = 1.0 / 6.0;
		SetNodeProbs(s_t, val, val, val, val, val, val);
		CheckError(__LINE__);
		
		// S_t_next								Attack	SWeapon	SEnergy	Explore	Flee	DDanger
		SetNodeProbs(s_t_next, "Attack",		0.80,	0.02,	0.05,	0.01,	0.10,	0.02);
		SetNodeProbs(s_t_next, "SearchWeapon",	0.01,	0.70,	0.02,	0.10,	0.01,	0.16);
		SetNodeProbs(s_t_next, "SearchEnergy",	0.01,	0.02,	0.80,	0.07,	0.07,	0.03);
		SetNodeProbs(s_t_next, "Explore",		0.04,	0.04,	0.01,	0.70,	0.01,	0.20);
		SetNodeProbs(s_t_next, "Flee",			0.01,	0.01,	0.10,	0.01,	0.85,	0.02);
		SetNodeProbs(s_t_next, "DetectDanger",	0.40,	0.04,	0.03,	0.03,	0.30,	0.20);
		CheckError(__LINE__);

		// Health								High	Low
		SetNodeProbs(health, "Attack",			0.75,	0.25);
		SetNodeProbs(health, "SearchWeapon",	0.60,	0.40);
		SetNodeProbs(health, "SearchEnergy",	0.30,	0.70);
		SetNodeProbs(health, "Explore",			0.80,	0.20);
		SetNodeProbs(health, "Flee",			0.15,	0.85);
		SetNodeProbs(health, "DetectDanger",	0.55,	0.45);
		CheckError(__LINE__);

		// Weapon								Armed	Unarmed
		SetNodeProbs(weapon, "Attack",			0.99,	0.01);
		SetNodeProbs(weapon, "SearchWeapon",	0.10,	0.90);
		SetNodeProbs(weapon, "SearchEnergy",	0.40,	0.60);
		SetNodeProbs(weapon, "Explore",			0.80,	0.20);
		SetNodeProbs(weapon, "Flee",			0.30,	0.70);
		SetNodeProbs(weapon, "DetectDanger",	0.45,	0.55);
		CheckError(__LINE__);

		// OpponentsWeapon								Armed	Unarmed
		SetNodeProbs(opponents_weapon, "Attack",		0.40,	0.60);
		SetNodeProbs(opponents_weapon, "SearchWeapon",	0.70,	0.30);
		SetNodeProbs(opponents_weapon, "SearchEnergy",	0.60,	0.40);
		SetNodeProbs(opponents_weapon, "Explore",		0.50,	0.50);
		SetNodeProbs(opponents_weapon, "Flee",			0.70,	0.30);
		SetNodeProbs(opponents_weapon, "DetectDanger",	0.50,	0.50);
		CheckError(__LINE__);

		// HeardNoise								Yes		No
		SetNodeProbs(heard_noise, "Attack",			0.70,	0.30);
		SetNodeProbs(heard_noise, "SearchWeapon",	0.60,	0.40);
		SetNodeProbs(heard_noise, "SearchEnergy",	0.65,	0.35);
		SetNodeProbs(heard_noise, "Explore",		0.30,	0.70);
		SetNodeProbs(heard_noise, "Flee",			0.60,	0.40);
		SetNodeProbs(heard_noise, "DetectDanger",	0.80,	0.20);
		CheckError(__LINE__);

		// NearEnemies								Many	Few
		SetNodeProbs(near_enemies, "Attack",		0.30,	0.70);
		SetNodeProbs(near_enemies, "SearchWeapon",	0.45,	0.55);
		SetNodeProbs(near_enemies, "SearchEnergy",	0.60,	0.40);
		SetNodeProbs(near_enemies, "Explore",		0.40,	0.60);
		SetNodeProbs(near_enemies, "Flee",			0.80,	0.20);
		SetNodeProbs(near_enemies, "DetectDanger",	0.20,	0.80);
		CheckError(__LINE__);

		// PackWeapon								Yes		No
		SetNodeProbs(pack_weapon, "Attack",			0.50,	0.50);
		SetNodeProbs(pack_weapon, "SearchWeapon",	0.90,	0.10);
		SetNodeProbs(pack_weapon, "SearchEnergy",	0.45,	0.55);
		SetNodeProbs(pack_weapon, "Explore",		0.60,	0.40);
		SetNodeProbs(pack_weapon, "Flee",			0.30,	0.70);
		SetNodeProbs(pack_weapon, "DetectDanger",	0.40,	0.60);
		CheckError(__LINE__);

		// PackHealth								Yes		No
		SetNodeProbs(pack_health, "Attack",			0.70,	0.30);
		SetNodeProbs(pack_health, "SearchWeapon",	0.20,	0.80);
		SetNodeProbs(pack_health, "SearchEnergy",	0.90,	0.10);
		SetNodeProbs(pack_health, "Explore",		0.60,	0.40);
		SetNodeProbs(pack_health, "Flee",			0.20,	0.80);
		SetNodeProbs(pack_health, "DetectDanger",	0.45,	0.55);
		CheckError(__LINE__);

		CompileNet_bn(net);
		CheckError(__LINE__);

		WriteNet_bn(net, NewFileStream_ns("../Net/bot.dne", env, NULL));
		CheckError(__LINE__);

		// Set the initial state
		char buffer[128] = { '\0' };
		strcpy(buffer, AskInitialState(net));
		EnterFinding("S_t", buffer, net);


		while (true) {
			setSensors(net);
			CheckError(__LINE__);
			step(net);
			CheckError(__LINE__);
		}

		/*
		belief = GetNodeBelief ("Tuberculosis", "present", net);
		CheckError();

		printf ("The probability of tuberculosis is %g\n\n", belief);

		EnterFinding ("XRay", "abnormal", net);
		belief = GetNodeBelief ("Tuberculosis", "present", net);
		CheckError();

		printf ("Given an abnormal X-ray, \n\
			 the probability of tuberculosis is %g\n\n", belief);

		EnterFinding ("VisitAsia", "visit", net);
		belief = GetNodeBelief ("Tuberculosis", "present", net);
		CheckError();

		printf ("Given an abnormal X-ray and a visit to Asia, \n\
			 the probability of tuberculosis is %g\n\n", belief);

		EnterFinding ("Cancer", "present", net);
		belief = GetNodeBelief ("Tuberculosis", "present", net);
		CheckError();

		printf ("Given abnormal X-ray, Asia visit, and lung cancer, \n\
			 the probability of tuberculosis is %g\n\n", belief);*/

	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what();
	}

	DeleteNet_bn(net);
	res = CloseNetica_bn(env, mesg);
	std::cout << "\n" << mesg << std::endl;
	return (res < 0) ? -1 : 0;
}


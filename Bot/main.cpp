

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Netica.h"
#include "NeticaEx.h"

void CheckError() {
	if (GetError_ns(env, ERROR_ERR, nullptr)) {
		const char* msg = ErrorMessage_ns(GetError_ns(env, ERROR_ERR, nullptr));
		throw std::runtime_error(msg);
	}
}

extern environ_ns *env;

int main (void){
	net_bn *net;
	node_bn *s_t, *s_t_next, *health, *weapon, *opponents_weapon, *heard_noise, *near_enemies, *pack_weapon, *pack_health;
	double belief;
	char mesg[MESG_LEN_ns];
	int res;
	
	try {
		env = NewNeticaEnviron_ns(nullptr, nullptr, nullptr);
		res = InitNetica2_bn(env, mesg);

		printf ("%s\n", mesg);
		if (res < 0)  return -1;
	
		net = NewNet_bn("BotNet", env);
		CheckError();

		s_t				 = NewNode_bn("S_t", 6, net);
		s_t_next		 = NewNode_bn("S_t_next", 6, net);
		health			 = NewNode_bn("Health", 2, net);
		weapon			 = NewNode_bn("Weapon", 2, net);
		opponents_weapon = NewNode_bn("OpponentsWeapon", 2, net);
		heard_noise		 = NewNode_bn("HeardNoise", 2, net);
		near_enemies	 = NewNode_bn("NearEnemies", 2, net);
		pack_weapon		 = NewNode_bn("PackWeapon", 2, net);
		pack_health		 = NewNode_bn("PackHealth", 2, net);
		CheckError();

		SetNodeStateNames_bn(s_t,      "Attack, SearchWeapon, SearchEnergy, Explore, Flee, DetectDanger");
		SetNodeStateNames_bn(s_t_next, "Attack, SearchWeapon, SearchEnergy, Explore, Flee, DetectDanger");
		SetNodeStateNames_bn(health, "High, Low");
		SetNodeStateNames_bn(weapon, "Armed, Unarmed");
		SetNodeStateNames_bn(opponents_weapon, "Armed, Unarmed");
		SetNodeStateNames_bn(heard_noise, "Yes, No");
		SetNodeStateNames_bn(near_enemies, "Many, Few");
		SetNodeStateNames_bn(pack_weapon, "Yes, No");
		SetNodeStateNames_bn(pack_health, "Yes, No");
		CheckError();

		AddLink_bn(s_t, s_t_next);
		AddLink_bn(s_t_next, health);
		AddLink_bn(s_t_next, weapon);
		AddLink_bn(s_t_next, opponents_weapon);
		AddLink_bn(s_t_next, heard_noise);
		AddLink_bn(s_t_next, near_enemies);
		AddLink_bn(s_t_next, pack_weapon);
		AddLink_bn(s_t_next, pack_health);
		CheckError();
		
		// S_t
		SetNodeProbs(s_t, 0.33, 0.33, 0.33, 0.33, 0.33, 0.33);
		
		// S_t_next					Attack	SWeapon	SEnergy	Explore	Flee	DDanger
		SetNodeProbs(s_t_next, "Attack",		0.80,	0.02,	0.05,	0.01,	0.10,	0.02	);
		SetNodeProbs(s_t_next, "SearchWeapon",		0.01,	0.70,	0.02,	0.10,	0.01	0.16	);
		SetNodeProbs(s_t_next, "SearchEnergy",		0.01,	0.02,	0.80,	0.07,	0.07,	0.03	);
		SetNodeProbs(s_t_next, "Explore",		0.04,	0.04,	0.01,	0.70,	0.01,	0.20	);
		SetNodeProbs(s_t_next, "Flee",			0.01,	0.01,	0.10,	0.01,	0.85,	0.02	);
		SetNodeProbs(s_t_next, "DetectDanger",		0.40,	0.04,	0.03,	0.03,	0.30,	0.20	);

		// Health					High	Low
		SetNodeProbs(s_t_next, "Attack",		0.75,	0.25	);
		SetNodeProbs(s_t_next, "SearchWeapon",		0.60,	0.40	);
		SetNodeProbs(s_t_next, "SearchEnergy",		0.30,	0.70	);
		SetNodeProbs(s_t_next, "Explore",		0.80,	0.20	);
		SetNodeProbs(s_t_next, "Flee",			0.15,	0.85	);
		SetNodeProbs(s_t_next, "DetectDanger",		0.55,	0.45	);

		// Weapon					Armed	Unarmed
		SetNodeProbs(s_t_next, "Attack",		0.99,	0.01	);
		SetNodeProbs(s_t_next, "SearchWeapon",		0.10,	0.90	);
		SetNodeProbs(s_t_next, "SearchEnergy",		0.40,	0.60	);
		SetNodeProbs(s_t_next, "Explore",		0.80,	0.20	);
		SetNodeProbs(s_t_next, "Flee",			0.30,	0.70	);
		SetNodeProbs(s_t_next, "DetectDanger",		0.45,	0.55	);

		// OpponentsWeapon				Armed	Unarmed
		SetNodeProbs(s_t_next, "Attack",		0.40,	0.60	);
		SetNodeProbs(s_t_next, "SearchWeapon",		0.70,	0.30	);
		SetNodeProbs(s_t_next, "SearchEnergy",		0.60,	0.40	);
		SetNodeProbs(s_t_next, "Explore",		0.50,	0.50	);
		SetNodeProbs(s_t_next, "Flee",			0.70,	0.30	);
		SetNodeProbs(s_t_next, "DetectDanger",		0.50,	0.50	);

		// HeardNoise								Alto	Bajo
		SetNodeProbs(s_t_next, "Attack", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchWeapon", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchEnergy", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Explore", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Flee", 0.80, 0.02);
		SetNodeProbs(s_t_next, "DetectDanger", 0.80, 0.02);

		// NearEnemies								Alto	Bajo
		SetNodeProbs(s_t_next, "Attack", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchWeapon", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchEnergy", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Explore", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Flee", 0.80, 0.02);
		SetNodeProbs(s_t_next, "DetectDanger", 0.80, 0.02);

		// PackWeapon								Alto	Bajo
		SetNodeProbs(s_t_next, "Attack", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchWeapon", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchEnergy", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Explore", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Flee", 0.80, 0.02);
		SetNodeProbs(s_t_next, "DetectDanger", 0.80, 0.02);

		// PackHealth								Alto	Bajo
		SetNodeProbs(s_t_next, "Attack", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchWeapon", 0.80, 0.02);
		SetNodeProbs(s_t_next, "SearchEnergy", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Explore", 0.80, 0.02);
		SetNodeProbs(s_t_next, "Flee", 0.80, 0.02);
		SetNodeProbs(s_t_next, "DetectDanger", 0.80, 0.02);

		CheckError();

		CompileNet_bn(net);
		CheckError();

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
			 the probability of tuberculosis is %g\n\n", belief);

	} catch (const std::exception& e) {
		std::cout << "Error: " << e.what();
	}

	DeleteNet_bn(net);
	res = CloseNetica_bn(env, mesg);
	printf ("%s\n", mesg);
	printf ("Press <enter> key to quit ", mesg);
	getchar();
	return (res < 0) ? -1 : 0;
}


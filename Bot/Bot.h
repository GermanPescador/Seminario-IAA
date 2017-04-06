#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Netica.h"
#include "NeticaEx.h"
#include "global.h"

class Bot 
{
	private:
		const char* kNetName = "BotNet";
		net_bn* net_;	// Bayes decision net

	public:
		Bot();
		Bot(const std::string& net_path);
		virtual ~Bot();

		void LoadNet(const std::string& net_path);
		net_bn* GetNet();
		void exec(const std::vector<char*>& node_names);
		void AskForNodeState(char* node_name);

	private:
		int selectRandomFromProb(const std::vector<double>& prob) const;
		void CheckError();
};


#include "Bot.h"

Bot::Bot():
	net_(nullptr)
{}

Bot::Bot(const std::string& net_path):
	net_(nullptr)
{
	LoadNet(net_path);
}

Bot::~Bot()
{
	if (net_)
		DeleteNet_bn(net_);
}

void Bot::LoadNet(const std::string& net_path)
{
	if (net_)
		DeleteNet_bn(net_);

	net_ = ReadNet_bn(NewFileStream_ns(net_path.c_str(), env, NULL), NO_VISUAL_INFO);
	CheckError();
	CompileNet_bn(net_);
	CheckError();
}

net_bn* Bot::GetNet()
{
	return net_;
}

void Bot::exec(const std::vector<char*>& node_names)
{
	char buffer[128] = { '\0' };

	for (char* node_name : node_names)
		AskForNodeState(node_name);

	node_bn* S_t_next = GetNode("S_t_next", net_); // Next state node
	int states = GetNodeNumberStates_bn(S_t_next);
	std::vector<double> states_prob(states); // Probabilities of each state

	for (int i = 0; i < states; ++i) { // Get the probabilities of each state
		strcpy(buffer, GetNodeStateName_bn(S_t_next, i));
		states_prob[i] = GetNodeBelief("S_t_next", buffer, net_);
	}

	int new_state = selectRandomFromProb(states_prob);

	std::cout << "\nNew state will be: " << GetNodeStateName_bn(S_t_next, new_state);
	std::cout << " its probability was: " << states_prob[new_state] << std::endl;

	RetractNodeFindings_bn(GetNode("S_t", net_));
	RetractNodeFindings_bn(GetNode("Health", net_));
	RetractNodeFindings_bn(GetNode("Weapon", net_));
	RetractNodeFindings_bn(GetNode("OpponentsWeapon", net_));
	RetractNodeFindings_bn(GetNode("HeardNoise", net_));
	RetractNodeFindings_bn(GetNode("NearEnemies", net_));
	RetractNodeFindings_bn(GetNode("PackWeapon", net_));
	RetractNodeFindings_bn(GetNode("PackHealth", net_));

	strcpy(buffer, GetNodeStateName_bn(S_t_next, new_state));
	EnterFinding("S_t", buffer, net_);
}

void Bot::AskForNodeState(char * node_name)
{
	node_bn* node = GetNode(node_name, net_);
	if (!node) {
		return;
	}

	int states = GetNodeNumberStates_bn(node);
	char buffer[128] = { '\0' };
	int selected = -1;

	std::cout << "\n" << node_name << " state:" << std::endl;
	for (int i = 0; i < states; ++i) {
		std::cout << "\t" << i << "- " << GetNodeStateName_bn(node, i) << "\n";
	}
	while ((selected < 0) || (selected >= states)) {
		std::cout << "Select state: ";
		std::cin >> selected;
	}
	strcpy(buffer, GetNodeStateName_bn(node, selected));
	EnterFinding(node_name, buffer, net_);
	CheckError();
}

int Bot::selectRandomFromProb(const std::vector<double>& prob) const
{
	double random = std::rand() / double(RAND_MAX);
	double inf_limit = 0;
	double sup_limit = 0;
	int size = prob.size();

	for (int i = 0; i < size; ++i) {
		sup_limit += prob[i];
		if ((random > inf_limit) && (random < sup_limit))
			return i;
		inf_limit += prob[i];
	}

	return 0;
}

void Bot::CheckError()
{
	if (GetError_ns(env, ERROR_ERR, nullptr)) {
		throw std::runtime_error(ErrorMessage_ns(GetError_ns(env, ERROR_ERR, nullptr)));
	}
}

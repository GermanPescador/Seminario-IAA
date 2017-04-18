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
		const char* kNetName = "BotNet"; // Nombre que recibir� la red del Bot.
		net_bn* net_;					 // Bayes decision net.

	public:

		/*
		 * Inicializa un bot sin red.
		 */
		Bot();

		/*
		 * Inicializa un bot y carga los contenidos de un archivo del netica
		 * en la red.
		 */
		Bot(const std::string& net_path);

		/*
		 * Borra la red utilizada por el bot si existe.
		 */
		virtual ~Bot();

		/*
		 * Carga una red almacenada en un archivo del netica y compila.
		 */
		void LoadNet(const std::string& net_path);

		/*
		 * Pide el valor de los nodos sensores por pantalla, despu�s 
		 * calcula el siguiente estado a trav�s de las probabilidades resultates
		 * en S_t_next. Coloca el nuevo resultado en el nodo S_t y borra los estados
		 * de los nodos sensores. 
		 * Recibe por par�metro un vector con el nombre de los nodos sensores.
		 */
		void exec(const std::vector<char*>& node_names);

		/*
		 * Pide por pantalla el estado de uno de los nodos de la red.
		 */
		void AskForNodeState(char* node_name);

	private:

		/*
		 * Recibe un vector deprobabilidades que debe sumar 1. Devuelve el n�mero de la posici�n
		 * seleccionada aleatoriamente de acuerdo a esas probabilidades.
		 */
		int selectRandomFromProb(const std::vector<double>& prob) const;

		/*
		 * Comprueba si se ha producido alg�n error en Netica y l�nzalo como una excepci�n.
		 */
		void CheckError();
};


/*
 * Vehicle.cpp
 *
 *  Created on: 16/05/2019
 *      Author: eduribeiro
 */


#include "Vehicle.h"


int Vehicle::getID() const {
	return id;
}


TYPE Vehicle::getType() const {
	return type;
}


vector<Vertex<Node>* > Vehicle::getVehiclePath() const {
	return vehiclePath;
}


vector<Delivery> Vehicle::getDeliveries() const {
	return deliveries;
}


void Vehicle::addVertexToPath(Vertex<Node>* vertex, int index) {

	if(index >= vehiclePath.size()) {
		cout << "Invalid index!" << endl;
		return;
	}

	vehiclePath.insert(vehiclePath.begin() + index, vertex);
}


void Vehicle::addDelivery(Delivery delivery) {
	deliveries.push_back(delivery);
}


double Vehicle::testInsertDelivery(Delivery delivery, Table table, int& bestPositionOrigin, int& bestPositionDestination) {

	double deltaOrigin, deltaDestination;

	// caso particular: o veiculo ainda nao tem nenhuma entrega associada
	if(deliveries.empty()) {
		bestPositionOrigin = 1;
		bestPositionDestination = 2;

		return;
	}

	Vertex<Node>* origin = delivery.getOrigem();
	Vertex<Node>* destination = delivery.getDestino();

	double minDistance = INF;

	// nao considera a primeira e ultima posicoes do vetor
	// de vertices, uma vez que estes sao a central.
	for(int i = 1; i < vehiclePath.size() - 1; i++) {

		Vertex<Node>* previous = vehiclePath.at(i - 1);
		Vertex<Node>* next = vehiclePath.at(i + 1);

		// NOTA: A funcao admite que a conectividade do grafo
		// ja foi avaliada, e que existe sempre um caminho entre
		// qualquer par de vertices.

		double curDistance = getDistFromTable(previous, origin, table) +
						  	 getDistFromTable(origin, next, table) -
							 getDistFromTable(previous, next, table);

		if(curDistance < minDistance) {
			minDistance = curDistance;
			bestPositionOrigin = i;
		}
	}

	minDistance = INF;

	// mesmo processo para o destino da entrega, so que o destino tem de vir
	// obrigatoriamente depois da origem
	for(int i = bestPositionOrigin + 1; i < vehiclePath.size() - 1; i++) {

		Vertex<Node>* previous = vehiclePath.at(i - 1);
		Vertex<Node>* next = vehiclePath.at(i + 1);

		// NOTA: A funcao admite que a conectividade do grafo
		// ja foi avaliada, e que existe sempre um caminho entre
		// qualquer par de vertices.

		double curDistance = getDistFromTable(previous, origin, table) +
						  	 getDistFromTable(origin, next, table) -
							 getDistFromTable(previous, next, table);

		if(curDistance < minDistance) {
			minDistance = curDistance;
			bestPositionDestination = i;
		}
	}

}







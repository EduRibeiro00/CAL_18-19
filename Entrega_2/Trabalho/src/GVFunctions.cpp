#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "GVFunctions.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>

using namespace std;

string colors[8] = {"BLUE", "RED", "GREEN", "WHITE", "ORANGE", "YELLOW", "LIGHT_GRAY", "BLACK" };


GraphViewer* displayGraph(Graph<Node>& graph, string edgeColor, int vertexSize, int& width, int& height) {

		double maxX = 0, maxY = 0, minX = 9999999999, minY = 9999999999;

		for(unsigned i = 0; i < graph.getVertexSet().size(); i++) {

			Node node = graph.getVertexSet().at(i)->getInfo();
			double currentX = node.getX();
			double currentY = node.getY();

			if(currentX > maxX)
				maxX = currentX;

			if(currentX < minX)
				minX = currentX;

			if(currentY > maxY)
				maxY = currentY;

			if(currentY < minY)
				minY = currentY;
		}


		// --------
		// CALCULO DE DIMENSOES
		// --------

		height = 800;
		double gravWidth = maxX - minX;
		double gravHeight = maxY - minY;

		// uso de uma regra de tres simples para calcular o comprimento ideal para a janela
		width = max(height, (int) (height * gravHeight / gravWidth));


		GraphViewer *gv = new GraphViewer(width, height, false);
		gv->createWindow(width, height);
		gv->defineEdgeColor(edgeColor);

		gv->defineVertexSize(vertexSize);
		gv->defineEdgeCurved(false);

		// --------
		// VERTICES
		// --------

		for(auto v : graph.getVertexSet()) {

			Node& node = v->getInfo();

			double displayX = (node.getX() - minX ) * width / gravWidth * 0.95;
			double displayY = (node.getY() - minY ) * height / gravHeight * 0.95;

			displayX += (0.025 * width);
			displayY += (0.025 * height);

			gv->addNode(node.getID(), (int) displayX, (int) displayY);

			node.setDisplayX((int) displayX);
			node.setDisplayY((int) displayY);


/*
			// TIRAR DPS
			stringstream ss;
			ss << node.getID();
			gv->setVertexLabel(node.getID(), ss.str());
*/


			gv->setVertexColor(node.getID(), colors[node.getType()]);

			if (node.getType() == CENTRAL) {
                gv->setVertexLabel(node.getID(), "CENTRAL");
                gv->setVertexSize(node.getID(), 5);
            }

		}


		// --------
		// ARESTAS
		// --------

		int idAresta = 1;


		for(unsigned i = 0; i < graph.getVertexSet().size(); i++){

			Node init_node = graph.getVertexSet().at(i)->getInfo();
			vector<Edge<Node> > edges = graph.getVertexSet().at(i)->getAdj();

			for(unsigned j = 0; j < edges.size(); j++){
				if(edges.at(j).shouldBeDisplayed()) {
					gv->addEdge(idAresta, init_node.getID(), edges.at(j).getDest()->getInfo().getID(), EdgeType::UNDIRECTED);

/*
					// TIRAR DPS
					stringstream ss;
					ss << idAresta << " - " << fixed << setprecision(5) << edges.at(j).getWeight();
					gv->setEdgeLabel(idAresta, ss.str());
*/

                    idAresta++;
				}
			}

		}


		gv->rearrange();

		return gv;
}




GraphViewer* displayVehiclePaths(Graph<Node>& graph, vector<Vehicle*> vehicles, Table table, int width, int height, string edgeColor) {

	GraphViewer* gv = new GraphViewer(width, height, false);
	gv->createWindow(width, height);
	gv->defineEdgeColor(edgeColor);
	gv->defineEdgeCurved(false);

	int idAresta = 1;

	for(auto v : vehicles) {

		if(!v->getDeliveries().empty()) {

			vector<Vertex<Node>* > path = v->getVehiclePath();

			for(int i = path.size() - 1; i >= 1; i--) {

				Vertex<Node>* v = path.at(i);
				Vertex<Node>* s = path.at(i - 1);

				while(s != v) {
					Vertex<Node>* t = getPathFromTable(s, v, table);

					cout << "t: " << t->getInfo().getID() << " v: " << v->getInfo().getID();

					Edge<Node>* e = graph.getEdge(t->getInfo(), v->getInfo());

					if(e != NULL && e->shouldBeDisplayed()) {

						cout << " - deu display";

						gv->addNode(t->getInfo().getID(), t->getInfo().getDisplayX(), t->getInfo().getDisplayY());
						gv->addNode(v->getInfo().getID(), v->getInfo().getDisplayX(), v->getInfo().getDisplayY());
						gv->setVertexColor(t->getInfo().getID(), colors[t->getInfo().getType()]);
						gv->setVertexColor(v->getInfo().getID(), colors[v->getInfo().getType()]);

/*
						// TIRAR DPS
						stringstream ssT, ssV;
						ssT << t->getInfo().getID();
						gv->setVertexLabel(t->getInfo().getID(), ssT.str());
						ssV << v->getInfo().getID();
						gv->setVertexLabel(v->getInfo().getID(), ssV.str());
*/


						gv->addEdge(idAresta, t->getInfo().getID(), v->getInfo().getID(), EdgeType::UNDIRECTED);
						idAresta++;

						// de modo a nao desenhar a mesma aresta da proxima vez
						e->setShouldDisplay(false);
					}
					else {

						e = graph.getEdge(v->getInfo(), t->getInfo());

						if(e != NULL && e->shouldBeDisplayed()) {

							cout << " - deu display";

							gv->addNode(t->getInfo().getID(), t->getInfo().getDisplayX(), t->getInfo().getDisplayY());
							gv->addNode(v->getInfo().getID(), v->getInfo().getDisplayX(), v->getInfo().getDisplayY());
							gv->setVertexColor(t->getInfo().getID(), colors[t->getInfo().getType()]);
							gv->setVertexColor(v->getInfo().getID(), colors[v->getInfo().getType()]);

/*
							// TIRAR DPS
							stringstream ssT, ssV;
							ssT << t->getInfo().getID();
							gv->setVertexLabel(t->getInfo().getID(), ssT.str());
							ssV << v->getInfo().getID();
							gv->setVertexLabel(v->getInfo().getID(), ssV.str());
*/

							gv->addEdge(idAresta, t->getInfo().getID(), v->getInfo().getID(), EdgeType::UNDIRECTED);
							idAresta++;

							// de modo a nao desenhar a mesma aresta da proxima vez
							e->setShouldDisplay(false);
						}
					}

					cout << endl;

					v = t;
				}

			}
		}
	}

	return gv;
}
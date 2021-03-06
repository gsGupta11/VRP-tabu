/*****************************************************************************
    This file is part of VRP.

    VRP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VRP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VRP.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "Graph.h"

/** @brief ###Insert a vertex.
 *
 * Create and insert a vertex in the graph.
 * @param[in] cust The customer who form the vertex
 */
void Graph::InsertVertex(Customer &cust) {
    Vertex::ConstructionToken c;
    Vertex v { c };
    InsertVertex(cust, v);
}

/** @brief ###Insert a vertex.
 *
 * Insert a vertex in the graph.
 * @param[in] c The customer who form the vertex
 * @param[in] v The vertex created
 */
void Graph::InsertVertex(Customer &c, Vertex &v) {
    std::pair<Customer, Vertex> temp (c, v);
    vertexes.insert(temp);
}

/** @brief ###Insert an Edge.
 *
 * Insert an edge with weight from a customer to another.
 * @param[in] node The starting customer
 * @param[in] new_edge The destination customer
 * @param[in] weight The weight of the edge
 */
void Graph::InsertEdge(Customer &node, Customer &new_edge, int weight) {
    auto it = vertexes.find(node);
    if (node.name != new_edge.name && it != vertexes.end())
        it->second.InsertEdge(new_edge, weight);
}

/** @brief ###Remove an edge.
 *
 * @param[in] node The customer which the edge start
 * @param[in] edge The customer which the edge finish
 */
void Graph::RemoveEdge(Customer &node, Customer &edge) {
    auto it = vertexes.find(node);
    if(it != vertexes.end())
        it->second.RemoveEdge(edge);
}

/** @brief ###Sort the customers by distance.
 *
 * This function sorts the customer by distance from the depot;
 * the order is crescent.
 * @return The map of customer sorted
 */
std::multimap<int, Customer> Graph::sortV0() {
    std::multimap<int, Customer> v;
    // get depot
    Customer c = vertexes.begin()->first;
    // insert the depot
    v.emplace(std::pair<int, Customer>(0, c));
    Vertex it = vertexes.find(c)->second;
    for (auto &edge : it.GetEdges()) {
        v.emplace(std::pair<int, Customer>(edge.second.weight, edge.first));
    }
    return v;
}

/** @brief ###Find the neighborhood of a customer.
 *
 * This function sorts the neighborhood of a customer by distance;
 * the order is crescent.
 * @return The map of customer sorted
 */
std::multimap<int, Customer> Graph::GetNeighborhood(Customer c) const {
    std::multimap<int, Customer> mm;
    Vertex it = vertexes.find(c)->second;
    Customer depot = vertexes.begin()->first;
    for (std::pair<Customer, Edge> edge : it.GetEdges()) {
        if (edge.first != depot)
            mm.emplace(std::pair<int, Customer>(edge.second.weight, edge.first));
	}
    return mm;
}

/** @brief ###Return the weight of an edge.
 *
 * This function compute the cost of travelling from a customer to another.
 * @param[in] from The starting customer
 * @param[in] to The ending customer.
 * @return The cost of the travel
 */
std::pair<Customer, int> Graph::GetCosts(const Customer &from, const Customer &to) const {
    /* get all edges from &from */
    Vertex it = vertexes.find(from)->second;
    return {from, it.GetEdges().find(to)->second.weight};
}

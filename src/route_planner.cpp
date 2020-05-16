#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    
    //initializing start_node and end_node
    //calls FindClosestNode method from route_model.cpp
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    
    //pointer stores address in memory of variable
    // -> accesses value of *node (pointer node)
    //Access a member function/member variable of an object through a pointer, as opposed to a regular variable or reference.
    /*
     a->b means (*a).b.

     If a is a pointer, a->b is the member b of which a points to.

     */
    //Calls FindNeighbor method in route_model.cpp
    return end_node->distance(*node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    
    //Same as below *current_node.FindNeighbors()
    //Below is arrow pointer and it dereferences a pointer in this case *current_node
    //Dereferencing means to get value of address
    current_node->FindNeighbors();
    
    for (auto n : current_node->neighbors) {
        
        if (n->visited) {
            continue;
        }
    
        //dereferences n and gets values of address for parent which becomes current_node
        n->parent = current_node;
        
        //uses current node's value, assigns it to g_value and adds it to H-Value
        n->g_value = current_node->g_value + current_node->distance(*n);
        
        //gets value of n, puts it into h_value and uses it for CalculateHValue() method
        n->h_value = CalculateHValue(n);
        
        //adds neighbor to open_list
        open_list.push_back(n);
        
        //sets nodes visited attribute to true
        n->visited = true;
    }
    
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

//F-Value = G-Value + H-Value
bool compareFValue(const RouteModel::Node* a, const RouteModel::Node* b) {
    return (a->h_value + a->g_value) > (b->h_value + b->g_value);
}

RouteModel::Node *RoutePlanner::NextNode() {
    
    //sorts lists from beginning to end based on F-value
    //Calls compareFValue function
    
    std::sort(this->open_list.begin(), this->open_list.end(), compareFValue);
    
    //references last element in list
    RouteModel::Node* lowest_f_val_node = this->open_list.back();
      //std::cout<<"lowest_f_value= "<<lowest_f_val_node->g_value + lowest_f_val_node->g_value<<std::endl;
    
    //removes last element
    this->open_list.pop_back();
    
    //returns the pointer
    return lowest_f_val_node;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    // 0.0f declares float
    
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // Continues to go through all nodes until starting node is reached
    //while (current_node != start_node)
    while(current_node->parent != nullptr){
        
        //Calculates the distance from the current node to the parent node then adds it to the overall distance
        distance += current_node->distance(*current_node->parent);
        
        //adds the current node to the path
        path_found.push_back(*current_node);
        
        //retrieves information of the current node via arrow operator and makes it the parent and assigns it to the current node
        current_node = current_node->parent;
    }
    
    //adds the start node to path found since loop exited after reaching start node
    path_found.push_back(*current_node);
    
    //reverses path found to list out the start node as the first element and the end node as the last element
    std::reverse(path_found.begin(),path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    
    // Passed argument to parameter current_node
    RouteModel::Node *current_node = nullptr;
    
    // Sets first node to the current node
    current_node = start_node;
    
    // Declares the first node has been visited
    current_node->visited=true;
    
    // Adds the first node to the open_list
    open_list.push_back(start_node);
    
    // Loops nodes till end has been reached
    while(current_node!=end_node){
        
        //returns next node
        current_node=NextNode();
        
        // If the distance from the current_node to end_node is 0
        // end_node has been reached
        if(current_node->distance(*this->end_node)==0){
            
            // Constructs the final path now that the end has been reached
            m_Model.path = ConstructFinalPath(current_node);
            break;
            
        } else {
            
            //Else add neighbor of current node to open_list
            AddNeighbors(current_node);
        }
    }
}

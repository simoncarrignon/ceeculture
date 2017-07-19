/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Simon <simon.carrignon@bsc.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <World.hxx>
#include <vector>
#include <ostream>
#include <map>

namespace Epnet
{
	class Network
	{
		std::vector<std::string> _nodeNames; //list of all node in the network
		std::map< std::string, std::vector< std::string > >  _node2Neighbours; //a map that gives all the neighbours of each node
		std::string _name; 
		std::string _type;
    std::map< std::string, double > _param;
		
		void fullNetwork();
		void freescaleNetwork();
		void smallworldNetwork();
		void randomNetwork();
	public:
		Network();
		Network(std::vector< std::string > nodes);
		Network(std::vector< std::string > nodes,int nnb,std::string name);
		Network(std::vector< std::string > nodes,std::string type,std::string name,std::map<std::string,double> param);//generate preset network of type `type`  
		Network(std::vector< std::string > nodes,std::string type,std::string name,bool fromfile); //read network from a file
		~Network();
		Network& operator=(const Network& other);
		bool operator==(const Network& other) const;
		
		friend std::ostream& operator<<(std::ostream& os, const Epnet::Network& n);
		std::vector<std::string> getNeighboursOf(std::string nodeName);
		
		int write(); // write the network in a file in gephi format
	};
} //namespace Epnet

#endif 

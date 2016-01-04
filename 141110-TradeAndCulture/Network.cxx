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
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Network.hxx>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
namespace Epnet
{
	
	Network::Network()
	{
		_name="";
		_nodeNames= std::vector<std::string>();
		
	}
	
	Network::Network(std::vector< std::string > nodes)
	{
		_nodeNames= std::vector<std::string>(nodes);
		
		for( std::vector<std::string>::iterator it = _nodeNames.begin();it != _nodeNames.end();it++){
			_node2Neighbours.insert(std::pair<std::string,std::vector<std::string>>(*it,_nodeNames));
		}
		
	}
	
	Network::Network(std::vector< std::string > nodes,int nnb,std::string name)
	{

		_name=name;
		_nodeNames= std::vector<std::string>(nodes);
		for( std::vector<std::string>::iterator it = _nodeNames.begin();it != _nodeNames.end();it++){
			std::vector<std::string> listnodes= std::vector<std::string>(nodes);
			std::random_shuffle(listnodes.begin(),listnodes.end());
			std::vector<std::string> neighbours = {};
			while(neighbours.size()<nnb){
				std::string v = listnodes[listnodes.size()-1];
				listnodes.pop_back();
				if(v != *it){
					neighbours.push_back(v);
				}
			}
			_node2Neighbours.insert(std::pair<std::string,std::vector<std::string>>(*it,neighbours));
			
		}
		
		
	}

	
	Network::~Network()
	{
		
	}
	
	Network& Network::operator=(const Network& other)
	{
		
	}
	
	bool Network::operator==(const Network& other) const
	{
		
	}


	std::ostream& operator<<(std::ostream& os, const Network& n)
	{
		os << "this network has : "<<n._nodeNames.size()<< " nodes"; 
		return(os);
		
	}
	std::vector< std::string > Network::getNeighboursOf(std::string nodeName)
	{
		return _node2Neighbours[nodeName];
		
	}
	
	int Network::write()
	{
		std::string filename="";
		if(_name != ""){
			std::ostringstream ossb;
			ossb << _name << ".gdf";
			filename=ossb.str();
		}
		else
			filename="network.gdf";
		std::ofstream fs(filename); 
		
		if(!fs)
		{
			std::cerr<<"Cannot open the output file."<<std::endl;
			return 1;
		}
		
		std::ostringstream node ;
		///the following lines write in gegphi format
		node<<"nodedef>name VARCHAR\n";
		std::ostringstream edge ;
		edge<<"edgedef>node1 VARCHAR,node2 VARCHAR\n";
		
		for ( std::map< std::string, std::vector< std::string > >::iterator it = _node2Neighbours.begin(); it != _node2Neighbours.end();it++){
			
			node<<it->first<<"\n";
			std::vector< std::string > nb = it->second;
			for(std::vector< std::string >::iterator i = nb.begin() ; i!=nb.end();i++)
				edge<<it->first<<","<<*i<<"\n";
		}
		fs<<node.str();
		fs<<edge.str();
		fs.close();
		return 0;
		
	}


}
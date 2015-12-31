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

#include <vector>
#include <ostream>
#include <map>

namespace Epnet
{
	class Network
	{
		std::vector<std::string> _nodeNames;
		std::map< std::string, std::vector< std::string > >  _node2Neighbours;
	public:
		Network();
		Network(std::vector< std::string > nodes);
		Network(std::vector< std::string > nodes,int nnb);
		~Network();
		Network& operator=(const Network& other);
		bool operator==(const Network& other) const;
		
		friend std::ostream& operator<<(std::ostream& os, const Epnet::Network& n);
		std::vector<std::string> getNeighboursOf(std::string nodeName);
	};
} //namespace Epnet

#endif 
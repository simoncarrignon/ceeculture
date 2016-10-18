##This python code generates networks given a pandora config file. 
#Maybe we should integrate it to the C++ code in the Network Class
#But maybe not

import networkx as nx
from numpy import logspace
from xml.etree import ElementTree as et

def adj_list_to_file(G,file_name):
    f = open(file_name, "w")
    for n in G.nodes():
        f.write(str(n) + ' ')
        for neighbor in G.neighbors(n):
            f.write(str(neighbor) + ' ')
        f.write('\n')

#Parsing of the config file for the experience
#xmldoc = minidom.parse('../config.xml')
xmldoc= et.parse('config.xml').getroot()

#Number of Agents in the simulation
numAgents = xmldoc.find('numAgents')

nA=int(numAgents.attrib["value"])

#Number of goods
numGoods = xmldoc.find('goods')
nG=int(numGoods.attrib['num'])

#Type of network
networkConf = xmldoc.find('network')
typeN=networkConf.attrib['type']

#for s in itemlist:
#        print(s.attributes['name'].value)

# cultural network size
N = nA / nG
print("Generation of "+str(nG)+" "+typeN+" network made of "+ str(N)+" agents." )



# generated graphs for each groups of good
rep = int(nG)
G=""


for i in range(rep):
    if typeN == "sw":
        if(int(networkConf.attrib["nparam"]) != 2 or len(networkConf) != 2):
            print("Small World networks need 2 parameters")
            exit(0)

        v=""
        p=""
        
        for param in networkConf:
            if param.attrib["id"] == "v":
                v=int(param.attrib["value"])
            if param.attrib["id"] == "p":
                p=float(param.attrib["value"])


        if(v == "" or p == ""):
            print("Problem with parameters ids")
            exit(0)
        # (1) Small-world
        # from 1e-4 to 1, k equi-spaced values for the rewiring probabilty p to cover the typical small world spectrum
        
       # k= 5
       # p_i = -4
       # p_f = 0
       # P = logspace(p_i,p_f, (p_f-p_i)*k+1)

        # fixed initial neighbors v. Must be even

        G = nx.connected_watts_strogatz_graph(N,v,p)

    if typeN == "sf":
        # output path
                # (2) Preff. attach - Barabasi-Albert model
                # generated graphs for each set of conditions


                if(int(networkConf.attrib["nparam"]) != 1):
                    print("Scale free network have only 1 parameter")
                    exit(0)

                netParam=networkConf.find('prop0')

                if(netParam.attrib["id"] != "m"):
                    print("warning: are you sure the parameter set is for Scale free Network? ") 

                # m  = number of links created for each new node
                m = int(netParam.attrib["value"])

                G = nx.barabasi_albert_graph(N,m)

    
    path='networks/g'+str(i)+'.txt'
    print(path)
    adj_list_to_file(G.to_undirected(),path)


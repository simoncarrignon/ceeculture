#!/bin/bash
#small scrip to quickly run an expe:
#./runExp expename
#create a folder 
name=$1
mkdir $name
cp config.xml $name
cd $name
for i in {1..50} ;
do
	mkdir exp_$i ; 
	ln -s ../../province exp_$i/; 
	ln -s ../../AnalyseTools/analysis exp_$i/; 
	ln -s ../config.xml exp_$i/; 
	cd exp_$i ; 
	./province ; 
	./analysis ; 
	cd .. ;
done
cd ..

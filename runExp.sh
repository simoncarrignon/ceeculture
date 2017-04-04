#!/bin/bash
for i in {1..100} ;
do
	mkdir exp_$i ; 
	ln -s ../province exp_$i/; 
	ln -s ../AnalyseTools/analysis exp_$i/; 
	ln -s ../config.xml exp_$i; 
	cd exp_$i ; 
	./province ; 
	./analysis ; 
	cd .. ;
done

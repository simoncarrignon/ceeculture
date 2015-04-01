if(require("plyr")){library(plyr)}
if(require("RColorBrewer")){library(RColorBrewer)}
if(require("poweRlaw")){library(poweRlaw)}




#fitness=function(p1,b1){abs(p1-b1)/b1}
fitness=function(p1,b1){1-abs(p1-b1)/sqrt(abs(p1^2-b1^2))}
#fitness=function(p1,b1){tan(p1^2-b1^2)/(b1/p1)}

plotFitness=function(p1,b1){
	print(b1)
	plot(fitness(p1,b1)~p1,ylim=c(-.05,1),xlab="price for x",ylab="score for one step",type="l",main="Score of an Agent \n for one good depending on some needs for this good" )
	abline(v=b1,col="red")
	text(b1,-0.08,paste("need(x)=",b1),srt=90,cex=.95,pos=4,col="red")
}
pointFitness=function(p1,b1){
	print(b1)
	points(fitness(p1,b1)~p1,type="l")
	abline(v=b1,col="red")
	text(b1,-0.08,paste("need(x)=",b1),srt=90,cex=.95,pos=4,col="red")
}


allClass<-function(datas,ngoods,g){
		nAgent=length(unique(datas$agent))
		a=sapply(c(0:nAgent)[0:(nAgent-1)%%(ngoods)==g],function(i){paste("Roman_",i,sep="")})
		res=c()
		for( b in a){
			res=rbind(res,datas[datas$agent == b,])
		}
		return(res)
}


plotEquilibrium=function(){
	    par(mar=c(5.1,6.1,4.1,2.1))
plot(tapply(t1$g0_p/t1$g1_p-t1$g0_n,t1$timeStep,mean)~unique(t1$timeStep),type="l",ylim=c(-.1,1),ylab=expression( frac(good(wanted),good(produce)) - need(wanted)),xlab="Time Step",main="Evolution of the mean ration good(wanted) good(produce)\n !!for one too short run only!!")
     
points(tapply(t1$g2_p/t1$g1_p-t1$g2_n,t1$timeStep,mean)~unique(t1$timeStep),type="l",col=2)
points(tapply(t2$g0_p/t2$g2_p-t2$g0_n,t2$timeStep,mean)~unique(t1$timeStep),type="l",col=3)
points(tapply(t2$g1_p/t2$g2_p-t2$g1_n,t2$timeStep,mean)~unique(t1$timeStep),type="l",col=4)
points(tapply(t0$g1_p/t0$g0_p-t0$g1_n,t0$timeStep,mean)~unique(t1$timeStep),type="l",col=5)
points(tapply(t0$g2_p/t0$g0_p-t0$g2_n,t0$timeStep,mean)~unique(t1$timeStep),type="l",col=6)
}

plotAllEquilibrium=function(datas,...){

	par(mar=c(5.1,6.1,4.1,2.1))
	plot(datas[1,]~as.numeric(colnames(datas)),type="l",ylab=expression( frac(good(wanted),good(produce)) - need(wanted)),xlab="Time Step",main="Evolution of the mean ration good(wanted) good(produce)\n !!for one too short run only!!",...)
	for ( i in 2:(nrow(datas))){
			wRes=paste("g",i,"_p",sep="")	
			wResN=paste("g",i,"_n",sep="")	
			points(datas[i,]~as.numeric(colnames(datas)),type="l",col=i)
		}
}

getMeanRatio<-function(datas,nres,timestep,abs=TRUE){

		res=c()
	for(p in 0:(nres-1)){
		cur=allClass(datas,nres,p)
		pRes=paste("g",p,"_p",sep="")	
		pResN=paste("g",p,"_n",sep="")	

		print(pRes)
		toBind=tapply(cur[,pRes]-1/cur[,pResN],cur$timeStep,mean)
		for ( i in 0:(nres-1)){
			if(i != p){
				wRes=paste("g",i,"_p",sep="")	
				wResN=paste("g",i,"_n",sep="")	
				print(wRes)
				if(abs)
					toBind=tapply(cur[,wRes]-cur[,wResN]*cur[,pResN],cur$timeStep,mean)
				else
					toBind=tapply(cur[,wRes]/cur[,pRes]-cur[,wResN],cur$timeStep,mean)
				res=rbind(res,toBind)
			}
		}
	}
	return(res)
}


plotAllClass=function(datas,ngoods,timeStep,...){
	oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,0)
	plot(oneClass$scores~oneClass$timeStep,col=rgb(0,0,0,20,maxColorValue=255),xlab="Time Step",ylab="Score",...)	

	for( i in 1:(ngoods-1)){
		oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,i)
	points(oneClass$scores~oneClass$timeStep,col=rgb(100,50*(i+1),200-50*i ,50,maxColorValue=255),...)	

	}
}

boxAllClass=function(datas,ngoods,timeStep,...){
	oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,0)
	boxplot(oneClass$scores~oneClass$timeStep,col=1,...)	

	for( i in 1:(ngoods-1)){
	oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,i)
		par(new=T)
	boxplot(oneClass$scores~oneClass$timeStep,col=i+2,...)	

	}
}

#plotOneProdPrice(data,g,ng){
#	prodGood=unique(paste("g",g,"_n"))
#	prodGet=unique(paste("g",g,"_n"))
#}
#
#plotOneOther(){
#}



computeVariantFrequency<-function(allData){
	propOfEach=table(allData)/sum(table(allData)) #this compute for each ressource type the proportion of them we have during all the simulation
	table(propOfEach) #count for all proportion the number of price we will find in those proportion
	#table(propOfEach)[],as.numeric(dimnames(table(propOfEach))[[1]])
}

countVar<-function(data,timestep){
	allcount=table(data,timestep)
	res=c()
	for (i in 0:(length(unique(timestep))-1)){
			res=c(res,length(allcount[allcount[,i]>0,i]))

	}
	return(res)

}


logR<-function(n){
	if(n==1)return(1)
	return(logR(n-1)*2)	
}



createEverything<-function(expeDir,ressource,timeA=0,timeB=0){

	all=c()
	files=list.files(expeDir,pattern="run_*")

	for ( i in files){
			file=	paste(expeDir,i,"/agents.csv",sep="")
			print(file)
			work=read.csv(file,sep=";")
			if(timeB == 0){FreqOfVar=createNormalizedTable(work[,ressource])}
			else{
			FreqOfVar=createNormalizedTable(work[work$timeStep>=timeA & work$timeStep<=timeB ,ressource])
			}
			mu=unique(work$mu)
			toBind=cbind(mu,FreqOfVar)
			all=rbind.fill(all,as.data.frame(toBind))
	}
	return(all)
}
getAllMeanRatio<-function(expeDir,nRess,timeA=0,timeB=0,timestep=1,abs=TRUE){

	all=c()
	files=list.files(expeDir,pattern="run_*")

	for ( i in files){
			file=	paste(expeDir,i,"/agents.csv",sep="")
			print(file)
			work=read.csv(file,sep=";")
			toBind=getMeanRatio(work[work$timeStep %% timestep == 0,],nRess,timestep,abs=abs)
			all=rbind(all,toBind)
	}
	return(all)
}


computeMeanSd<-function(expeDir,nRess,timeA,timeB,numRun){

	all=c()
	allR=c()
	for ( i in 0:(numRun-1)){
		file=	paste(expeDir,"/run_",sprintf("%04d",i),"/agents.csv",sep="")
		print(file)
		work=read.csv(file,sep=";")
		for ( j in 0:(nRess-1)){
			ressource=paste("g",j,"_p",sep="")
			print(ressource)
			toBind=work[work$timeStep>=timeA & work$timeStep<=timeB ,c(ressource,"timeStep")]
			allR=c(allR,ressource)
			toBind=tapply(toBind[,ressource],toBind[,"timeStep"],sd)
			all=rbind(all,toBind)
		}
	}
	return(cbind.data.frame(allR,all))
}




readDir<-function(expeDir,nrun){

	all=c()
	for(i in 0:(nrun-1)){
		file=	paste(expeDir,"/run_",sprintf("%04d",i),"/agents.csv",sep="")
		all=rbind(all,read.csv(file,sep=";"))
	}
	return(all)
}


loadSource=function(){
	source("../AnalyseTools/analysis.R")
}

plotAllRessource<-function(expeDir,ressoureN,nstep,nrun,...){

	toPlot=createEverything(expeDir,"g0_p",0,nstep,nrun)
	plotLogBin(toPlot,...)
	for ( i in 1:(ressoureN-1)){
		ressource=paste("g",i,"_p",sep="")
		toPlot=createEverything(expeDir,ressource,0,nstep,nrun)
		pointsLogBin(toPlot,...)
	}
}

concateAllRessouceMean<-function(expeDir,ressoureN,nstep,nrun,...){

	all=c()
	for ( i in 0:(ressoureN-1)){
		c=c+1
		ressource=paste("g",i,"_p",sep="")
		toBind=c(ressource,makeamean(createEverything(expeDir,ressource,0,nstep,nrun)))
		all=rbind.fill(all,as.data.frame(toBind))
	}
	return(all)
}

createAllBin<-function(expeDir,timeA,timeB,numRun){

	all=c()
	taq=c()
	for ( i in 0:(numRun-1)){
		file=	paste(expeDir,"/run_",sprintf("%04d",i),"/agents.csv",sep="")
		print(file)
		work=read.csv(file,sep=";")
		#print(sort(table(work$g0_p)))
		countOfEachVariants=table(work$g0_p)
		powerMx=as.integer(log2(max(countOfEachVariants)))+1
		breaks=2^(0:powerMx)
		labels=2^(1:powerMx)
		binCOEV=cut(countOfEachVariants,labels=labels,breaks=breaks)
		all=c(all,binCOEV)
	}
	return(all)


}

plotLogBin=function(freq,...){
#	xmax=ncol(freq)
#	ymin=log10(min(freq,na.rm=T))
#ylim=c(10^ymin,1),xlim=c(1,2^xmax)
	plot(makeamean(freq)~ names(makeamean(freq)),log="yx",ylab="prop. of variant",xlab="# of variants",type="o",...)
}
pointsLogBin=function(freq,...){
	points(makeamean(freq)~ names(makeamean(freq)),type="o",...)
}

makeamean=function(al){
	al[al==0]=NA
	al=apply(al,2,mean,na.rm=T)
	return(al)

}

createNormalizedTable<-function(data){
	countOfEachVariants=table(data)
	powerMx=as.integer(log2(max(countOfEachVariants)))+1
	breaks=2^(0:powerMx)
	labels=2^(1:powerMx)
	binCOEV=cut(countOfEachVariants,labels=labels,breaks=breaks)
	countBin=table(binCOEV)
	res=as.data.frame((countBin/sum(countBin))/labels)
	res[res==0]=NA
	toBind=t(res["Freq"])	
	colnames(toBind)=res[,1]
	return(toBind)
}


tableOfaAlpha<-function(expeDir,timeA=0,timeB=0,nRess,modulo=1){

	all=data.frame()
	files=list.files(expeDir,pattern="run_*")

	for ( i in files){
		file=	paste(expeDir,i,"/agents.csv",sep="")
		print(file)
		work=read.csv(file,sep=";")
		for ( j in 0:(nRess-1)){
			ressource=paste("g",j,"_p",sep="")
			print(ressource)
			if(timeB<=timeA){
				toBind=work[,ressource]
			}
			else{
				toBind=work[work$timeStep>=timeA & work$timeStep<=timeB ,ressource]
			}
			alpha=estimateAlpha(table(toBind))
			print(alpha)
			print(unique(work$mu))
			toBind=cbind(unique(work$mu),ressource,alpha)
			all=rbind(all,toBind)
		}

	}
	all$alpha = as.numeric(as.character(all$alpha))
	return(all)
}


estimateAlpha<-function(data){
	mpl=displ$new(data)
	est=estimate_xmin(mpl)
	mpl$setXmin(est)
	return(mpl$pars)
}



plotMu<-function(dataMu){
	xmax=ncol(dataMu)-1
	ymin=log10(min(dataMu,na.rm=T))
	c=1
	le=c()
	cle=c()
	plot(makeamean(dataMu)~ names(makeamean(dataMu)),log="yx",ylim=c(10^ymin,1),xlim=c(1,2^xmax),type="n",xaxt="n",yaxt="n",ylab="prop. of variant",xlab="# of variants")
	for(i in sort(unique(dataMu$mu))){ 
		tc=brewer.pal(length(unique(dataMu$mu)),"RdYlBu")[c]
		#tc=heat.colors(length(unique(dataMu$mu)))[c]
		points(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])~ names(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])),type="o",col=tc)
		le=c(le,i)
		cle=c(cle,tc)
		c=c+1
	}
	legend(2^(xmax-3),1,legend=le,col=cle,lty=1)


	ticks <- seq(0, xmax, by=1)
	at <- sapply(ticks, function(i) {10^i})
	labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
	axis(1,at=at,label=labels)

	ticks <- seq(-1, ymin, by=-1)

	at <- sapply(ticks, function(i) {10^i})
	at = c(1,0,at)
	print(at)
	labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
	labels=c(1,0,labels)
	print(labels)
	axis(2,at=at,label=labels)
	title(expression(paste("Freq. Of Var. and ",mu)))
}
pointsMu<-function(dataMu){
	xmax=ncol(dataMu)-1
	ymin=log10(min(dataMu,na.rm=T))
	c=1
	le=c()
	cle=c()
	for(i in sort(unique(dataMu$mu))){ 
		tc=brewer.pal(length(unique(dataMu$mu)),"RdYlBu")[c]
		#tc=heat.colors(length(unique(dataMu$mu)))[c]
		points(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])~ names(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])),type="o",col=tc)
		le=c(le,i)
		cle=c(cle,tc)
		c=c+1
	}
}


plotVarientFrequency<-function(propOfEach,...){
	plot(propOfEach[]/as.numeric(dimnames(propOfEach)[[1]]),as.numeric(dimnames(propOfEach)[[1]]),log="yx",ylim=c(0.0001,1),xlim=c(1,10000),xaxt="n",yaxt="n",...)

	ticks <- seq(0, 8, by=1)
	at <- sapply(ticks, function(i) {10^i})
	labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
	axis(1,at=at,label=labels)

	ticks <- seq(-1, -4, by=-1)

	at <- sapply(ticks, function(i) {10^i})
	at = c(1,0,at)
	print(at)
	labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
	labels=c(1,0,labels)
	print(labels)
	axis(2,at=at,label=labels)



}
#plot4Distrib=function(){
#	plotLogBin(createNormalizedTable(lexp2$g0_p),main="Variants distribution for the 4 setup\n !!ONE RUN!!",col=2)
#	pointsLogBin(createNormalizedTable(lexp3$g0_p),col=3)
#	pointsLogBin(createNormalizedTable(lexp4$g0_p),col=4)
#	pointsLogBin(createNormalizedTable(lexp5$g0_p),col=5)
#	legend("topright",legend=c("A","B","C","D"),fill=c(2,4,3,5))
#
#}
#
#{
#
#	pdf("frequenciesABCD.pdf")
#plotLogBin(exp2_2,col="lightblue")
#pointsLogBin(exp4,col="blue")
#pointsLogBin(exp3,col="lightgreen")
#pointsLogBin(exp5A,col="green")
#	legend("topright",legend=c("A","B","C","D"),fill=c("lightblue","blue","lightgreen","green"))
#dev.off()
#
#	pdf("A-B.pdf")
#plotLogBin(exp2_2,col="lightblue")
#pointsLogBin(exp4,col="blue")
#dev.off()
#	pdf("A-C.pdf")
#plotLogBin(exp2_2,col="lightblue")
#pointsLogBin(exp3,col="lightgreen")
#dev.off()
#	pdf("A-D.pdf")
#plotLogBin(exp2_2,col="lightblue")
#pointsLogBin(exp5A,col="green")
#dev.off()
#	pdf("B-C.pdf")
#plotLogBin(exp4,col="blue")
#pointsLogBin(exp3,col="lightgreen")
#dev.off()
#	pdf("B-D.pdf")
#plotLogBin(exp4,col="blue")
#pointsLogBin(exp5A,col="green")
#dev.off()
#	pdf("C-D.pdf")
#plotLogBin(exp3,col="lightgreen")
#pointsLogBin(exp5A,col="green")
#dev.off()
#
#
#
#}

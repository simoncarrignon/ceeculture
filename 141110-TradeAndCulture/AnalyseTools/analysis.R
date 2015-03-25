library(plyr)



fitness=function(p1,b1){abs(p1-b1)/b1}

plotFitness=function(p1,b1){
	print(b1)
	plot(fitness(p1,b1)~p1)
	abline(v=b1,col="red")
}


allClass<-function(datas,ngoods,g){
		nAgent=length(unique(datas$agent))
		print(nAgent)
		a=sapply(c(0:nAgent)[0:(nAgent-1)%%(ngoods)==g],function(i){paste("Roman_",i,sep="")})
		res=c()
		for( b in a){
			print(b)
			res=rbind(res,datas[datas$agent == b,])
		}


		return(res)
	
}




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



createEverything<-function(expeDir,ressource,timeA,timeB,numRun){

	all=c()
	for ( i in 0:(numRun-1)){
		file=	paste(expeDir,"/run_",sprintf("%04d",i),"/agents.csv",sep="")
		print(file)
		work=read.csv(file,sep=";")
		FreqOfVar=createNormalizedTable(work[work$timeStep>=timeA & work$timeStep<=timeB ,ressource])
		mu=unique(work$mu)
		toBind=cbind(mu,FreqOfVar)
		all=rbind.fill(all,as.data.frame(toBind))
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
	source("./analysis.R")
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


tableOfaAlpha<-function(expeDir,timeA,timeB,numRun,nRess){

	all=data.frame()
	for ( i in 0:(numRun-1)){
		file=	paste(expeDir,"/run_",sprintf("%04d",i),"/agents.csv",sep="")
		print(file)
		work=read.csv(file,sep=";")
		for ( j in 0:(nRess-1)){
			ressource=paste("g",j,"_p",sep="")
			print(ressource)
			toBind=work[work$timeStep>=timeA & work$timeStep<=timeB ,ressource]
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
	for(i in unique(dataMu$mu)){ 
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

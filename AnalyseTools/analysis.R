if(require("plyr")){library(plyr)}
if(require("vioplot")){library(vioplot)}
if(require("RColorBrewer")){library(RColorBrewer)}
if(require("poweRlaw")){library(poweRlaw)}
if(require("XML")){library(XML)}




fitness<-function(p1,b1){1-abs(p1-b1)/sqrt(abs(p1^2-b1^2))}
mariofit<-function(p1,b1=.5){
    if(p1>=b1)
	return(0)
    else
	return(1-(1/b1*p1))
}
#fitness=function(p1,b1){tan(p1^2-b1^2)/(b1/p1)}
#fitness=function(p1,b1){abs(p1-b1)/b1}


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
    text(b1,-0.09,paste("n(g)=",b1,sep=""),srt=90,cex=1.2,pos=4,col="red")
}

allCPhDlass<-function(datas,ngoods,g){
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
    plot(tapply(t1$g0_p/t1$g1_p-t1$g0_n,t1$timeStep,mean)~unique(t1$timeStep),type="l",ylim=c(-.1,1))

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

#This function return the difference between the price of each ressources and the optimal price for those ressources (rember that the optimal price is not the same depending on if the ressource is produced or not by the agents.
getMeanRatio<-function(datas,nres,timestep,timeA=0,timeB=0,abs=TRUE){
    res=c()
    datas=datas[datas$timeStep %% timestep == 0,]

    if(timeB>timeA)
	datas=datas[datas$timeStep >timeA & datas$timeStep < timeB ,]
    for(p in 0:(nres-1)){
	cur=allClass(datas,nres,p)
	pRes=paste("g",p,"_p",sep="")	
	pResN=paste("g",p,"_n",sep="")	
	print(pRes)
	for ( i in 0:(nres-1)){
	    #### Following the old ratio
	    # if(i != p){
	    #     wRes=paste("g",i,"_p",sep="")	
	    #     wResN=paste("g",i,"_n",sep="")	
	    #     print(wRes)
	    #     if(abs)
	    #         toBind=tapply(cur[,wRes]-cur[,wResN]*cur[,pResN],cur$timeStep,mean)
	    #     else
	    #         toBind=tapply(cur[,wRes]/cur[,pRes]-cur[,wResN],cur$timeStep,mean)
	    #     res=rbind(res,toBind)
	    # }
	    # else{
	    #     toBind=tapply(cur[,pRes]-cur[,pResN],cur$timeStep,mean)
	    #     res=rbind(res,toBind)

	    # }
	    if(i != p){
		#     wRes=paste("g",i,"_p",sep="")	
		#     wResN=paste("g",i,"_n",sep="")	
		#     print(wRes)
		#     toBind=tapply(cur[,wRes]-cur[,wResN],cur$timeStep,mean)
		#     res=rbind(res,toBind)
	    }
	    else{
		ni=c()
		for ( o in 0:(nres-1)){
		    if(o != p){
			wResN=paste("g",o,"_n",sep="")	
			ni=c(ni,unique(cur[,wResN]))
		    }
		}

		nP=unique(cur[,pResN])
		opt=prodOpt(nP,ni)
		print(opt)
		print(nP)
		toBind=tapply(cur[,pRes]-opt,cur$timeStep,mean)
		res=rbind(res,toBind)
	    }
	}
    }
    return(res)
}


getMeanRatio2<-function(datas,timestep=1,timeA=0,timeB=0){

    res=c()
    datas=datas[datas$timeStep %% timestep == 0,]

    if(timeB>timeA)
	datas=datas[datas$timeStep >timeA & datas$timeStep < timeB ,]
    for(p in levels(datas$p_good)){
	cur=datas[datas$p_good == p,]
	pRes=paste(p,"_p",sep="")	
	pResN=paste(p,"_n",sep="")	
	print(pRes)
	for(i in levels(cur$p_good)){
	    if(i != p){
		wRes=paste(i,"_p",sep="")	
		wResN=paste(i,"_n",sep="")	
		print(wRes)
		toBind=tapply(cur[,wRes]-cur[,wResN],cur$timeStep,mean)
		res=rbind(res,toBind)
	    }
	    #else{
	    #    ni=c()
	    #    for ( o in 0:(nres-1)){
	    #        if(o != p){
	    #    	wResN=paste("g",o,"_n",sep="")	
	    #    	ni=c(ni,unique(cur[,wResN]))
	    #        }
	    #    }

	    #    nP=unique(cur[,pResN])
	    #    opt=prodOpt(nP,ni)
	    #    print(opt)
	    #    print(nP)
	    #    toBind=tapply(cur[,pRes]-opt,cur$timeStep,mean)
	    #    res=rbind(res,toBind)
	    #}
	}
    }
    return(res)
}



##This function plot mean score for each producer group during time
plotAllClassMean=function(datas,timeStep=1,...){

    allgoods=levels(datas$p_good)
    ngoods=length(allgoods)
    j=0
    plot(c(0,max(datas$timeStep)),c(1,1),ylim=c(0,ngoods*10),type="n")
    for( i in allgoods){
	oneClass=datas[datas$timeStep %% timeStep == 0 & datas$p_good == i,]
	avg=	tapply(10*ngoods-oneClass$scores,oneClass$timeStep,mean)
	sdev=	tapply(10*ngoods-oneClass$scores,oneClass$timeStep,sd)
	points(avg~names(avg),ylim=c(20,40),type="o",pch=j,col=j+1)
	x=as.numeric(names(avg))
	arrows(x, avg-sdev, x, avg+sdev, length=0.01, angle=90, code=3,col=j+1)
	j=j+1

    }
    legend("bottomleft",legend=allgoods,col=1:ngoods,lty=1)
}

pointsAllClass=function(datas,timeStep=1,...){

    allgoods=levels(datas$p_good)
    ngoods=length(allgoods)
    j=0
    plot(c(0,max(datas$timeStep)),c(1,1),ylim=c(0,ngoods*10),type="n")
    for( i in allgoods){
	oneClass=datas[datas$timeStep %% timeStep == 0 & datas$p_good == i,]
	points(10*ngoods-oneClass$score~oneClass$timeStep,ylim=c(20,40),col=j+1)
	j=j+1

    }
    legend("bottomleft",legend=allgoods,col=1:ngoods,lty=1)
}

boxAllClass=function(datas,ngoods,timeStep,...){
    oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,0)
    boxplot(30-oneClass$scores~oneClass$timeStep,col=1,...)	

    for( i in 1:(ngoods-1)){
	oneClass=allClass(datas[datas$timeStep %% timeStep == 0,],ngoods,i)
	par(new=T)
	boxplot(30-oneClass$scores~oneClass$timeStep,col=i+2,...)	

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



createEverything<-function(expeDir,timeA=0,timeB=0){

    all=c()
    files=list.files(expeDir,pattern="run_*")

    for ( i in files){
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	work=read.csv(file,sep=";")
	colnames(work)[c(1,2)]=c("nAg","nGood")

	ressource = work$nGood[1]
	print(ressource)
	if(ressource == 1){
	    if(timeB == 0){FreqOfVar=createNormalizedTable(work[,"g0_p"])}
	    else{
		FreqOfVar=createNormalizedTable(work[work$timeStep>=timeA & work$timeStep<=timeB ,ressource])
	    }
	    mu=unique(work$mu)
	    toBind=cbind(mu,FreqOfVar)
	    all=rbind.fill(all,as.data.frame(toBind))
	}
	else{
	    for( i in 1:(ressource-1)){
		oneClass=allClass(work[work$timeStep %% 1 == 0,],ressource,i)
		for( i in 1:(ressource-1)){
		    res=paste("g",i,"_p",sep="")
		    if(timeB == 0){FreqOfVar=createNormalizedTable(work[,res])}
		    else{
			FreqOfVar=createNormalizedTable(work[work$timeStep>=timeA & work$timeStep<=timeB ,res])
		    }
		    mu=unique(work$mu)
		    toBind=cbind(mu,ressource,FreqOfVar)
		    all=rbind.fill(all,as.data.frame(toBind))
		}
	    }
	}
    }
    return(all)
}

#Return a table with all ratio mean for each file of the dir expedir
getAllMeanRatio<-function(expeDir,timeA=0,timeB=0,timestep=1,maxfolder=10000){

    all=data.frame()
    files=list.files(expeDir,pattern="run_*")
    sim=0

    for ( i in files[1:min(c(length(files),maxfolder))]){
	folder=	paste(expeDir,i,sep="")
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	work=read.csv(file,sep=";")
	toBind=getMeanRatio2(work,timestep,timeA=timeA,timeB=timeB)
	p=getPropFromXml(folder,"network","p")
	v=getPropFromXml(folder,"network","v")
	network=paste("networks/",sim,"_",colnames(toBind),".gdf",sep= "")
	all=rbind(all,cbind(t(toBind),p,v,network))
	sim=sim+1
    }
    return(all)
}

getAllMeanScore<-function(expeDir,timeA=0,timeB=0,timestep=1,maxfolder=10000,listOfXmlValue=c(),fun=mean){

    all=data.frame()
    files=list.files(expeDir,pattern="run_*")
    sim=0

    for ( i in files[1:min(c(length(files),maxfolder))]){
	folder=	paste(expeDir,i,sep="")
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	work=read.csv(file,sep=";")
	work=work[work$timeStep %% timestep == 0,]
	toBind=tapply(work$scores,work[,c("timeStep","p_good")],fun)

	#	m=getPropFromXml(folder,"network","m")
	#v=getPropFromXml(folder,"network","v")
	#network=paste("networks/",sim,"_",colnames(toBind),".gdf",sep= "")
	all=rbind(all,cbind(t(toBind)))#,m,network))
	sim=sim+1
    }
    return(all)
}

getAllFinalCharac<-function(expeDir,nRess,timeA=0,timeB=0,timestep=1){

    all=data.frame()
    files=list.files(expeDir,pattern="run_*")

    rname=c()
    for ( i in files){
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	rname=c(rname,i)
	work=read.csv(file,sep=";")
	maxt=max(work$timeStep)
	tBind=mean(work$score[work$timeStep == maxt])
	tbName="score_mean"
	for ( j in 0:(nRess-1)){
	    ressource=paste("g",j,"_n",sep="")
	    tBind=cbind(tBind,unique(work[,ressource]))
	    tbName=c(tbName,ressource)
	}
	all=rbind(all,tBind)
    }
    colnames(all)=tbName
    rownames(all)=rname
    return(all)
}


getFinalRatioVsVar<-function(expeDir,Var){

    all=c()
    files=list.files(expeDir,pattern="run_*")

    for ( i in files){
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	work=read.csv(file,sep=";")
	colnames(work)[c(1,2)]=c("nAg","nGood")
	print(unique(work$nGood))
	toBind=getMeanRatio(work[work$timeStep == max(work$timeStep),],unique(work$nGood),timestep=1)
	orow=cbind(toBind,unique(work[,Var]))
	all=rbind(all,orow)
    }
    return(all)
}

getFinalMeanScore<-function(expeDir,Var,fun){
    all=c()
    files=list.files(expeDir,pattern="run_*")

    for ( i in files){
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	colnames(work)[c(1,2)]=c("nAg","nGood")
	all=rbind(all,fun(work$sc))
    }
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
    plotLogBin(toPlot,ylab="proportion of variant",xlab="number of variants",...)
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

    xmax=ncol(freq)-1
    ymin=log10(min(freq,na.rm=T))-1
    plot(makeamean(freq)~ names(makeamean(freq)),log="yx",ylab="proportion of variant",xlab="number of variants",type="b",xaxt="n",yaxt="n",ylim=c(10^ymin,1),...)


    ticks <- seq(0, xmax, by=1)
    at <- sapply(ticks, function(i) {10^i})
    labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
    axis(1,at=at,label=labels)

    ticks <- seq(-1, ymin, by=-2)

    at <- sapply(ticks, function(i) {10^i})
    at = c(1,0,at)
    labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
    labels=c(1,0,labels)
    axis(2,at=at,label=labels)
}
pointsLogBin=function(freq,...){
    points(makeamean(freq)~ names(makeamean(freq)),type="b",...)
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

estimateAlphaC<-function(data){
    mpl=conpl$new(data)
    est=estimate_xmin(mpl)
    mpl$setXmin(est)
    return(mpl)
}


plotMu<-function(dataMu){
    xmax=ncol(dataMu)-1
    ymin=log10(min(dataMu,na.rm=T))-1
    c=1
    le=c()
    ple=c()
    cle=c()
    pch=c(0,1,2)
    plot(makeamean(dataMu)~ names(makeamean(dataMu)),log="yx",ylim=c(10^ymin,1),xlim=c(1,2^xmax),type="n",xaxt="n",yaxt="n",ylab="proportion of variant",xlab="number of variants")
    for(i in sort(unique(dataMu$mu))){ 
	#tc=brewer.pal(length(unique(dataMu$mu)),"RdYlBu")[c]
	#tc=heat.colors(length(unique(dataMu$mu)))[c]
	tc=1
	pc=pch[c]
	points(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])~ names(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])),type="b",col=tc,pch=pc)
	le=c(le,i)
	cle=c(cle,tc)
	ple=c(ple,pc)
	c=c+1
    }
    #legend(2^(xmax-3),1,legend=le,col=cle,lty=1,pch=ple)
    legend("topright",legend=le,col=cle,pch=ple)


    ticks <- seq(0, xmax, by=1)
    at <- sapply(ticks, function(i) {10^i})
    labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
    axis(1,at=at,label=labels)

    ticks <- seq(-1, ymin, by=-2)

    at <- sapply(ticks, function(i) {10^i})
    at = c(1,0,at)
    print(at)
    labels <- sapply(ticks, function(i) as.expression(bquote(10^ .(i))))
    labels=c(1,0,labels)
    print(labels)
    axis(2,at=at,label=labels)
    #title( expression(paste("Frequencies distribution for differents ",mu,sep="")))
}
pointsMu<-function(dataMu){
    xmax=ncol(dataMu)-1
    ymin=log10(min(dataMu,na.rm=T))
    c=1
    le=c()
    cle=c()
    for(i in sort(unique(dataMu$mu))){ 
	#tc=brewer.pal(length(unique(dataMu$mu)),"RdYlBu")[c]
	tc=heat.colors(length(unique(dataMu$mu)))[c]
	points(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])~ names(makeamean(dataMu[dataMu$mu == i,2:ncol(dataMu)])),type="b",col=tc)
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
#
#
#pdf("scatterPlotGood1.pdf")
# ggplot(data.frame(40-g0$scores,g0$timeStep),aes(g0$timeStep,40-g0$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 1",ylab="score")
#dev.off()                                                       
#pdf("scatterPlotGood2.pdf")                                     
# ggplot(data.frame(40-g1$scores,g1$timeStep),aes(g1$timeStep,40-g1$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 2",ylab="score")
#dev.off()                                                       
#pdf("scatterPlotGood3.pdf")                                     
# ggplot(data.frame(40-g2$scores,g2$timeStep),aes(g2$timeStep,40-g2$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 3",ylab="score")
#dev.off()                                                       
#pdf("scatterPlotGood4.pdf")                                     
# ggplot(data.frame(40-g3$scores,g3$timeStep),aes(g3$timeStep,40-g3$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 4",ylab="score")
#dev.off()

#pdf("ScoreMeanForEachAgetnsType.pdf")




#dev.off()                                                       
#pdf("scatterPlotGood2.pdf")                                     
# ggplot(data.frame(40-g1$scores,g1$timeStep),aes(g1$timeStep,40-g1$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 2",ylab="score")
#dev.off()                                                       
#pdf("scatterPlotGood3.pdf")                                     
# ggplot(data.frame(40-g2$scores,g2$timeStep),aes(g2$timeStep,40-g2$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 3",ylab="score")
#dev.off()                                                       
#pdf("scatterPlotGood4.pdf")                                     
# ggplot(data.frame(40-g3$scores,g3$timeStep),aes(g3$timeStep,40-g3$scores)) +  geom_point() + geom_smooth() + labs(title="Evolution of the score of produceur of good 4",ylab="score")
#dev.off()

pritnAllGRaph<-function(){
    c=1
    le=c()
    ple=c()
    cle=c()
    for( i in unique(da$site)){
	subP=da[da$site == i,]
	png(paste(i,".png",sep=""))
	plotLogBin(createNormalizedTable(table(subP$typology)),main=i)
	dev.off()
	c=c+1
	le=c(le,i)
	cle=c(cle,c)
    }

}

plotInteractionMeanSd=function(datas,...){

    datasB=datas[datas$nAg == 200,]

    avg=tapply(datasB[,"30000"],datasB$nGood,mean)
    sdev=tapply(datasB[,"30000"],datasB$nGood,sd)
    plot(avg~names(avg),type="o",pch=1,col=1,ylim=c(-.5,.5))
    x=as.numeric(names(avg))
    arrows(x, avg-sdev, x, avg+sdev, length=0.01, angle=90, code=3,col=1)
    for( i in unique(datas$nAg)){
	datasB=datas[datas$nAg == i,]
	avg=tapply(datasB[,"30000"],datasB$nGood,mean)
	sdev=tapply(datasB[,"30000"],datasB$nGood,sd)
	points(avg~names(avg),ylim=c(20,40),type="o",pch=i,col=i/100)
	x=as.numeric(names(avg))
	arrows(x, avg-sdev, x, avg+sdev, length=0.01, angle=90, code=3,col=i/100)
    }
}

main <- function(){


    setwd("~/result/")


    nratc1=getFinalRatioVsVar("testGoodC1/",c("nAg","nGood"))   
    nratc2=getFinalRatioVsVar("testGoodC2/",c("nAg","nGood"))   
    nratc3=getFinalRatioVsVar("testGoodC3/",c("nAg","nGood"))   
    nratc4=getFinalRatioVsVar("testGoodC4/",c("nAg","nGood"))   
    nratd3=getFinalRatioVsVar("testGoodD3/",c("nAg","nGood"))   
    nratd4=getFinalRatioVsVar("testGoodD4/",c("nAg","nGood"))   
    nrat1000a=getFinalRatioVsVar("testGood1000ag1/",c("nAg","nGood"))   
    nrat1000b=getFinalRatioVsVar("testGood1000ag2/",c("nAg","nGood"))   

    nratc=rbind(nratc1,nratc2,nratc3,nratc4)
    nrata=rbind(nrata1,nrata2,nrata3,nrata4)
    nratd=rbind(nratd1,nratd2,nratd3,nratd4)
    nratb=rbind(nratb1,nratb2,nratb3,nratb4)

    t=nratc[nratc$nGood == 10 ,c("30000","nAg")]

    nrat=rbind(nrata,nratb,nratc,nratd,nrat1000b,nrat1000a)
    nratR=cbind(nrat,nrat$nAg/nrat$nGood)


    boxplot(nrat[,"30000"] ~ nrat$nAg * nrat$nGood,las=3)

    aov(nrat[,"30000"] ~ nrat$nAg + nrat$nGoodo)
    plot(nratR[,"30000"] ~ nratR[,4],col=nratR$nGood)


    dev.off()
    interaction.plot( nrat$nGood,nrat$nAg,nrat[,"30000"],fun="sd",col=heat.colors(5))     

    t=createAllBin("testGoodD2",0,300000,50)


    table(nrat[,c("nGood","nAg")])

    plot( nrat$nGood[ nrat$nAg == 200]+.5,nrat[ nrat$nAg == 200 ,"30000"] ,ylim=c(-2,2),type="n")
    p=0
    for (i in unique(nrat$nAg)){
	points( nrat$nGood[ nrat$nAg == i]+p/10,nrat[ nrat$nAg == i ,"30000"], col=heat.colors(length(unique(nrat$nAg)),alpha=.1)[p+1],pch=20)
	p=p+1
    }

    A=createEverything("~/result/testGoodOnRepA/",timeA=0,timeB=0)
    A=createEverything("~/result/testGoodOnRepB/",timeA=0,timeB=0)

    a=read.csv("../agents.csv",sep=";")
    dev.off()
    boxplot(a$scores ~a$timeStep)

    rand=getAllMeanRatio("~/randnet//",timestep=50)
    newrat12=getAllMeanRatio("~/newcondition12/",3,timestep=50)
    longRat=getAllMeanRatio("~/long/",3)

    newratlast=getAllMeanRatio("~/newconditionlast/",3,timestep=50)
    newrat=getAllMeanRatio("~/newcondition/",3,timestep=50)
    colname=read.csv("~/newcondition1/run_0000/agents.csv",sep=";",nrows=1)
    a=c()
    expeDir="~/newcondition1/"
    for(i in list.files(expeDir,pattern="run_*")){
	print(i)
	temp=read.csv(paste(expeDir,i,"/agents.csv",sep=""),sep=";")
	a=rbind(a,temp[temp$timeStep %% 100 ==0,])
    }
    pdf("boxScoreTime.pdf")
    boxplot((30-a$scores)~a$timeStep,outline=F,ylim=c(0,30))
    dev.off()
    pdf("meanEachGroup.pdf")
    plotAllClassMean(a,3,100)
    dev.off()
    rat=getAllMeanRatio(expeDir,3,timestep=100)
    pdf("meanRatio.pdf")
    boxplot(rat,outline=F)
    dev.off()


    b=getAllMeanScore("~/long/",timestep=1)
    bf=getAllFinalCharac("~/long/",3)
    l=cbind(bf,apply(bf[,2:ncol(bf)],1, rapMin))
    plot(bf$score_mean ~ apply(bf[,2:ncol(bf)],1, rapMin))

    apply(a[,2:ncol(a)],1,function(x) sum(x)/prod(x))

    ex=ex[ex$timeStep %% 1000 == 0,]

    g0=allClass(ex,3,0)
    ni0=c(unique(g0$g1_n),unique(g0$g2_n))
    n0=unique(g0$g0_n)
    opt0=prodOpt(n0,ni0)
    plot((g0$g0_p-opt0)~g0$timeStep,ylim=c(-1,1))

    g1=allClass(ex,3,1)
    ni1=c(unique(g1$g0_n),unique(g1$g2_n))
    n1=unique(g1$g1_n)
    opt1=prodOpt(n1,ni1)
    points((g1$g1_p-opt1)~g1$timeStep)

    g2=allClass(ex,3,2)
    ni2=c(unique(g2$g0_n),unique(g2$g1_n))
    n2=unique(g2$g2_n)
    opt2=prodOpt(n2,ni2)
    points((g2$g1_p-opt2)~g2$timeStep)

    plot(tapply((30-ex$score),ex$timeStep,mean)~as.numeric(unique(ex$timeStep)),type="l",col="red")
    rex=getMeanRatio(ex,3,1)
    boxplot(rex)

    boxplot(ex$score ~ex$timeStep)
    par(new=T)
    plotAllClassMean(ex,3,1)

}
rapMin <- function(x){
    a=vapply(seq_along(x),function(i)x[i]/sum(x[-1]),numeric(1))
    min(a)/max(a)
}
prodOpt <- function(np,ni){
    return(sum(ni*ni)/(length(ni)*np))
}

getSumPrice<-function(data,nRess){

    res=0
    for ( j in 0:(nRess-1)){
	ressource=paste("g",j,"_n",sep="")
	res=res+unique(data[,ressource])
    }
    return (res)
}


getAllSumPrice<-function(folder,nRess){
    res=c()
    allF =list.files(folder,pattern="run_*")
    for (i in allF){
	res=c(res,getSumPrice(read.csv(paste(folder,i,"/agents.csv",sep=""),sep=";"),nRess))
    }
    return (res)
}


getColors=function(datas){
    cgood=1:length(levels(datas$p_good))
    names(cgood)=levels(datas$p_good)
    return(cgood)
}

#plot(c$g0_q ~ c$timeStep, col=cg[c$p_good])



#legend("bottomright",legend=names(cg),col=cg,lty=1)

sumfact=function(datas){
    res=0
    for ( i in levels(datas$p_good)){
	res= res+ unique(datas[,paste(i,"_n",sep="")])
    }
    return(res)
}

#c(unique(c$g0_n),unique(c$g1_n),unique(c$g2_n))
#c(mean(c$g0_p),mean(c$g1_p),mean(c$g2_p))


############################################################
##XML tools

xmltest<-function(){
    i=xmlTreeParse("~/randnet/run_0000/config.xml")
    i=xmlRoot(i)
    net=i[["network"]]
    paramId=c()
    paramValue=c()
    print(xmlAttrs(net))
    print(xmlElementsByTagName(net,"prop0"))
    print(xmlChildren(net))
    #    for( u in 1:xmlSize(net)){
    #	attri=xmlAttrs(net[[u]]);
    #	attri[["id"]]
    #	paramValue=c(paramValue,as.numeric(attri[["value"]]))
    #	#paramName=c(param,attri[0
    #    }
    #    print(paramValue)
}


    plotDerive=function(setnet,...){

	plot(apply(setnet[[1]],2,mean),type="n",...)
	print(seq_along(setnet))
	sapply(seq_along(setnet),function(x){
	       u=apply(setnet[[x]],2,function(l)l[1:(length(l)-1)]-l[2:length(l)])
	       points(apply(u,2,mean),type="l",col=x)
	       #   arrows(x0=seq_along(apply(u,2,mean)),
	       #	  y0=apply(u,2,mean)-apply(u,2,sd),
	       #	  x1=seq_along(apply(u,2,mean)),
	       #	  y1=apply(u,2,mean)+apply(u,2,sd),
	       #	  ylim=c(0,30),col=x,lty=1,code=3,angle=90,length=.01)
	       } )
	legend("topleft",legend=names(setnet),col=seq_along(setnet),lty=1)

    }

    createTable <- function(seqnet){
	res=data.frame()
	for( x in seq_along(seqnet)){
	    tmp=	cbind.data.frame(     
					 names(seqnet[[x]]),
					 apply(seqnet[[x]],2,mean),
					 apply(seqnet[[x]],2,sd),
					 names(seqnet)[x])
	    colnames(tmp)=c("timestep","mean","sd","net")
	    res= rbind.data.frame(res,tmp)
	}
	return(res)
    }


	   plotDeriveOfMean=function(setnet,...){

	       plot(apply(setnet[[1]],2,mean),type="n",...)
	       sapply(seq_along(setnet),function(x){
		      l=apply(setnet[[x]],2,mean)
		      u=(l[1:(length(l)-100)]-l[100:length(l)])/100
		      print(rbind(l,u))

		      points(u,type="l",col=x)
})
	       legend("topleft",legend=names(setnet),col=seq_along(setnet),lty=1)

	   }
	   #This function use a folder with set of network to create a list... baah is complicated
	   makeListWithAllFolder=function(homrep){
	       allG=c()
	       for(g in list.files(homrep)){
		   print(paste(homrep,g,sep=""))
		   allG[[g]] = getAllMeanScore(paste(homrep,g,"/",sep=""))
		   allG[[g]] = allG[[g]][complete.cases(allG[[g]]),]
	       }
	       return(allG);
	   }

	   plotMeanSd=function(setnet){

	       plot(apply(setnet[[1]],2,mean),col=1,type="n")
	       sapply(seq_along(setnet),function(x){
		      print(x)
		      points(apply(setnet[[x]],2,mean),col=x,type="l")
		      arrows(x0=seq_along(apply(setnet[[x]],2,mean)),
			     y0=apply(setnet[[x]],2,mean)-apply(setnet[[x]],2,sd),
			     x1=seq_along(apply(setnet[[x]],2,mean)),
			     y1=apply(setnet[[x]],2,mean)+apply(setnet[[x]],2,sd),
			     ylim=c(0,30),col=x,lty=1,code=3,angle=90,length=.01)
} )
	       legend("topright",legend=names(setnet),col=seq_along(setnet),lty=1)
	   }

	   plotFun=function(setnet,fun=mean,...){

	       plot(apply(setnet[[1]],2,fun),col=1,type="n",...)
	       sapply(seq_along(setnet),function(x){
		      print(x)
		      points(apply(setnet[[x]],2,fun),col=x,type="l")
} )
	       legend("topright",legend=names(setnet),col=seq_along(setnet),lty=1)
	   }


test<-function(){
    uSF=read.csv("../agents.csv",sep=";")
    boxplot(40 - uSF$scores ~ uSF$timeStep,ylim=c(0,40),outline=F)    
    plotAllClassMean(uSF) 

    uSW=read.csv("../agents.csv",sep=";")
    boxplot(30 - uSW$scores ~ uSW$timeStep,ylim=c(0,30),outline=F)    
    plotAllClassMean(uSW) 
    ####G1-4
    resG1=getAllMeanScore("~/result/6Net_test/g1/")
    resG2=getAllMeanScore("~/result/6Net_test/g2/")
    resG3=getAllMeanScore("~/result/6Net_test/g3/")
    resG4=getAllMeanScore("~/result/6Net_test/g4/")
    resG5=getAllMeanScore("~/result/6Net_test/g5/")
    resG6=getAllMeanScore("~/result/6Net_test/g6/")
    allG=rbind(
	       cbind(resG1,net="G1"),
	       cbind(resG1,net="G2"),
	       cbind(resG1,net="G3"),
	       cbind(resG1,net="G4"))
    allG=rbind(
	       cbind(timestep=colnames(resG1),mean=apply(resG1,2,mean),sd=apply(resG1,2,sd),net="G1"),
	       cbind(timestep=colnames(resG2),mean=apply(resG2,2,mean),sd=apply(resG2,2,sd),net="G2"),
	       cbind(timestep=colnames(resG3),mean=apply(resG3,2,mean),sd=apply(resG3,2,sd),net="G3"),
	       cbind(timestep=colnames(resG4),mean=apply(resG4,2,mean),sd=apply(resG4,2,sd),net="G4"))

    interaction.plot( allG$net,allG$mean,allG$timestep) 
    allG=data.frame(allG)
    homrep="~/result/4Net_test/"
    homrep="~/result/6Net_test/"
    allrep=sapply(list.files(homrep),function(x) paste(homrep,x,sep=""))
    dataFiles6=sapply(allrep,function(x) paste(x,list.files(x),"agents.csv",sep="/"))

    allData=lapply(dataFiles,function(x) sapply(x,read.csv,sep=";"))
    g1=dataFiles$g1

    d1=lapply(names(dataFiles),function(x){ 
	      print(x) ; 
	      sapply(dataFiles[[x]][1:2],function(y){ cbind(read.csv(y,sep=";"),x)})
	       })

    net6=makeListWithAllFolder("~/result/6Net_test/")
    net15=makeListWithAllFolder("~/result/15Net_test/")
    net6Rp=makeListWithAllFolder("~/result/6Net_testRandomPrice//")
    netLast=makeListWithAllFolder("~/result/lastNet/")
    net4=makeListWithAllFolder("~/result/4Net_test/")
    net4Rp=makeListWithAllFolder("~/result/4Net_testRandomPrice/")
    plotMeanSd(net15)
    #boxplot(net6[[1]][1,]-net6[[1]][1,seq_along(net6[[1]][1,])]+1])
    plotDerive(net15,ylim=c(-0.05,0.05))
    png("checkMDeriv.png")
    plotDeriveOfMean(netLast,ylim=c(-0.,.1))
    dev.off()
    png("checkM.png")
    plotFun(a)

    conpl$new(net6[[1]][2:length(net6[[1]])])
    x=1:300
    points(x,u$xmin+x^(-u$pars),type="l")
    fit1 <- lm( y~offset(x) -1 )
    fit2 <- lm( y~x )
    fit3 <- lm( y~poly(x,3) )
    fit4 <- lm( y~poly(x,9) )
    library(splines)
    fit5 <- lm( y~ns(x, 3) )
    fit6 <- lm( y~ns(x, 9) )
    fit7 <- lm( y ~ x + cos(x*pi) )

    xx <- x
    lines(xx, predict(fit1, data.frame(x=xx)), col='blue')
    lines(xx, predict(fit2, data.frame(x=xx)), col='green')
    lines(xx, predict(fit3, data.frame(x=xx)), col='red')
    points(xx, predict(fit4, data.frame(x=xx)), col='purple')
    lines(xx, predict(fit5, data.frame(x=xx)), col='orange')
    points(xx, predict(fit6, data.frame(x=xx)), col='grey')
    lines(xx, predict(fit7, data.frame(x=xx)), col='black')


    boxplot(u$mean[u$timestep > 2000] ~ u$net[u$timestep > 2000])         

    dataRead=read.csv( "~/Downloads/G1-6.csv")
    dataRead=read.csv( "G1-15.csv")
    res=fitting(net15)
    pdf("meanCurveAndFit.pdf",width=9,height=10)
    layout(matrix(c(1,1,2,3), 2, 2, byrow = TRUE))
    plotFun(netLast)
    mtext("Fitted model : y ~ i * x ^ -z+k",side=1,line=4)
    plot(dataRead$average_dist,res$z,col=1:15,ylab="z",xlab="average_path")   
    text(label=dataRead$network,x=dataRead$average_dist,y=res$z-0.002,col=1:15)   
    plot(dataRead$density,res$k,col=1:15,ylab="i",xlab="dens")   
    text(label=dataRead$network,x=dataRead$density,y=res$k-0.03,col=1:15)   
    dev.off()
    plot(res$z,res$i,col=1:6,xlab="z",ylab="i")


    lines(predict(nls1),col="red")
    plot(data.frame(x=x,y=y))

    dev.off()
    net6[[5]]=net6[[5]][complete.cases(net6[[5]]),]
    all=createTable(net6)
   all15=createTable(net15)
   allLas=createTable(netLast)
   alla=createTable(a)

    sparsities=c(0,900,940,980)
   sapply(names(a),function(g){
	a[[g]] = cbind(G 
	       }
    sapply(alla,function(i){
	   pdf(paste("tmp/100/agentwrtK_D-",formatC(1000-i,width=4,format="d",flag="0"),".pdf",sep=""),pointsize=14)
	   t_comp=getLastIt(alld100WOUT[alld100WOUT$Sparsity ==i,])
	   plot(1,1,xlim=c(.5,6.5),ylim=c(0,100),type="n",xaxt="n",ylab="number of active agents",xlab="tournament size",main=paste("Evolution of #agents for different tournament size\n and density of ",(1000-i)/1000 ,sep=""))
	   axis(1,at=seq_along(tsize),labels=sort(tsize))
	   sapply(seq_along(tsize),function(k){
		  vioplot(t_comp$alive[t_comp$t_size == sort(tsize)[k]],at=k,add=T,col="white")})
	   dev.off()
	})


    utest=sapply(net6,function(net){
		 u=apply(net,2,mean);
		 fi=lm(y ~ x , list(y=u,x=as.numeric(names(u))));
	       })


	   l=net6[[1]][1,]
	   boxplot()
	   plotMeanSd(net4)
	   plotFun(net6,fun=sd)




	   dev.off()




	   test=read.csv("~/result/4Net_test/g2/run_0001/agents.csv",sep=";")
	   plotAllClassMean(test) 

}


fitting=function(idata){
    res=c()
    allFit=c()
    for( net in 1:length(idata)){
	y1=apply(idata[[net]],2,mean)
	y1=y1[2:length(y1)]
	x1=1:length(y1)
	model=nls(y~(i*(x^-z)),data=data.frame(x=x1,y=y1))
	allFit[[net]]=model
	res=rbind(res,model[["m"]]$getPars())
    }
    res=as.data.frame(res)
    return(res)
}


caaGraph=fucntion(){
    
sdMean=tapply(alla$mean,alla$timestep,sd)
	bestTime=names(sort(sdMean)[length(sdMean)])
	bestTimeB=as.numeric(bestTime)+90
	bestTimeC=as.numeric(bestTime)-90
	lastTime=names(sdMean[length(sdMean)])
	speed=alla[alla$timestep==bestTime,]
	ending=alla[alla$timestep==lastTime,]

	lim=c(.005,0.25)
	pdf(paste("tmp/speedDist.pdf",sep=""),pointsize=14)
	plot(1,1,xlim=lim,ylim=c(0,30),type="n",xaxt="n",ylab="Score",xlab="Density",main=paste("Distribution of Score at timestep:",lastTime))
	sapply(names(a),function(x){
	       u=a[[x]]
	       u=u[[lastTime]]
	       vioplot(u,at=fit$dens[fit$network == x],add=T,col="white",wex=0.05,xlim=lim)
	      })
	dev.off()

	lim=c(3,80)
	pdf(paste("tmp/speedDist.pdf",sep=""),pointsize=14)
	sapply(names(a),function(x){
	       u=a[[x]]
	       u=u[[bestTime]]
	       vioplot(u,at=fit$dist[fit$network == x],add=T,col="white",xlim=lim)
	      })
	dev.off()

	bestTime=900
	for(bestTime in names(a[[1]])[seq(2,134,2)]){
	    #pdf(paste("tmp/scoreComp",bestTime,".pdf",sep=""),pointsize=14,width=10,height=7)
	    png(paste("tmp/scoreComp",formatC(as.numeric(bestTime), width = 5, format = "d", flag = "0"),".png",sep=""),pointsize=14,width=1000,height=600)
	    plot(1,1,xlim=c(0,length(seq_along(fit$dist))+1),ylim=c(1,20),type="n",xaxt="n",ylab="Average Shortest Path Length",xlab="Score",main=paste("Score for all setup at timestep=",bestTime ,sep=""))
	    axis(1,at=seq_along(fit$dist),labels=sort(fit$dist),las=3)
	    sapply(names(a),function(x){
		   u=a[[x]]
		   u=u[[bestTime]]
		   print(fit$dist[fit$network == x])
		   print(match(fit$dist[fit$network == x],fit$dist))
		   vioplot(u,at=match(fit$dist[fit$network == x],fit$dist),add=T,col="white",xlim=lim)
	      })
	    dev.off()
	}


	bestTime="9900"
	for(bestTime in names(a[[1]])[seq(2,134,2)]){
	    pdf(paste("tmp/scoreComp",bestTime,".pdf",sep=""),pointsize=14,width=7,height=7)
	    #png(paste("tmp/scoreCompDen",formatC(as.numeric(bestTime), width = 5, format = "d", flag = "0"),".png",sep=""),pointsize=14,width=1000,height=600)
	    plot(1,1,xlim=c(0,max(fit$dens)+.011),ylim=c(1,20),type="n",xaxt="n",ylab="Density",xlab="Score",main=paste("Score for all setup at timestep=",bestTime ,sep=""))
	    axis(1,at=fit$dens,labels=sort(fit$dens),las=3)
	    sapply(names(a),function(x){
		   u=a[[x]]
		   u=u[[bestTime]]
		   print(fit$dens[fit$network == x])
		   print(match(fit$dens[fit$network == x],fit$dens))
		   vioplot(u,at=fit$dens[fit$network == x],add=T,col="white",xlim=lim,wex=.1)
	      })
	    dev.off()
	}

	

fit=read.csv("~/Dropbox/trade/python/complete/fits.csv") 



    full=getAllMeanScore("~/result/testFull/")




}

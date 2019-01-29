require("vioplot")
source("~/projects/PhD/dev/abc-pandora/abctools.R")
source("~/projects/PhD/dev/abc-pandora/function.R")
#datafolder="~/share_res/agentsShort.csv"
#expeDir="~/share_res/"
#ds=read.csv(datafolder,sep=";")
#
#
#
#    sparsities=c(0,900,940,980)
#    sapply(sparsities,function(i){
#	   pdf(paste("tmp/100/agentwrtK_D-",formatC(1000-i,width=4,format="d",flag="0"),".pdf",sep=""),pointsize=22)
#	   par(mar=c(5,4,2,.5),cex.lab=1.2)
#	   t_comp=getLastIt(alld100WOUT[alld100WOUT$Sparsity ==i,])
#	   plot(1,1,xlim=c(.5,6.5),ylim=c(35,100),type="n",xaxt="n",yaxt="n",ylab="#Active",xlab="Tournament Size",main=paste("Density=",(1000-i)/1000 ,sep=""))
#	   axis(1,at=seq_along(tsize),labels=sort(tsize))
#	   axis(2,at=c(40,60,80,100),labels=c(40,60,80,100))
#	   sapply(seq_along(tsize),function(k){
#		  vioplot(t_comp$alive[t_comp$t_size == sort(tsize)[k]],at=k,add=T,col="white")})
#	   dev.off()
#	})
#
plotEvol <- function(data,...){
    timestep=sort(unique(data$timeStep))
    plot(1,1, type="n",ylim=c(min(data$scores),max(data$scores)),xlim=c(1,length(timestep)),xaxt="n",ylab="score",xlab="time")#,main=paste("Density=",(1000-i)/1000 ,sep=""))
    axis(1,at=seq_along(timestep),labels=sort(timestep))
    sapply(seq_along(timestep),function(k){
	   print(seq_along(timestep))
	   print(timestep)
	   print(k)
	   print(timestep[k])
	   vioplot(data$scores[data$timeStep == timestep[k]],at=k,add=T,col="white")
	})
}



getSetOfResu <- function(pat){
    all=data.frame()
    print(paste("agents",pat,sep= ""))
    files=list.files(expeDir,pattern=paste("agents",pat,sep= ""))

    for ( i in files){
	print(i)
	all=rbind(all,read.csv(paste(expeDir,i,sep=""),sep=";"))
    }
    return(all)
}


 sapply(colnames(aa),function(i){
	png(paste0(i,"png")
	    plotDensities("~/",i,.16,range(aa[,i])[1],range(aa[,i])[2]) dev.off()})


plotDensities <- function(path,param,epsilon,from,to,...){
    htcol=topo.colors(length(epsilon),alpha=1)
    names(htcol)=epsilon
    #htcolF=topo.colors(length(epsilon),alpha=.5)
    htcolF=c(rgb(34, 139, 34,127.5,maxColorValue=255),rgb(178, 255, 255,127.5,maxColorValue=255))
    names(htcolF)=epsilon
    listParticles=lapply(epsilon,function(eps){print(eps);cbind(read.csv(paste(path,"result_",eps,".csv",sep="") ),epsilon=eps)})
    names(listParticles)=epsilon
    #print(listParticles)
    densities=lapply(listParticles,function(i){density(i[,param],from=from,to=to)})
    rdnm=runif(5000,from,to)
    densitiesPrio=density(rdnm,from=from,to=to)
    names(densities)=epsilon
    rangex=range(lapply(densities,function(i)range(i$x)))
    rangey=range(lapply(densities,function(i)range(i$y)))
    par(mar=c(5,5,1,1))
    plot(density(listParticles[[1]][,param]),ylim=rangey,type="n",main="", xlab=substitute(p,list(p=param)),...)
    polygon(c(from,densitiesPrio$x,to),c(0,densitiesPrio$y,0),col=htcolF[2],lwd=2)
    lapply(seq_along(densities),function(i){
	   polygon(c(from,densities[[i]]$x,to),c(0,densities[[i]]$y,0),col=htcolF[names(densities)[i]],lwd=2)#,density=20,angle=45*i,border=htcol[names(densities)[i]])
#	   abline(v=mean(densities[[i]]$x),col=htcol[names(densities)[i]])
#	   text(mean(densities[[i]]$x),0,names(densities)[i],col=htcol[names(densities)[i]])
	})
    text(from,max(densitiesPrio$y)+.05*max(densitiesPrio$y),substitute(prior:param %~% italic(u)(from,to),list(param=param,from=from,to=to)))
    legend("topright",legend=c("prior","posterior"),fill=rev(htcolF))
}


main <- function(){

    uuu2=read.csv("~/share_res/result_1.00.csv")
    uuu=read.csv("~/share_res/luv/")
    uuu=read.csv("~/share_res/result_0.20.csv")

    path="~/share_res/luv/"
    epsilon=c("0.30","0.20","0.10")
    uuu2=read.csv("~/share_res/full/result_0.50.csv",sep= ";", header=F)
    uuu1=read.csv("~/share_res/full/result_0.48.csv",sep= ";", header=F)
    uuu0=read.csv("~/share_res/full/result_0.46.csv",sep= ";", header=F)
    uuu4=read.csv("~/share_res/full/result_0.44.csv",sep= ";", header=F)

    path="~/share_res/testAg/"
    plotDensities(path,"mu",c("2000.00","0.20"))

    plotDensities(path,"mu",epsilon)

    path="~/share_res/noNegat//"
    plotDensities(path,"mu",c("2000.00","0.20"))

    pathBig="~/share_res/biggerThanPhat/"
    plotDensities(pathBig,"cstep",c("3000.00","0.20"))
    tt=read.csv("~/share_res/testMu/3_100_0.6_20_0.124471348764/agents.csv",sep=";")
    tt2=read.csv("~/share_res/testMu/3_100_0.6_20_0.832957020173/agents.csv",sep=";")

    alltt=rbind(tt,tt2)

    alltt=alltt[alltt$timeStep >= 6000,]

    tt=read.csv("~/share_res/biggerThanPhat/2_199_0.508440976643_33_0.967006910121/agents.csv",sep=";")
    tt2=read.csv("~/share_res/biggerThanPhat/3_387_0.827853613564_24_0.214588177227/agents.csv",sep=";")

    mean(tt$score[tt$timeStep == max(tt$timeStep)])/(33*1)
    mean(tt2$score[tt2$timeStep == max(tt2$timeStep)])/(24*2)

    alltt=rbind(tt,tt2)

    boxplot(alltt$score~ alltt$mu)
    getPropFromXml("~/share_res/testMu/3_100_0.6_20_0.124471348764/","controller","step")
    getPropFromXml("~/share_res//3_100_0.6_20_0.124471348764/","controller","step")

    path="~/share_res/smallTight/"
    plotDensities(path,"cstep",c("3000.00","0.05"))

    path="~/share_res/testMuBisWider/"
    path="~/share_res/testMutRand200/"
    tt2=read.csv("~/share_res/testMuBisWider/3_50_0.6_10_0.000590196537098/agents.csv",sep=";")
    tt2=getAllMeanScore("~/share_res/testMu/")
    pposteriorlotDensities(path,"mu",c("3000.00","0.25"))
    #Devrait pouvoir etre assz utile de faire une fonction qui genere cce graphe de façon auto en fonction de deux variable
    plot(density(uuu$V3),xlim=range(c(density(uuu2$V3)$x,density(uuu1$V3)$x,density(uuu0$V3)$x)),ylim=range(c(density(uuu2$V3)$y,density(uuu1$V3)$y,density(uuu0$V3)$y)),)

    lines(density(uuu1$V1),col="orange")
    lines(density(uuu0$V3),col="red")

    #pdf("../../../doc/conferences/20170127_YSLRWinterWorkshop/img/ABC.pdf")
    pdf("~/resultsABC.pdf")
    par(mar=c(5,4,1,1))
     plotDensities(path,"mu",c("3000.00","0.90"),from=0,to=1,xlim=c(0,1))
    dev.off()
    pdf("~/presModel/secondSim.pdf")
     barplot(table(read.csv("../bhB.csv",header=F)),cex.names=.5,las=2)   
    dev.off()
    pdf("~/presModel/Sim.pdf")
     barplot(table(read.csv("../bhB.csv",header=F)),cex.names=.5,las=2)   
    dev.off()

}



manualABC<-function(){
	allr=read.csv("~/share_res/results_big.csv")
	allr$nscore= allr$score / (allr$cstep * allr$ngoods)
	allr=na.omit(allr)
	prior=allr
	posterior=allr[allr$nscore < .08693,]
	hist(prior$nag_good,freq=F)
	lines(density(prior$nag_good))
	lines(density(posterior$nag_good))
}

lapply(names(realdatadiversities),function(i){pdf(paste0("~/presModel/realdivers",i,".pdf"));plotSiteWithGood(realdatadiversities[[i]]);dev.off()})
lapply(names(realdatadiversities),function(i){pdf(paste0("~/presModel/realdivers",i,"prop.pdf"));plotSiteWithGood(getprop(realdatadiversities[[i]]));dev.off()})
plot( multiexp$count ~multiexp$div)

	years=rownames(multiexp$count)
	colyear=topo.colors(length(years))
	names(colyear)=years
	pdf("~/presModel/zscorePATTERNtest.pdf")
	plot(multiexp$count ~ multiexp$div,col=colyear[rownames(multiexp$count)],pch=20,ylab="diversity",xlab="distribution",main="zscores")
	legend("topright",legend=years,col=colyear,pch=20,title="#periods")
	dev.off()
	pdf("~/presModel/zscorePROPtest.pdf")
	plot(multiexp$div ~ zscorePROP$div,col=colyear[rownames(multiexp$count)],pch=20,ylab="raw",xlab="proportion",main="zscores")
	dev.off()

	pdf("~/presModel/2scoretest.pdf")
	plot(multiexp$count ~ absdif$count,col=colyear[rownames(multiexp$count)],pch=20,ylab="zscore",xlab="absolute diff",main="zscores")
	dev.off()
	pdf("~/presModel/absPATTERNtest.pdf")
	plot(absdif$count ~ absdif$div,col=colyear[rownames(absdif$count)],pch=20,ylab="diversity",xlab="distribution",main="mean of abs diff")
	legend("topright",legend=years,col=colyear,pch=20,title="#periods")
	dev.off()

	pdf("~/presModel/zscorePSZIE.pdf")
	plot(1,1,ylim=range(multiexp$div,na.rm=T),xlim=c(.5,length(rownames(multiexp$div))+.5),axes=F,ylab=" score" ,xlab="size of P (y)" )  
	        axis(2)
	        axis(1,1:length(rownames(multiexp$div)),label=rownames(multiexp$div))
	apply(multiexp$div,2,lines,lwd=.05) 
	dev.off()

	pdf("~/presModel/zscorePSZIEcount.pdf")
	plot(1,1,ylim=range(multiexp$count,na.rm=T),xlim=c(.5,length(rownames(multiexp$count))+.5),axes=F,ylab=" score" ,xlab="size of P (y)" )  
	        axis(2)
	        axis(1,1:length(rownames(multiexp$count)),label=rownames(multiexp$count))
	apply(multiexp$count,2,lines,lwd=.05) 
	dev.off()

	pdf("~/presModel/absdifPSZIE.pdf")
	plot(1,1,ylim=range(absdif$div,na.rm=T),xlim=c(.5,length(rownames(absdif$div))+.5),axes=F,ylab=" score" ,xlab="size of P (y)" )  
	        axis(2)
	        axis(1,1:length(rownames(absdif$div)),label=rownames(absdif$div))
	apply(absdif$div,2,lines,lwd=.05) 
	dev.off()

	pdf("~/presModel/absdifPSZIEcount.pdf")
	plot(1,1,ylim=range(absdif$count,na.rm=T),xlim=c(.5,length(rownames(absdif$count))+.5),axes=F,ylab=" score" ,xlab="size of P (y)" )  
	        axis(2)
	        axis(1,1:length(rownames(absdif$count)),label=rownames(absdif$count))
	apply(absdif$count,2,lines,lwd=.05) 
	dev.off()

	pdf("~/presModel/absdifPROP_propPSZIE.pdf")
	plot(1,1,ylim=range(absdifPROP$div,na.rm=T),xlim=c(.5,length(rownames(absdifPROP$div))+.5),axes=F,ylab=" score" ,xlab="size of P (y)" )  
	        axis(2)
	        axis(1,1:length(rownames(absdifPROP$div)),label=rownames(absdifPROP$div))
	apply(absdifPROP$div,2,lines,lwd=.05) 
	dev.off()


	pdf("~/presModel/absdifPSPL.pdf")
	plot(1,1,ylim=range(absdif$count_sample,na.rm=T),xlim=c(.5,6+.5),axes=F,ylab=" score" ,xlab="%sample" )  
	        axis(2)
	        axis(1,1:length(rownames(absdif$count_sample)),label=rownames(absdif$count_sample))
	apply(absdif$count_sample,2,lines,lwd=.1) 
	dev.off()

	pdf("~/presModel/zscorePSPL.pdf")
	plot(1,1,ylim=range(multiexp$count_sample,na.rm=T),xlim=c(.5,6+.5),axes=F,ylab=" score" ,xlab="%sample" )  
	        axis(2)
	        axis(1,1:length(rownames(multiexp$count_sample)),label=rownames(multiexp$count_sample))
	apply(multiexp$count_sample,2,lines,lwd=.1) 
	dev.off()

	pdf("~/presModel/absdifPSPL.pdf")
	plot(1,1,ylim=range(absdif$div_sample,na.rm=T),xlim=c(.5,6+.5),axes=F,ylab=" score" ,xlab="%sample" )  
	        axis(2)
	        axis(1,1:length(rownames(absdif$div_sample)),label=rownames(absdif$div_sample))
	apply(absdif$div_sample,2,lines,lwd=.1) 
	dev.off()

	pdf("~/presModel/zscorePSPL.pdf")
	plot(1,1,ylim=range(multiexp$div_sample,na.rm=T),xlim=c(.5,6+.5),axes=F,ylab=" score" ,xlab="%sample" )  
	        axis(2)
	        axis(1,1:length(rownames(multiexp$div_sample)),label=rownames(multiexp$div_sample))
	apply(multiexp$div_sample,2,lines,lwd=.1) 
	dev.off()

	pdf("~/presModel/absdifPROPPSPL.pdf")
	plot(1,1,ylim=range(absdifPROP$count_sample,na.rm=T),xlim=c(.5,6+.5),axes=F,ylab=" score" ,xlab="%sample" )  
	        axis(2)
	        axis(1,1:length(rownames(absdifPROP$count_sample)),label=rownames(absdifPROP$count_sample))
	apply(absdifPROP$count_sample,2,lines,lwd=.1) 
	dev.off()
	plot(1,1,ylim=range(absdif$div,na.rm=T),xlim=c(0,10),ylab="zscore",xlab="#periods",lwd=.05)  

	plot(1,1,ylim=range(absdif$count,na.rm=T),xlim=c(0,12))  
	apply(absdif$count,2,lines) 
	names(topten)


	foldname=colnames(multiexp$count)
	biases=seq(0,1,.2)
        sample_exp = foldname[sample.int(length(foldname),50,replace=F)]
        sample_exp = foldname[1:50]
	spl=file.path("~/projects/PhD/dev/abc-pandora/",sample_exp,"agents.csv")
        multiexp[["count_sample"]]=     sapply( spl,function(eij){print(eij);sapply(biases,function(y){print(y);tryCatch(zscore(t(agentWith(read.csv(eij,sep=";"),breaks=25,numsite=200*y,        type="count")),realdatadistributions[[as.character(25)]]),error=function(err){NA})})})
        zscorePROP[["count_sample"]]=   sapply( spl,function(eij){print(eij);sapply(biases,function(y){print(y);tryCatch(zscore(getprop(t(agentWith(read.csv(eij,sep=";"),breaks=25,numsite=200*y,type="count"))),getprop(realdatadistributions[[as.character(25)]])),error=function(err){NA})})})

        absdif[["count_sample"]]=       sapply( spl,function(eij){print(eij);sapply(biases,function(y){print(y);tryCatch(absdiff(t(agentWith(read.csv(eij,sep=";"),breaks=25,numsite=200*y,       type="count")),realdatadistributions[[as.character(25)]]),error=function(err){NA})})})
        absdifPROP[["count_sample"]]=   sapply( spl,function(eij){print(eij);sapply(biases,function(y){print(y);tryCatch(absdiff(getprop(t(agentWith(read.csv(eij,sep=";"),breaks=25,numsite=200* y,type="count"))),getprop(realdatadistributions[[as.character(25)]])),error=function(err){NA})})})

	pdf("~/presModel/succes.pdf")
	plotAllDensities(getlistParticlesFromPath("~/projects/PhD/dev/abc-pandora/realIntro/CITIESPL/"))
	dev.off()

	pdf("~/presModel/random.pdf")
	plotAllDensities(getlistParticlesFromPath("~/projects/PhD/dev/abc-pandora/realIntro/trand/"))
	plotAllDensities(getlistParticlesFromPath("~/projects/PhD/dev/abc-pandora/nuevoscore/"))
	dev.off()
	rdm=getlistParticlesFromPath("~/projects/PhD/dev/abc-pandora/realIntro/trand/")
	sbd=getlistParticlesFromPath("~/projects/PhD/dev/abc-pandora/realIntro/CITIESPL/")
	mixd=list(random=rdm[["0.0894"]],succes=sbd[["0.0894"]])
	plotDensities(mixd,"mu")


plotDensities <- function(table,param,...){
	#we assume table1 is a "priorlike" function
	#htcolF=c(rgb(34, 139, 34,127.5,maxColorValue=255),rgb(178, 255, 255,127.5,maxColorValue=255))
	rng=range(table[[1]][,param])
	print(rng)
	from=rng[1]
	htcolF=c("blue","green")
	names(htcolF)=c("succes","random")
	to=rng[2]
	listParticles=table[1:length(table)]
	names(listParticles)=names(table)
	densitiesPrio=density(table[[1]][,param],from=from,to=to)
	densitiesPrio2=density(table[[2]][,param],from=from,to=to)
	par(mar=c(5,5,1,1))
	plot(density(listParticles[[1]][,param]),type="n",main="", xlab=substitute(p,list(p=param)),...)
	polygon(c(from,densitiesPrio$x,to),c(0,densitiesPrio$y,0),col=htcolF[names(table)[1]],lwd=2)
	polygon(c(from,densitiesPrio2$x,to),c(0,densitiesPrio2$y,0),col=htcolF[names(table)[2]],lwd=2)
	legend("topright",legend=names(table),fill=htcolF[names(table)])
}

row=names(which.min(apply( multiexp$count,1,min,na.rm=T)) )
col=names(which.min(apply( multiexp$count,2,min,na.rm=T)) )
best=cbind(multiexp$div[row,col],multiexp$count[row,col])
plot(multiexp$count ~ multiexp$div,col=colyear[rownames(multiexp$count)],pch=20)
plot(multiexp$count ~ absdif$count,col=colyear[rownames(multiexp$count)],pch=20)
plot(multiexp$div ~ absdif$div,col=colyear[rownames(multiexp$count)],pch=20)
points(best,col="red",cex=2)

col=names(which.max(apply( multiexp$count,1,max,na.rm=T)) )
row=names(which.max(apply( multiexp$count,1,max,na.rm=T)) )
worst=cbind(multiexp$div[row,col],multiexp$count[row,col])
points(worst,col="red",cex=2)


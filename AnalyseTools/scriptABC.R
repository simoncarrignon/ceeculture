require("vioplot")
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


plotDensities <- function(path,param,epsilon,from,to,...){
    htcol=topo.colors(length(epsilon),alpha=1)
    names(htcol)=epsilon
    #htcolF=topo.colors(length(epsilon),alpha=.5)
    htcolF=c(rgb(34, 139, 34,127.5,maxColorValue=255),rgb(178, 255, 255,127.5,maxColorValue=255))
    names(htcolF)=epsilon
    listParticles=lapply(epsilon,function(eps){print(eps);cbind(read.csv(paste(path,"result_",eps,".csv",sep="") ),epsilon=eps)})
    names(listParticles)=epsilon
    print(listParticles)
    densities=lapply(listParticles,function(i){density(i[,param],from=from,to=to)})
    names(densities)=epsilon
    rangex=range(lapply(densities,function(i)range(i$x)))
    rangey=range(lapply(densities,function(i)range(i$y)))
    par(mar=c(5,5,1,1))
    plot(density(listParticles[[1]][,param]),ylim=rangey,type="n",main="", xlab=expression(mu),...)
    lapply(seq_along(densities),function(i){
	   polygon(c(0,densities[[i]]$x,to),c(0,densities[[i]]$y,0),col=htcolF[names(densities)[i]],lwd=2)#,density=20,angle=45*i,border=htcol[names(densities)[i]])
#	   abline(v=mean(densities[[i]]$x),col=htcol[names(densities)[i]])
#	   text(mean(densities[[i]]$x),0,names(densities)[i],col=htcol[names(densities)[i]])
	})
    text(.65,1.2,expression(prior:mu %~% italic(u)(0,1)))
    legend("topright",legend=c("prior","posterior"),fill=htcolF)
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


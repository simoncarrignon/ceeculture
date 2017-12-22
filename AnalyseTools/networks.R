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
    names(net15)=basename(names(net15))
    net6Rp=makeListWithAllFolder("~/result/6Net_testRandomPrice//")
    netLast=makeListWithAllFolder("~/result/lastNet/")
    net4=makeListWithAllFolder("~/result/4Net_test/")
    net4Rp=makeListWithAllFolder("~/result/4Net_testRandomPrice/")
    plotMeanSd(net15)
    #boxplot(net6[[1]][1,]-net6[[1]][1,seq_along(net6[[1]][1,])]+1])
    plotDerive(netLast,ylim=c(-0.05,0.05))
    png("checkMDeriv.png")
    plotDeriveOfMean(netLast,ylim=c(-0.1,.1))
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
    dataRead=read.csv( "~/Downloads/G1-6.csv") 
    dataRead=read.csv( "~/Dropbox/trade/python/complete/fits.csv") 
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
	a[[g]] = cbind(G )       })
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


newPrintFunction <- function(){
    
    netLast=c(netLast,net15)
    netLastB=netLast
    netLast=netLastB
    names(netLastAll)=basename(names(netLastAll))
    tims=2
    timsA=0
    timsB=1
    for( i in 1:12){
    print(tims)
    allmeanAt=sapply(netLastAll,function(i)i[1:98,tims])   #get score value of last time step
    dataRead=read.csv( "~/Dropbox/trade/python/complete/fits.csv") 
    dataRead=dataRead[ dataRead$network %in% colnames(allmeanAt),]    ##get only the data that are in allmean
    meanPer=apply(allmeanAt,2,sd) #take the means of all different exp at the end
    clrsD=heat.colors(length(dataRead$dens)) 
    names(clrsD)=sort(dataRead$dens,decreasing=T) 

    clrsL=heat.colors(length(dataRead$dist)) 
    names(clrsL)=sort(dataRead$dist) 

    png(paste0("SD-density-n-lengh",formatC(tims,width=5,flag=0),".png"),width=1000)
    par(mfrow=c(1,2))
    plot(meanPer[ as.character(dataRead$network) ] ~ dataRead$dist,xlab="dist",ylab="mean score at the end",log="x",bg=clrsD[as.character(dataRead$dens)],pch=21,cex=2,ylim=c(0,2),main=paste("Time step:",colnames(netLast[[1]])[tims]))
    plot(meanPer[ as.character(dataRead$network) ] ~ dataRead$dens,xlab="dens",ylab="sd score at the end",bg=clrsL[as.character(dataRead$dist)],pch=21,cex=2,ylim=c(0,2),main=paste("Time step:",colnames(netLast[[1]])[tims]))
    dev.off()
	tims=timsA+timsB
    timsA=timsB
    timsB=tims
    }


    ##create csv for ignacio

aaa=sapply(colnames(netLastAll[[1]])[seq(1,222,4)],function(tims) apply(sapply(netLastAll,function(i)i[1:98,tims]),2,sd) )
write.csv(aaa,"allSDScoreByTimeAndNet.csv")


test=read.csv("~/share_res/goodTiming/exp_10/agents.csv",sep=";")
test=read.csv("~/share_res/agents.csv/exp_10/agents.csv",sep=";")

testR=read.csv("~/share_res/agents.csv",sep=";")
boxplot(testR$u ~ testR$timeStep)#,log="y",ylim=c(.0000001,1000))
boxplot(testR$u-testR$opt <- u ~ testR$timeStep,ylim=c(-0.1,0.1))
boxplot(testR$u-testR$opt <- u ~ testR$timeStep,ylim=c(-0.1,0.1))


testR=testR[testR$score ,]

testNR=read.csv("~/share_res/agents.csv",sep=";")
test=read.csv("~/share_res/checkrandom.csv",sep=";")
testG=read.csv("~/share_res/checkgintisCop.csv",sep=";")
testG=read.csv("~/share_res/gintRandom/exp_100/agents.csv",sep=";")
testH=read.csv("~/share_res/gintCopyma/exp_10/agents.csv",sep=";")
test6G=read.csv("~/share_res/gintCopyma6G/exp_200/agents.csv",sep=";")
test6GN=read.csv("~/share_res/gintCopyma6G/exp_200/agents.csv",sep=";")
test6GNLong=read.csv("~/share_res/gintCopymaNEW6G45000/exp_104/agents.csv",sep=";")
testCons=read.csv("~/share_res/consProd.csv",sep=";")
testCons2G=read.csv("~/share_res/gintCopyma2G13500OPT/exp_100/agents.csv",sep=";")
testConsb2G=read.csv("~/share_res/gintCopyma2G34500OPT/exp_160/agents.csv",sep=";")
plot(testConsb2G$g0_q[testConsb2G$p_good == "g0"])   
plot(testConsb2G$g1_q[testConsb2G$p_good == "g1"])   

testG1
plot(unique(testR$timeStep),abs(9-apply(getSumOf(testR,"n"),2,mean,na.rm=T)),type="l", ylim=c(0,20))
plot(unique(testR$timeStep),apply(getSumOf(testR,"p"),2,mean,na.rm=T),type="l")
boxplot(abs(4-getSumOf(testR,"n")),ylim=c(0,10),outline=F)
boxplot(getOf(testR,"p"))
boxplot(testR$g3_q ~testR$timeStep)
plot(getSumOf(test6GN,"q", sum)~getSumPrice(test6GN,"n", sum),ylim=c(0,10))
plot(getSumOf(test6GNLong,"p", sum),ylim=c(0.1,10))


testscore=getAllMeanScore("~/share_res/goodTiming/")
ginran=getAllMeanScore("~/share_res/gintRandom//")
ginranB=getAllMeanAlgo("~/share_res/gintRandom//")
ginranC=getAllMeanAlgo("~/share_res/gintRandom//",fun="median")
gincopmax6G=getAllMeanAlgo("~/share_res/gintCopyma6G/",fun="median")
gincopmax6GN=getAllMeanAlgo("~/share_res/gintCopyma6G/",fun="median")
gincopmax6GNC=getAllMeanScore("~/share_res/gintCopyma6G/")
gincopmax6GNCLong=getAllMeanScore("~/share_res/gintCopymaNEW6G45000/")
gincopmax6GNLong=getAllMeanAlgo("~/share_res/gintCopymaNEW6G45000/",fun=median)
gincopmax=getAllMeanScore("~/share_res/gintCopyma/")
gincopmaxB=getAllMeanAlgo("~/share_res/gintCopyma/")
gincopmax2C=getAllMeanAlgo("~/share_res/gintCopyma2G13500OPT/",fun="median")
gincopmax2=getAllMeanScore("~/share_res/gintCopyma2G13500OPT/")
gincopmaxb2C=getAllMeanAlgo("~/share_res/gintCopyma2G34500OPT/",fun="median")
gincopmaxb2=getAllMeanScore("~/share_res/gintCopyma2G34500OPT/")

randn=getAllMeanScore("~/share_res/randn/")
randn4=getAllMeanScore("~/share_res/randn4/")
boxplot(randn4)
random4=getAllMeanScore("~/share_res/random4/")
boxplot(random4)
rand=getAllMeanScore("~/share_res/random/")
randomSel=getAllMeanScore("~/share_res/testSelf/")
boxplot(random4)

autFoldTest("gintCopyma2G13500OPT")
autFoldTest("gintCopyma2G34500OPT")
autFoldTest("gintCopyma2G12250OPT")
autFoldTest("gintCopyma2G12250OPTwoP")
autFoldTest("gintRand2G12250OPTwoP")
autFoldTest("gintCopymax2G13500OPTwoP")
autFoldTest("gintCopymax3G40500OPTwoP")
autFoldTest("gintCopymax6G40500OProd")
autFoldTest("gintRand3G40500OProd")
autFoldTest("gintRand3G600ag40500O")
	


plot(unique(test6GN$timeStep),apply(getSumOf(test6GN,"p"),2,mean,na.rm=T),type="l")


u=c()
for (  i in levels(testG$p_good)){
     u=cbind(u,testG[,paste(i,"_q",sep="")] * testG[,paste(i,"_p",sep="")])
	
}
	     

getAllMeanAlgo<-function(expeDir,timeA=0,timeB=0,timestep=1,maxfolder=10000,listOfXmlValue=c(),fun=mean){

    all=data.frame()
    files=list.files(expeDir,pattern="exp*")
    sim=0

    for ( i in files[1:min(c(length(files),maxfolder))]){
	folder=	paste(expeDir,i,sep="")
	file=	paste(expeDir,i,"/agents.csv",sep="")
	print(file)
	work=try(read.csv(file,sep=";"))
    	if(is.data.frame(work)){
	    work=work[work$timeStep %% timestep == 0,]
	    #toBind=tapply(work$scores,work$timeStep,fun)
	    toBind=tapply(work$u/work$opt_u,work$timeStep,fun)

	    #	m=getPropFromXml(folder,"network","m")
	    #v=getPropFromXml(folder,"network","v")
	    #network=paste("networks/",sim,"_",colnames(toBind),".gdf",sep= "")
	    all=rbind(all,cbind(t(toBind)))#,m,network))
	    sim=sim+1
	}
	else
	    print(paste("Fodler",folder,"doesn't contains the agents.csv file"))
    }
    return(all)
}
autFoldTest<-function(folder){
thisfol=paste("~/share_res/",folder,"/",sep="")
gincopmaxb2C=getAllMeanAlgo(thisfol,fun="median")
gincopmaxb2=getAllMeanScore(thisfol)
testCons2G=read.csv(paste(thisfol,"/exp_92/agents.csv",sep=""),sep=";")
testCons2Gb=read.csv(paste(thisfol,"/exp_93/agents.csv",sep=""),sep=";")
pdf(paste("~/results",folder,".pdf",sep=""))
boxplot(gincopmaxb2,main="score")
boxplot(gincopmaxb2C,main="ratio")

sapply(levels(testCons2G$p_good),function(i){
       oneprod=testCons2G[testCons2G$p_good == i,]
       plot(oneprod$g0_p,main=paste("prices prod",i))   
       points(oneprod$g1_p,col="blue")   
       points(oneprod$g2_p,col="green")   
       legend("topleft",c("g0","g1","g2"),fill=c(1,"blue","green"))
})
sapply(levels(testCons2G$p_good),function(i){
       oneprod=testCons2G[testCons2G$p_good == i,]
       plot(oneprod$g0_q,main=paste("quantities prod",i))   
       points(oneprod$g1_q,col="blue")   
       points(oneprod$g2_q,col="green")   
       legend("topleft",c("g0","g1","g2"),fill=c(1,"blue","green"))
})
sapply(levels(testCons2G$p_good),function(i){
       oneprod=testCons2G[testCons2G$p_good == i,]
       plot(oneprod$g0_n,main=paste("need prod",i),ylim=c(.00001,1000),log="y")   
       points(oneprod$g1_n,col="blue")   
       points(oneprod$g2_n,col="green")   
       legend("topleft",c("g0","g1","g2"),fill=c(1,"blue","green"))
})

sapply(levels(testCons2G$p_good),function(i){
       oneprod=testCons2G[testCons2G$p_good == i,]
       plot(oneprod$g0_q/oneprod$g0_n,main=paste("quantity vs curneed prod",i),ylim=c(-1,1.5))#,log="y")   
       points(oneprod$g1_q/oneprod$g1_n,col="blue")   
       points(oneprod$g2_q/oneprod$g2_n,col="green")   
       legend("topleft",c("g0","g1","g2"),fill=c(1,"blue","green"))
})

sapply(levels(testCons2G$p_good),function(i){
       oneprod=testCons2G[testCons2G$p_good == i,]
       plot(oneprod$g0_q/oneprod$g0_on,main=paste("quantity vs optneed prod",i),ylim=c(-1,1.5))#,log="y")   
       points(oneprod$g1_q/oneprod$g1_on,col="blue")   
       points(oneprod$g2_q/oneprod$g2_on,col="green")   
       legend("topleft",c("g0","g1","g2"),fill=c(1,"blue","green"))
})

plot(getSumOf(testCons2G,"q", sum)~getSumOf(testCons2G,"n", sum),main="t1")
plot(getSumOf(testCons2Gb,"q", sum)~getSumOf(testCons2G,"n", sum),main="t2")
plot(getSumOf(testCons2G,"q", sum)~getSumOf(testCons2G,"on", sum),main="t1")
plot(getSumOf(testCons2Gb,"q", sum)~getSumOf(testCons2G,"on", sum),main="t2")
plot(getSumOf(testCons2G,"p", sum),main="t1 price")
plot(getSumOf(testCons2Gb,"p", sum),main="t2 price")
plot(getSumOf(testCons2G,"q", sum),main="t1 quantity")
plot(getSumOf(testCons2Gb,"q", sum),main="t2 quatity")
plot(getSumOf(testCons2G,"n", sum),main="t1 need",ylim=c(.00001,1000),log="y")
plot(getSumOf(testCons2Gb,"n", sum),main="t2 need",ylim=c(.00001,1000),log="y")
dev.off()
}

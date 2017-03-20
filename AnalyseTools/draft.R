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

testG1
plot(unique(testR$timeStep),abs(9-apply(getSumOf(testR,"n"),2,mean,na.rm=T)),type="l", ylim=c(0,20))
plot(unique(testR$timeStep),apply(getSumOf(testR,"p"),2,mean,na.rm=T),type="l")
boxplot(abs(4-getSumOf(testR,"n")),ylim=c(0,10),outline=F)
boxplot(getOf(testR,"p"))
boxplot(testR$g3_q ~testR$timeStep)

testscore=getAllMeanScore("~/share_res/goodTiming/")
ginran=getAllMeanScore("~/share_res/gintRandom//")
ginranB=getAllMeanAlgo("~/share_res/gintRandom//")
ginranC=getAllMeanAlgo("~/share_res/gintRandom//",fun="median")
gincopmax6G=getAllMeanAlgo("~/share_res/gintCopyma6G/",fun="median")
gincopmax6GN=getAllMeanAlgo("~/share_res/gintCopyma6G/",fun="median")
gincopmax6GNC=getAllMeanScore("~/share_res/gintCopyma6G/")
gincopmax6GC=getAllMeanScore("~/share_res/gintCopyma6G/")
gincopmax=getAllMeanScore("~/share_res/gintCopyma/")
gincopmaxB=getAllMeanAlgo("~/share_res/gintCopyma/")
gincopmaxC=getAllMeanAlgo("~/share_res/gintCopyma/",fun="median")
boxplot(testscore)
randn=getAllMeanScore("~/share_res/randn/")
randn4=getAllMeanScore("~/share_res/randn4/")
boxplot(randn4)
random4=getAllMeanScore("~/share_res/random4/")
boxplot(random4)
rand=getAllMeanScore("~/share_res/random/")
randomSel=getAllMeanScore("~/share_res/testSelf/")
boxplot(random4)
	

testH$g1_qua



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
	work=read.csv(file,sep=";")
	work=work[work$timeStep %% timestep == 0,]
	#toBind=tapply(work$scores,work$timeStep,fun)
	toBind=tapply(work$u/work$opt_u,work$timeStep,fun)

	#	m=getPropFromXml(folder,"network","m")
	#v=getPropFromXml(folder,"network","v")
	#network=paste("networks/",sim,"_",colnames(toBind),".gdf",sep= "")
	all=rbind(all,cbind(t(toBind)))#,m,network))
	sim=sim+1
    }
    return(all)
}

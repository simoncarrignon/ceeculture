source("ext_scripts/analysis.R")
source("ext_scripts/configHandle.R")

cbbPalette <- c("#000000", "#E69F00", "#56B4E9", "#009E73", "#F0E442", "#0072B2", "#D55E00", "#CC79A7")
options(scipen=10)

#ceramics=read.csv("type.csv",row.names=1)
#ceramics[is.na(ceramics)]=0
#ceramics=ceramics[,colnames(ceramics)[c(2,5,4,1,3)]]
#pdf("~/presModel/type.pdf")
#barplot(t(ceramics),main="Real Data",space=0,border=NA)
#dev.off()

basuraJunio <- function(){

    par(mfrow=c(2,1))
    barplot(concateOnMeasurement(read.csv("~/share_res/produce1_randnd_500ag_5G_Gin06copymax/exp_24/agents.csv",sep=";"),"p"),space=0,border=NA)
    barplot(concateOnMeasurement(read.csv("~/share_res/produce1_randnd_500ag_5G_Gin06copymax/exp_24/agents.csv",sep=";"),"q"),space=0,border=NA)

    par(mfrow=c(2,2),mar=rep(.1,4))
    layout(matrix(1:3,1,3),c(0.45,0.45,0.2))
    barplot(concateOnMeasurement(read.csv("~/result/randnGin06/exp_10/agents.csv",sep=";"),"p"),space=0,border=NA,axes=F,names.arg=F)
    barplot(concateOnMeasurement(read.csv("~/result/randnGin06/exp_10/agents.csv",sep=";"),"q"),space=0,border=NA,axes=F,names.arg=F)
    barplot(concateOnMeasurement(read.csv("~/result/randnGin06/exp_10/agents.csv",sep=";"),"n"),space=0,border=NA,axes=F,names.arg=F)

    barplot(concateOnMeasurement(read.csv("~/share_res/500agentsFlou.csv",sep=";"),"p"),space=0,border=NA,axes=F,names.arg=F)
    barplot(concateOnMeasurement(read.csv("~/share_res/500agentsFlou.csv",sep=";"),"q"),space=0,border=NA,ylim=c(0,1))
    barplot(concateOnMeasurement(read.csv("~/share_res/checksocre2",sep=";"),"p"),space=0,border=NA)
    barplot(concateOnMeasurement(read.csv("~/share_res/checksocre2",sep=";"),"q"),space=0,border=NA,axes=F,names.arg=F)
    barplot(concateOnMeasurement(read.csv("~/share_res/checksocre2",sep=";"),"n"),space=0,border=NA)

    barplot(concateOnMeasurement(read.csv("~/share_res/100agentsMan.csv",sep=";"),"p"),space=0,border=NA,axes=F,names.arg=F)
    barplot(concateOnMeasurement(read.csv("~/share_res/100agentsMan.csv",sep=";"),"q"),space=0,border=NA,ylim=c(0,1))
    barplot(concateOnMeasurement(read.csv("~/share_res/100agentsMan.csv",sep=";"),"n"),space=0,border=NA,axes=F,names.arg=F)




    ############################
    ############################
    ############################
    gin06gin06on=read.csv("~/share_res/gin06gin06oncopymax.csv",sep=";")
    gin06gin06=read.csv("~/share_res/gin06gin06copymax.csv",sep=";")
    customgin06=read.csv("~/share_res/customgin06copymin.csv",sep=";")
    customgin06on=read.csv("~/share_res/customgin06oncopymin.csv",sep=";")

    checkev=read.csv("~/share_res/checkEvents",sep=";")

    layout(matrix(1:4,1,4),c(0.45,0.45,0.45,0.2))
    boxplot(gin06gin06on$scores ~ gin06gin06on$timeStep)
    barplot(concateOnMeasurement(gin06gin06on,"p"),space=0,border=NA)
    barplot(concateOnMeasurement(gin06gin06on,"q"),space=0,border=NA)
    barplot(concateOnMeasurement(gin06gin06on,"n"),space=0,border=NA)
    dev.new()
    layout(matrix(1:4,1,4),c(0.45,0.45,0.45,0.2))
    boxplot(gin06gin06$scores ~ gin06gin06$timeStep)
    barplot(concateOnMeasurement(gin06gin06,"p"),space=0,border=NA)
    barplot(concateOnMeasurement(gin06gin06,"q"),space=0,border=NA)
    barplot(concateOnMeasurement(gin06gin06,"n"),space=0,border=NA)
    dev.new()
    layout(matrix(1:4,1,4),c(0.45,0.45,0.45,0.2))
    boxplot(customgin06$scores ~ customgin06$timeStep)
    barplot(concateOnMeasurement(customgin06,"p"),space=0,border=NA)
    barplot(concateOnMeasurement(customgin06,"q"),space=0,border=NA)
    barplot(concateOnMeasurement(customgin06,"n"),space=0,border=NA)
    dev.new()
    ComparePricesQuantitiesScores(customgin06on)

    ############################
    ############################
    ############################




    MANU=read.csv("~/share_res/MANUALcustomgin06oncopymin.csv",sep=";")
    deb=read.csv("~/share_res/deb.csv",sep=";")
    ComparePricesQuantitiesScores(deb)
    mean(MANU$g0_p[MANU$p_good == "g0" & MANU$timeStep > 8000])  
    mean(MANU$g1_p[MANU$p_good == "g1" & MANU$timeStep > 8000])  
    mean(MANU$g2_p[MANU$p_good == "g2" & MANU$timeStep > 8000])  
    mean(MANU$g3_p[MANU$p_good == "g3" & MANU$timeStep > 8000])  
    mean(MANU$g4_p[MANU$p_good == "g4" & MANU$timeStep > 8000])  
    plot(MANU$g0_p[MANU$p_good == "g0" & MANU$timeStep > 8000])  

    aaaS=getAllMeanScore("~/share_res/6_cult-prod_init-rand_volsel-gin_ufun-gin/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodmanualTradGintisTakingNeedAccountCustomUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/Manual5CopymaxNeed/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodmanualTradGintisTakingNeedAccountCustomUitiliy.pdf")
    boxplot(aaaP,ylim=c(-1,1))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/Manual5CopymaxNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodmanualTradGintisTakingNeedAccountCustomUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/Manual5CopymaxNoNeedBIS/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodmanualTradGintisTakingNoNeedBISAccountCustomUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/Manual5CopymaxNoNeedBIS/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodmanualTradGintisTakingNoNeedBISAccountCustomUitiliy.pdf")
    boxplot(aaaP,ylim=c(-1,1))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/Manual5CopymaxNoNeedBIS/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodmanualTradGintisTakingNoNeedBISAccountCustomUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/Manual5CopymaxNoNeed/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodmanualTradGintisTakingNoNeedAccountCustomUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/Manual5CopymaxNoNeed/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodmanualTradGintisTakingNoNeedAccountCustomUitiliy.pdf")
    boxplot(aaaP,ylim=c(-1,1))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/Manual5CopymaxNoNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodmanualTradGintisTakingNoNeedAccountCustomUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/Manual5CopymaxGintisNoNeed/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodmanualTradGintisTakingNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/Manual5CopymaxGintisNoNeed/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodmanualTradGintisTakingNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaP,ylim=c(-1,1))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/Manual5CopymaxGintisNoNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodmanualTradGintisTakingNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/Manual5CopymaxGintisNeed/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodmanualTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/Manual5CopymaxGintisNeed/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodmanualTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaP,ylim=c(-1,1))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/Manual5CopymaxGintisNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodmanualTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/randn5CopymaxGintisNeed/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodrandnTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/randn5CopymaxGintisNeed/",type="p")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodrandnTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaP,ylim=c(-10,0))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/randn5CopymaxGintisNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodrandnTradGintisTakingNeedAccountGintisUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    aaaS=getAllMeanScore("~/share_res/randn5CopymaxGintisNoNeed/")
    pdf("~/projects/PhD/doc/thesis/images/Scores-300Ag5goodrandnTradGintisNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaS)
    dev.off()
    aaaP=getAllMeanDiff("~/share_res/randn5CopymaxGintisNoNeed/",type="p")
    deb=read.csv("~/share_res/randn5CopymaxGintisNoNeed/exp_1/agents.csv",sep=";")
    pdf("~/projects/PhD/doc/thesis/images/Prices-300Ag5goodrandnTradGintisNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaP,ylim=c(-10,0))
    dev.off()
    aaaQ=getAllMeanDiff("~/share_res/randn5CopymaxGintisNoNeed/",type="q")
    pdf("~/projects/PhD/doc/thesis/images/Quantities-300Ag5goodrandnTradGintisNoNeedAccountGintisUitiliy.pdf")
    boxplot(aaaQ)#,ylim=c(-5,5))
    dev.off()

    deb=read.csv("~/result/SnapchotModelJuly2017/1_cult-prod_init-man_volsel-gin_ufun-cust/exp_1/agents.csv",sep=";")

}

#check step by step how is changing the value of the column `colmn` for each agent
checChanges <- function(expe,colmn="g0_p"){
    plot(expe[,colmn] ~expe$timeStep ,type="n")
    abline(v=seq(0,1000,1),col="light grey")
    abline(v=seq(0,1000,3*15)+(3*15),col="red")
    sapply(levels(expe$agent),function(i){lines(expe[expe$agent == i,colmn] ~expe$timeStep[expe$agent == i] ,type="l",col=expe$p_good[expe$agent == i] )})

}

ComparePricesQuantitiesScores <- function(expe,event=F){
    colrs=brewer.pal(length(levels(expe$p_good)),"Set2")
    par(mar=c(3,3,2,2))
    layout(matrix(1:4,1,4),c(0.45,0.45,0.45,0.15))
    boxplot(expe$scores ~ expe$timeStep,main="score")
    if(event){
	par(new=T)
	plot(checkev$g0_n ~ checkev$timeStep,type="n",axes=F,ylab="",xlab="") 
	abline(v=seq(3*100*15,10000,3*15*100),col="red") 
    }
    barplot(concateOnMeasurement(expe,"p",fun="median"),space=0,border=NA,main="price",col=colrs)
    if(event){
	par(new=T)
	plot(checkev$g0_n ~ checkev$timeStep,type="n",axes=F,ylab="",xlab="") 
	abline(v=seq(3*100*15,10000,3*15*100),col="red") 
    }
    barplot(concateOnMeasurement(expe,"q",fun="median"),space=0,border=NA,main="quantities",col=colrs)
    if(event){
	par(new=T)
	plot(checkev$g0_n ~ checkev$timeStep,type="n",axes=F,ylab="",xlab="") 
	abline(v=seq(3*100*15,10000,3*15*100),col="red") 
    }
    barplot(concateOnMeasurement(expe,"n")[,c(1,ncol(concateOnMeasurement(expe,"n")))],space=0,border=NA,names.arg=rep("",2),main="ideal quantites",col=colrs)
    if(event){
	par(new=T)
	plot(checkev$g0_n ~ checkev$timeStep,type="n",axes=F,ylab="",xlab="") 
	abline(v=seq(3*100*15,10000,3*15*100),col="red") 
    }
}


##thsi funciton count the number of consumer with a positive value for each good
plotAgentsWithSmthg <- function(expe){

    plot(sapply( unique(expe$timeStep), function(t){length(expe[ "g0_q"  > 0 & expe$timeStep == t ,"agent"])}),type="n",ylim=c(0,length(levels(expe$agent))))
    colu=1:length(levels(expe$p_good))
    names(colu)=levels(expe$p_good)
    print(colu)

    sapply(levels(expe$p_good),function(g){
	   points(sapply( unique(expe$timeStep), function(tmstp){length(expe[ expe[,paste(g,"_q",sep="")] > 0 & expe$timeStep == tmstp & expe$p_good != g,"agent"])}),type="l",col= colu[g]) 
	   #points(sapply( unique(expe$timeStep), function(tmstp){length(expe[ 1-(expe[,paste(g,"_q",sep="")]/expe[,paste(g,"_n",sep="")]) < 0.001 & expe$timeStep == tmstp & expe$p_good != g,"agent"])}),type="l",col= colu[g]) 


})
}

agentWith <- function(expe){
    expe[is.na(expe)]=0
    sapply(levels(expe$p_good)[which(levels(expe$p_good) != "coins")],function(g){
	   sapply( unique(expe$timeStep), function(tmstp){length(expe[ expe[,paste(g,"_q",sep="")] >= 1 & expe$timeStep == tmstp & expe$p_good != g,"agent"])})
})

}

noIdea <- function(){
    paintmat=matrix(0,nrow=101,ncol=6)
    colnames(paintmat)=0:5
    rownames(paintmat)=1:101

    for(i in 1:101){print(paintmat[i,]);print(sumcount[[i]]);paintmat[i,names(sumcount[[i]])]=sumcount[[i]];}

}


#print the ratio with hoter col givne timestep
printCulture <- function(cul){
    heatcol=heat.colors(length(unique(cul$V1)))
    names(heatcol)=unique(cul$V1)
    plot(cul$V3 ~ cul$V5,col=heatcol[as.character(cul$V1)])
}

plotNumCopy<-function(cul){
    plot(names(table(cul$V1)),table(cul$V1),type="l",ylim=c(0,max(table(cul$V1))))
}



#this function pop up 3 windows with the evolution through time of various indicators 
#if tofile is tru then pdf of the graph are saved in the folder `tofilefolder`
print3IndicatorFromFolder <- function(folder,tofile=F,tofilefolder=""){

    Scores=getAllMeanScore(folder)
    if(tofile)pdf(file.path(tofilefolder,paste("Scores-",basename(folder),".pdf",sep="")))
    else dev.new()
    boxplot(Scores,main="Evolution of Scores through time")
    if(tofile)dev.off()
    Prices=getAllMeanDiff(folder,type="p")
    if(tofile)pdf(file.path(tofilefolder,paste("Prices-",basename(folder),".pdf",sep="")))
    else dev.new()
    boxplot(Prices,ylim=c(-1,1),main="Difference from ideal prices through time")
    if(tofile)dev.off()
    Quantities=getAllMeanDiff(folder,type="q")
    if(tofile)pdf(file.path(tofilefolder,paste("Quantities-",basename(folder),".pdf",sep="")))
    else dev.new()
    boxplot(Quantities,main="Difference from ideal quantites through time",ylim=c(-5,0))
    if(tofile)dev.off()
}


writeAllIndicatorFromSnapshotJulyResult <- function(){
    fl=   list.dirs("~/result/SnapchotModelJuly2017",recursive=F)
    fl=fl[1:24]
    sapply(fl,function(i){print3IndicatorFromFolder(i,tofile=T,tofilefolder="~/projects/PhD/doc/thesis/images/")})


    tableGraph=c()
    id=1
    foldname="/home/scarrign/projects/PhD/doc/thesis/images"
    indic=c("Scores","Prices","Quantities")
    for( ufun in c("cust","gin")){
	for( init in c("man","rand","randn")){
	    for(volsel in c("gin","gino")){
		for( cult in c("prod","integ")){
		    a=c(cult,init,volsel,ufun,paste("\\includegraphics[width=.10\\textwidth]{",file.path(foldname,paste(indic, paste(id,"_cult-",cult,"_init-",init,"_volsel-",volsel,"_ufun-",ufun,".pdf",sep=""),sep="-")),"}",sep=""))
		    tableGraph=	   rbind(tableGraph,a)
		    print(a)

		    id=id+1
		}
	    }
	}
    }

    colnames(tableGraph)=c("Copy","Init","Choice","Util",indic)
    print(xtable(tableGraph),include.rownames=F,sanitize.text.function=identity,file="test.tex")

    deb=read.csv("~/share_res/agents.csv",sep=";")

    unbal=read.csv("~/share_res/agentsUnbal.csv",sep=";")
    ComparePricesQuantitiesScores(unbal)
    logunbal=read.csv("~/share_res/logUnbal")
    firstep = logunbal[ logunbal$timestep == 1,]
    bal=read.csv("~/share_res/agentsBal.csv",sep=";")
    logbal=read.csv("~/share_res/logBal")

    checChanges(unbal)

    unbalbis=read.csv("~/share_res/agentsUnbalBis.csv",sep=";")
    logunbalbis=read.csv("~/share_res/logUnbalBis")
    logunbalbis=read.csv("~/share_res/trade.log")

    plot(tapply(logbal[,11],logbal$timestep,sum),type="l",ylim=c(0,8000))                                              
    whyFailedTrade(logbal)
    whyFailedTrade(logunbalbis)
    whyFailedTrade(logunbal)

    fail=read.csv("~/share_res/trade.log")
    whyFailedTrade(fail[fail$goodwanted == "g0",])
    whyFailedTrade(fail[fail$goodwanted == "g1",])
    whyFailedTrade(fail[fail$goodwanted == "g2",])
    whyFailedTrade(fail[fail$goodwanted == "g3",])

    whyFailedTrade(fail[fail$goodproposed == "g0",])
    whyFailedTrade(fail[fail$goodproposed == "g1",])
    whyFailedTrade(fail[fail$goodproposed == "g2",])
    whyFailedTrade(fail[fail$goodproposed == "g3",])
    unbalbis=unbal
    ComparePricesQuantitiesScores(unbal)
    ComparePricesQuantitiesScores(bal)
    ComparePricesQuantitiesScores(unbalbis)

    dev.new()
    plot(tapply(logunbal[,13],logunbal$timestep,sum),type="l",ylim=c(0,8000))                                              
    barplot(concateOnMeasurement(expe,"p",fun="median"),space=0,border=NA,main="price")
    countGoodPerSite=siteWithAmount(unbal)
}

##This function use a simple  log whil and count why the transact have failed
whyFailedTrade <- function(dat){
    ymax=max(sapply(11:15,function(i)tapply(!dat[,i],dat$timestep,sum)))
    plot(tapply(!dat[,11],dat$timestep,sum),type="n",ylim=c(0,ymax))
    sapply(11:15,function(i)lines(tapply(!dat[,i],dat$timestep,sum),col=i))
    legend("bottomleft",colnames(dat[,11:15]),col=11:15,lwd=1,cex=.8,bg="white")
}



plotGoodPerSite <- function(matrixGoodPerSite){
    par(xpd=TRUE)
    plot(matrixGoodPerSite[,1]~ rownames(matrixGoodPerSite),type="n",main="Number of different good type",bty="n",ylab="number of site",xlab="timestep",ylim=range(matrixGoodPerSite)) 
    clrs=rev(heat.colors(8))
    sapply(0:(ncol(matrixGoodPerSite)-1),function(i)lines( matrixGoodPerSite[,as.character(i)] ~ rownames(matrixGoodPerSite) ,csl=clrs[i+1],lwd=3))
    legend("top",legend=paste(0:(ncol(matrixGoodPerSite)-1),"different goods"),col=clrs,lwd=3,cex=.8)
    text(as.numeric(rownames(matrixGoodPerSite)[length(rownames(matrixGoodPerSite))])+10,matrixGoodPerSite[nrow(matrixGoodPerSite),],labels=paste(colnames(matrixGoodPerSite),"goods"),cex=.8,adj=0)
}

heatmapGoodPerSite <- function(matrixGoodPerSite){
    barplot(t(matrixGoodPerSite),legend=paste(0:(ncol(matrixGoodPerSite)-1),"different goods"),space=0,border=NA,main="Number of different good per site",ylab="number of sites",args.legend=list(x="right",xpd=T,cex=.8,bg="white"),col=rev(heat.colors(ncol(matrixGoodPerSite))))
}

siteWithAmount <- function(expe){
    allCount=	matrix(nrow=length(unique(expe$timeStep)),ncol=length(levels(expe$p_good)),0)
    colnames(allCount)=0:(length(levels(expe$p_good))-1)
    rownames(allCount)=unique(expe$timeStep)
    for( tstep in unique(expe$timeStep)){
	res=c()
	for( ag in unique(expe$agent)){
	    res=c(res,sum(sapply(levels(expe$p_good)[which(levels(unbal$p_good) != "coins")],function(g)expe[  expe$timeStep == tstep & expe$agent == ag,paste(g,"_q",sep="")] >0)))
	}
	res=table(res)
	allCount[as.character(tstep),names(res)]=res
    }
    return(allCount)

}

siteWithAmount <- function(expe){
    ###small warnigns, in this function, the output matrix use the number od levels(p_good) as we considered coins aren't counted.
    goodCounted=grep("g.*_q",colnames(expe))  ##we will count good with that ar not coins
    res=matrix(ncol=length(levels(expe$p_good)),nrow=length(unique(expe$timeStep)),0)
    colnames(res)=0:(length(levels(expe$p_good))-1)
    rownames(res)=unique(expe$timeStep)
    gpa=apply(expe,1,function(i)sum(as.numeric(i[goodCounted])>0))
    goodPerAgent=matrix(gpa,nrow=length(unique(expe$timeStep)),ncol=length(unique(expe$agent)),byrow=T) ##for each time step, count number of good with positiv enumber 
    allcount= apply(goodPerAgent,1,table)

    for(i in 1:length(allcount)){
	res[i,names(allcount[[i]])]=allcount[[i]]
    }
    return(res)


}


heatmapSiteWithGood <- function(expe){
    countSite=agentWith(expe)
    rownames(countSite)=unique(expe$timestep)
    colrs=brewer.pal(length(colnames(countSite)),"Set2")
    barplot(t(countSite),legend=colnames(countSite),space=0,border=NA,main="Number of sites with good type",args.legend=list(x="topleft"),col=colrs)

}

plotSiteWithGood <- function(matrixGoodPerSite,g=NA,...){
    if(is.na(g)){
	clrs=brewer.pal(ncol(matrixGoodPerSite),"Set2")
	names(clrs)=colnames(matrixGoodPerSite)
	par(xpd=NA)
	plot(1:nrow(matrixGoodPerSite),matrixGoodPerSite[,1] ,type="n",main="Number of sites with good type",ylim=range(matrixGoodPerSite),bty="n",ylab="number of site",xlab="timestep",...) 
	sapply(colnames(matrixGoodPerSite),function(i)lines(1:nrow(matrixGoodPerSite), matrixGoodPerSite[,i]   ,col=clrs[i],lwd=3))
	legend("bottomright",legend=colnames(matrixGoodPerSite),col=clrs,lwd=3,cex=.8)
	text(nrow(matrixGoodPerSite)+.2,matrixGoodPerSite[nrow(matrixGoodPerSite),],labels=paste(colnames(matrixGoodPerSite)),cex=.8,adj=0)
    }
    else{
	clrs=brewer.pal(ncol(matrixGoodPerSite),"Set2")
	names(clrs)=colnames(matrixGoodPerSite)
	par(xpd=NA)
	plot(1:nrow(matrixGoodPerSite),matrixGoodPerSite[,1] ,type="n",main="Number of sites with good type",ylim=range(matrixGoodPerSite),bty="n",ylab="number of site",xlab="timestep") 
	lines(1:nrow(matrixGoodPerSite), matrixGoodPerSite[,g]   ,col=clrs[g],lwd=3)
	legend("bottomright",legend=colnames(matrixGoodPerSite),col=clrs,lwd=3,cex=.8)
	text(nrow(matrixGoodPerSite)+.2,matrixGoodPerSite[nrow(matrixGoodPerSite),g],labels=paste(colnames(matrixGoodPerSite))[g],cex=.8,adj=0)
    }

}

SimppleComAugust2017 <- function(){

    unbal=read.csv("~/share_res/testEventNormalMut.csv",sep=";")
    allCount1=siteWithAmount(unbal)
    unbal[unbal == -1] = 0
    ComparePricesQuantitiesScores(unbal)
    dev.new()
    heatmapGoodPerSite(allCount1)
    dev.new()
    plotGoodPerSite(allCount1)

    dev.new()
    unbal2n=read.csv("~/share_res/agentsShorterCycle.csv",sep=",")
    write.csv(unbal2,"~/share_res/agentsShorterCycle.csv")
    allCount2=siteWithAmount(unbal2)
    unbal2[unbal2 == -1] = 0
    ComparePricesQuantitiesScores(unbal2)
    dev.new()
    heatmapGoodPerSite(allCount2)
    dev.new()
    plotGoodPerSite(allCount2)
    unbal3=read.csv("~/share_res/agentsShorterCycleSmallMu.csv",sep=";")
    allCount3=siteWithAmount(unbal3)
    unbal3[unbal3 == -1] = 0
    ComparePricesQuantitiesScores(unbal3)
    dev.new()
    heatmapGoodPerSite(allCount3)
    dev.new()
    plotGoodPerSite(allCount2)
    aaaS=getAllMeanScore("~/share_res/checkMult/")
    boxplot(aaaS)
    aaaP=getAllMeanDiff("~/share_res/checkMult/",type="p")
    boxplot(aaaP,ylim=c(-1,1))
    aaaQ=getAllMeanDiff("~/share_res/checkMult/",type="q")
    boxplot(aaaQ)#,ylim=c(-5,5))

    aaaS=getAllMeanScore("~/share_res/checkMultLong/")
    boxplot(aaaS)
    aaaP=getAllMeanDiff("~/share_res/checkMultLong/",type="p")
    boxplot(aaaP,ylim=c(-1,1))
    aaaQ=getAllMeanDiff("~/share_res/checkMultLong/",type="q")
    boxplot(aaaQ)#,ylim=c(-5,5))

    aaaS=getAllMeanScore("~/share_res/checkNoEventBIS9000/")
    boxplot(aaaSjjhh)
    aaaP=getAllMeanDiff("~/share_res/checkNoEventBIS9000/",type="p")
    boxplot(aaaP,ylim=c(-1,1))
    aaaQ=getAllMeanDiff("~/share_res/checkNoEvent9000/",type="q")
    boxplot(aaaQ)#,ylim=c(-5,5))

    colrs=brewer.pal(length(colnames(ceramics)),"Set2")
    barplot(t(ceramics),legend=colnames(ceramics),space=0,border=NA,main="Number of sites with the different goods",args.legend=list(x="topleft"),col=colrs)


    ####Check Equilibrium 
    pdf(file.path(tofilefolder,"checkEquilLong.pdf"),width=21)
    ComparePricesQuantitiesScores(unbal)
    dev.off()
    pdf(file.path(tofilefolder,"checkEquilShort.pdf"),width=21)
    ComparePricesQuantitiesScores(unbal2)
    dev.off()
    pdf(file.path(tofilefolder,"checkEquilShortLowMu.pdf"),width=21)
    ComparePricesQuantitiesScores(unbal3)
    dev.off()

    ####number of site with goods N\in{Set of Goods}
    pdf(file.path(tofilefolder,"hmNbSiteWGoodN.pdf"),width=25)
    layout(matrix(1:3,1,3),c(0.45,0.45,0.45))
    barplot(t(ceramics),legend=colnames(ceramics),space=0,border=NA,main="Number of sites with the different goods",args.legend=list(x="topleft"),col=colrs)
    heatmapSiteWithGood(good)   
    heatmapSiteWithGood(bad)   
    dev.off()

    pdf(file.path(tofilefolder,"hmNbSiteWGoodData.pdf"))
    barplot(t(ceramics),legend=colnames(ceramics),space=0,border=NA,main="Number of sites with the different goods",args.legend=list(x="topleft"),col=colrs)
    dev.off()


    pdf(file.path(tofilefolder,"plotNbSiteWGoodData.pdf"))
    plotSiteWithGood(ceramics)
    dev.off()

    pdf(file.path(tofilefolder,"plotNbSiteWGoodN.pdf"),width=25)
    layout(matrix(1:3,1,3),c(0.45,0.45,0.45))
    plotSiteWithGood(ceramics)   
    matrixGoodPerSite=agentWith(good)
    rownames(matrixGoodPerSite)=unique(good$timeStep)
    plotSiteWithGood(matGood)   
    matrixGoodPerSite=agentWith(bad)
    rownames(matrixGoodPerSite)=unique(bad$timeStep)
    plotSiteWithGood(matBad)   
    dev.off()


    ##Number of different goods per site
    unbal[unbal == -1] = 0
    allCount1=siteWithAmount(unbal)
    allCount2=siteWithAmount(unbal2)
    unbal2[unbal2 == -1] = 0
    allCount3=siteWithAmount(unbal3)
    unbal3[unbal3 == -1] = 0
    pdf(file.path(tofilefolder,"hmNbGoodPerSite.pdf"),width=21)
    layout(matrix(1:3,1,3),c(0.45,0.45,0.45))
    heatmapGoodPerSite(allCount1)
    heatmapGoodPerSite(allCount2)
    heatmapGoodPerSite(allCount3)
    dev.off()
    pdf(file.path(tofilefolder,"plotNbGoodPerSite.pdf"),width=21)
    layout(matrix(1:3,1,3),c(0.45,0.45,0.45))
    plotGoodPerSite(allCount1)
    plotGoodPerSite(allCount2)
    plotGoodPerSite(allCount3)
    dev.off()

    tofilefolder="~/projects/PhD/doc/thesis/images/"

    aaaSu=getAllMeanScore("~/share_res/unbalanceT/")
    aaaPu=getAllMeanDiff("~/share_res/unbalanceT/",type="p")
    aaaQu=getAllMeanDiff("~/share_res/unbalanceT/",type="q")

    boxplot(aaaSu,ylim=c(0,30))
    boxplot(aaaS,ylim=c(0,30))
    boxplot(aaaS1000,ylim=c(0,30))
    boxplot(aaaPu,ylim=c(-1,1.5))
    boxplot(aaaP1000,ylim=c(-1,1.5))
    boxplot(aaaQu,ylim=c(-1,1))
    boxplot(aaaQ1000,ylim=c(-1,1))

    aaaS=getAllMeanScore("~/share_res/balanceT/")
    aaaP=getAllMeanDiff("~/share_res/balanceT/",type="p")
    aaaQ=getAllMeanDiff("~/share_res/balanceT/",type="q")

    aaaS1000=getAllMeanScore("~/share_res/balanceT1000/")
    aaaP1000=getAllMeanDiff("~/share_res/balanceT1000/",type="p")
    aaaQ1000=getAllMeanDiff("~/share_res/balanceT1000/",type="q")

    testMu=getAllMeanScore("~/result/testMu/10_mu0.01-mumax0.1/")
    testMu=getAllMeanScore("~/result/testMu/1_mu0.001-mumax0.01/")
    aaaP1000=getAllMeanDiff("~/share_res/balanceT1000/",type="p")
    aaaQ1000=getAllMeanDiff("~/share_res/balanceT1000/",type="q")

    aaaS1000=getAllMeanScore("~/result/testMuNeed-0.2/16_mu0.05-mumax10/")
    aaaP1000=getAllMeanDiff( "~/result/testMuNeed-0.2/16_mu0.05-mumax10/",type="p")
    aaaQ1000=getAllMeanDiff ("~/result/testMuNeed-0.2/16_mu0.05-mumax10/",type="q")

    aaaQ1000=getAllMeanDiff ("~/result/testMu/1_mu0.001-mumax0.01/",type="q")

    plot(sort(as.numeric(sapply(list.dirs("~/result/testMuNeed-0.2/",recursive=F),function(fold){ getProp(file.path(fold,"config.xml"),"controller//culture","mutation")}))))

    mutmum=c("mutation","mumax")
    allscoresN02=getAllLastresults("~/result/testMuNeed-0.2/","s",mutmum)
    allquantitiesN02=getAllLastresults("~/result/testMuNeed-0.2/","q",mutmum)
    allpricesN02=getAllLastresults("~/result/testMuNeed-0.2/","p",mutmum)

    allscoresN1=getAllLastresults("~/result/testMu/","s",mutmum)
    allquantitiesN1=getAllLastresults("~/result/testMu/","q",mutmum)
    allpricesN1=getAllLastresults("~/result/testMu/","p",mutmum)

    allscoresCopy=getAllLastresults("~/result/testCopy/","s")
    allquantitiesCopy=getAllLastresults("~/result/testCopy/","q")
    allpricesCopy=getAllLastresults("~/result/testCopy/","p")

    allscoresCopyMN=getAllLastresults("~/result/testCopyMutEvents/","s")
    allquantitiesCopyMN=getAllLastresults("~/result/testCopyMutEvents/","q")
    allpricesCopyMN=getAllLastresults("~/result/testCopyMutEvents/","p")

    ####PLot three 3D graphs with the score/quantities/prices at the end and the assocaited boxplot for an easier reading
    ##score
    pdf(file.path(foldname,"ScoreMuCopyAll.pdf"))
    wireframing(allscoresCopyM,col=NA,main="Score")                                                                   
    #mtext("Score of the agents (timestep=last)",3)
    dev.off()

    pdf(file.path(foldname,"ScoreMuAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allscoresCopyMN[,"s"] ~ allscoresCopyMN[,"mutation"],ylab="score",xlab="mutation")     
    dev.off()

    pdf(file.path(foldname,"ScoreCopyAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allscoresCopyM[,"s"] ~ allscoresCopyM[,"copy"],ylab="score",xlab="copy")     
    dev.off()

    ##prices
    pdf(file.path(foldname,"PriceMuCopyAll.pdf"))
    wireframing(allpricesCopyM,col=NA,main="Price")                                                                   
    #mtext("Price of the agents (timestep=last)",3)
    dev.off()

    pdf(file.path(foldname,"PriceMuAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allpricesCopyM[,"p"] ~ allpricesCopyM[,"mutation"],ylab="price",xlab="mutation")     
    dev.off()

    pdf(file.path(foldname,"PriceCopyAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allpricesCopyMN[,"p"] ~ allpricesCopyMN[,"copy"],ylab="price",xlab="copy")     
    dev.off()

    ##quantities
    pdf(file.path(foldname,"QuantitieMuCopyAll.pdf"))
    par(mar=rep(0,4))
    wireframing(allquantitiesCopyM,col=NA,main="Quantitie")                                                                   
    #mtext("Quantitie of the agents (timestep=last)",3)
    dev.off()

    pdf(file.path(foldname,"QuantitieMuAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allquantitiesCopyM[,"q"] ~ allquantitiesCopyM[,"mutation"],ylab="quantity",xlab="mutation")     
    dev.off()

    pdf(file.path(foldname,"QuantitieCopyAll.pdf"),width=5,height=3)
    par(mar=c(4,4,1,1))
    boxplot(allquantitiesCopyM[,"q"] ~ allquantitiesCopyM[,"copy"],ylab="quantity",xlab="copy")     
    dev.off()

    ###############3



    ###plot the changes in proportion

    allCount=sapply(list.dirs("/home/scarrign/result/testCopyMutEvents/",recursive=F),function(fold){
		    tmpCount=getAllSiteWithAmount(fold)
		    meanTmp=getStatFromAll(tmpCount)
		    png(paste("heatMap-",basename(fold),".pdf",sep=""))
		    heatmapGoodPerSite(meanTmp)
		    dev.off()
		    png(paste("plot-",basename(fold),".pdf",sep=""))
		    plotGoodPerSite(meanTmp)
		    dev.off()
		    return(tmpCount)
})

    sapply(colnames(allCount),function(tmp){
	   meanTmp=getStatFromAll(allCount[,tmp],sd)
	   print(names(meanTmp))
	   png(paste(foldname,"/heatmap/heatMap-",basename(tmp),"-SD.png",sep=""))
	   heatmapGoodPerSite(meanTmp)
	   dev.off()
	   png(paste(foldname,"/plot/plot-",basename(tmp),"-SD.png",sep=""))
	   plotGoodPerSite(meanTmp)
	   dev.off()
})

    ng=5
    tst=9000
    ex="1_mu0.001-copy0.0001"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=1
    tst=9000
    ex="1_mu0.001-copy0.0001"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()


    ng=5
    tst=9000
    ex="6_mu0.001-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=1
    tst=9000
    ex="6_mu0.001-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()


    ng=5
    tst=9000
    ex="12_mu0.01-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=1
    tst=9000
    ex="12_mu0.01-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()


    ng=5
    tst=9000
    ex="7_mu0.01-copy0.0001"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=1
    tst=9000
    ex="7_mu0.01-copy0.0001"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=5
    tst=9000
    ex="12_mu0.01-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()

    ng=1
    tst=9000
    ex="12_mu0.01-copy10"
    png(file.path(foldname,paste("distrib-",ng,"goods-",tst,"timstep-exp",ex,".png",sep="")))
    distribSimu(allCount,ng,tst,ex)
    dev.off()
}


distribSimu <- function(allsim,ngoods,timestep,simname)hist(allCount[, simname][[as.character(ngoods)]][as.character(timestep),],xlim=c(0,1000),xlab=paste("number of site ending with",ngoods,"goods after",timestep,"timestep"),main=simname,ylab="number of simulations",freq=T)



bplot <- function(dat) boxplot( dat[dat[,1]==0.002,3] ~ dat[dat[,1]==0.002,2])


wireframing <- function(dat,...){
    yval=sort(unique(dat[,2]))
    yname=colnames(dat)[2]
    zval=sort(unique(dat[,3]))
    zname=colnames(dat)[3]
    xval=sort(unique(dat[,1]))
    xname=colnames(dat)[1]
    yargs=list(at=log(yval),labels=yval)
    xargs=list(at=log(xval),labels=xval)
    wireframe(dat[,3] ~ log(dat[,1]) * log(dat[,2]),drape=T,scales=list(arrows=F,y=yargs,x=xargs),ylab=yname,xlab=xname,zlab=zname,shade=T,colorkey=F) #ylim=c(log(range(allquantities[,2])[2])-0.04*log(range(allquantities[,2])[2])*-1,log(range(allquantities[,2])[2])-0.04*log(range(allquantities[,2])[2])*-1))

}

##return the last result. Can be "s" for the score, "q" for the mean quantities or "p" for the mean prices
getAllLastresults <- function(folder,type="s",prop=c("mutation","copy")){
    alllastres=c()
    for( fold in list.dirs(folder,recursive=F)){

	try({
	    prop1=getProp(file.path(fold,"config.xml"),"controller//culture",prop[1])
	    prop2=getProp(file.path(fold,"config.xml"),"controller//culture",prop[2])
	    print(paste("Get simulation for",prop[1],"=",prop1,"and", prop[2],"=",prop2))
	    if(type == "s")
		tmpS=getAllMeanScore(fold)
	    if(type == "p")
		tmpS=getAllMeanDiff(fold,type="p")
	    if(type == "q")
		tmpS=getAllMeanDiff(fold,type="q")
	    tmpS=tmpS[,ncol(tmpS)]
	    alllastres=rbind(alllastres,c(prop1,prop2,mean(tmpS)))
	})
    }
    colnames(alllastres)=c(prop[1],prop[2],type)
    return(alllastres)
}



##Trying to put some horizontal plane
stufwith3D <- function(){
    panel.3d.contour <- function(x, y, z, rot.mat, distance, nlevels = 20, zlim.scaled, ...) { 
	add.line <- trellis.par.get("add.line") 
	panel.3dwire(x, y, z, rot.mat, distance, zlim.scaled = zlim.scaled, ...) 
	clines <- contourLines(x, y, matrix(z, nrow = length(x), byrow = TRUE), nlevels = nlevels) 
	for (ll in clines) { 
	    m <- ltransform3dto3d(rbind(ll$x, ll$y, zlim.scaled[2]), rot.mat, distance) 
	    panel.lines(m[1,], m[2,], col = add.line$col, lty = add.line$lty, lwd = add.line$lwd) 
	}
    } 
    cl<- 54.1
    age<- 10:80
    wt<- 25:160
    sim<- expand.grid(age = age,wt = wt)

    sim$cl<- cl*(sim$wt/70)**0.412 * (sim$age/50)**0.152

    print(cloud(cl~wt*age, data = sim))
}


persping <- function(dat,op=10,...){
    persp3d(x=log(as.numeric(rownames(dat))),log(as.numeric(colnames(dat))),dat,col=topo.colors(nrow(dat)*ncol(dat))[rank(dat)],scale=T,...)
    opt=dat[,]
    opt[,]=op
    surface3d(x=log(as.numeric(rownames(dat))),y=log(as.numeric(colnames(dat))),z=opt,col="red") 
}


toMat <- function(dat){
    mtx <- matrix(NA, nrow=length(unique(dat[,1])), ncol=length(unique(dat[,2])) )
    dimnames(mtx)=list(sort(unique(dat[,1])),sort(unique(dat[,2])))
    for( i in 1:nrow(dat)){
	mtx[as.character(dat[i,1]),as.character(dat[i,2])]=dat[i,3]
    }
    return(mtx)
}


#x=1:10
#y=21:30
#z=51:60
#
#plot3d(x,y,z, type='s')
#plot3d(x,y,z, type='h',add=TRUE)



getAllSiteWithAmount <- function(fold){
    nums=getProp(file.path(fold,"/config.xml"),"numSteps","value")
    ser=getProp(file.path(fold,"/config.xml"),"numSteps","serializeResolution")
    print(nums)
    print(ser)
    print(nums/ser+1)
    numStep=nums/ser+1  #actual number of step
    res=sapply(0:4,function(i)matrix(nrow=numStep,ncol=0))
    ##res will be a list of matrices, the length of res represent the number of different good the site can have
    ##thus 0 and 4 should be computed using the config file of the folder `fold` to be portable

    ##the number of row represent the number of timestep, number of column will be the number of experience, the length of res
    names(res)=0:4
    for ( i in list.dirs(fold,recursive=F) ){
	print(paste("get site with num good from expe ",i))
	this=siteWithAmount(read.csv(file.path(i,"agents.csv"),sep=";"))
	for(num in colnames(this)){ ##each column of `this` represent change throuhg time of the number of site with `num` good
	    res[[num]]=cbind(res[[num]],this[,as.character(num)])
	}
    }
    return(res)
}

getStatFromAll <- function(allexp,fun=mean){
    sapply(allexp,function(i)apply(i,1,fun))    
}


lowco=getAllSiteWithAmount("/home/scarrign/result/testCopy/1_mu0.002-copy0.0001/")
highco=getAllSiteWithAmount("/home/scarrign/result/testCopy/6_mu0.002-copy10/")
lowcohmu=getAllSiteWithAmount("/home/scarrign/result/testCopy/13_mu0.006-copy0.0001/")
highcohmu=getAllSiteWithAmount("/home/scarrign/result/testCopy/18_mu0.006-copy10/")

lowco=getAllSiteWithAmount("/home/scarrign/result/testCopyMutEvents/1_mu0.001-copy0.0001/")
highco=getAllSiteWithAmount("/home/scarrign/result/testCopyMutEvents/6_mu0.001-copy10/")



apply(sdL,2,lines)
apply(sdH,2,lines,col="red")
meanL=getStatFromAll(lowco)
meanH=getStatFromAll(highco)
sdL=getStatFromAll(lowco,sd)
seL=getStatFromAll(lowco,se)
meanH=getStatFromAll(highco)
sdH=getStatFromAll(highco,sd)
seH=getStatFromAll(highco,se)

meanL2=getStatFromAll(lowcohmu)
sdL2=getStatFromAll(lowcohmu,sd)
seL2=getStatFromAll(lowcohmu,se)
meanH2=getStatFromAll(highcohmu)
sdH2=getStatFromAll(highcohmu,sd)
seH2=getStatFromAll(highcohmu,se)

apply(meanL,2,lines)    
apply(meanH2,2,lines,col="red")    
apply(meanA+seA,2,lines,lty=2)    
apply(meanA-seA,2,lines,lty=2)    

getStatFromAll(aaa)-getStatFromAll(aaa,sd)


timingTest <- function(){

    ptm <- proc.time()
    aab=siteWithAmount(deb)
    print(proc.time()-ptm)

    a=c()
    b=c()
    for(i in 1:1000){
	ptm <- proc.time()
	aaa=siteWithAmount2(deb)
	tmp=print(proc.time()-ptm)
	b=c(b,tmp[1])
	ptm <- proc.time()
	aac=siteWithAmount3(deb)
	tmp=print(proc.time()-ptm)
	a=c(a,tmp[1])
    }
}


graphAarhus <- function(){

    ##plot two curves and area between them:

    oneExp=read.csv("~/result/testCopyMutEvents/20_mu1-copy0.001/exp_1/agents.csv",sep=";")
    best=read.csv("~/result/testCopyMutEvents/19_mu1-copy0.0001/exp_25/agents.csv",sep=";")
    good=read.csv("~/result/testCopyMutEvents/20_mu1-copy0.001/exp_14/agents.csv",sep=";") ##Score=30.3904
    worst=read.csv("~/result/testCopyMutEvents/5_mu0.001-copy1/exp_36/agents.csv",sep=";")
    bad=read.csv("~/result/testCopyMutEvents/6_mu0.001-copy10/exp_46/agents.csv",sep=";")##41.1844
    siteWith=agentWith(oneExp)
    matGood=apply(agentWith(good)/5,2,getChunk,6)
    plotSiteWithGood(matGood ) 
    matBad=apply(agentWith(bad)/5,2,getChunk,6)
    plotSiteWithGood(matBad ) 
    plotSiteWithGood(apply(agentWith(worst)/5,2,getChunk,6) ) 
    #apply(ceramics,1,function(i)
    g=4
    ei=siteWith[,g]
    plot(i,col=colrs[g],lwd=3,type="l",ylim=c(0,200))  
    chuncked=getChunk(ei,nrow(ceramics))/5
    lines(chuncked,lwd=3,lty=3,col=colrs[g])    
    polygon(c(1:length(i),rev(1:length(i))),c(i,rev(chuncked)),col=colrs[g],density=20)
    #polygon(1:length(i),pmax(i,chuncked),col=colrs[g],density=20)


    ##Build heatmap of simulation with score <35
    allCount=sapply(list.dirs("/home/scarrign/result/testCopyMutEvents/",recursive=F),function(fold){
		    getMeanDistFold(fold)
	})

    comp=cbind(apply(allCount,2,function(i)length(i[i<35])),apply(allCount,2,mean)) 
    matParam=matrix(0,ncol=6,nrow=4) 
    colnames(matParam)=c(0.0001,0.001,0.01,0.1,1,10)
    rownames(matParam)=c(0.001,0.01,0.1,1)
    for(i in rownames(comp)){

	mu=getProp(file.path(i,"config.xml"),"controller//culture","mutation")
	lambda=getProp(file.path(i,"config.xml"),"controller//culture","copy")
	matParam[as.character(mu),as.character(lambda)] <- comp[i,1]
    }

    tofilefolder="~/projects/PhD/doc/thesis/201710_presentationAarhus/images/"
    pdf(file.path(tofilefolder,"heatmapSimu.pdf"))
    m=matParam
    image(1:ncol(m), 1:nrow(m), t(m), col =  colorRampPalette(c("blue", "green"))(n = 100), axes = FALSE,ylab="",xlab="")
    box()
    axis(3, 1:ncol(m), colnames(m),lwd=0,lwd.ticks=1)
    axis(2, 1:nrow(m), rownames(m),lwd=0,lwd.ticks=1)
    for (x in 1:ncol(m))
	for (y in 1:nrow(m))
	    text(x, y, m[y,x],cex=1.5,col="white")
    mtext(expression(lambda),3,2)
    mtext(expression(mu),2,2)
    dev.off()

    pdf(file.path(tofilefolder,"heatmapSimuProba.pdf"),pointsize=22)
    m=matParam
    image(1:ncol(m), 1:nrow(m), t(m), col =  colorRampPalette(c("blue", "green"))(n = 100), axes = FALSE,ylab="",xlab="")
    box()
    axis(3, c(1,2,3.5,ncol(m)-1,ncol(m)), c("--","-","...","+","++"),lwd=0,lwd.ticks=0,line=-1)
    axis(2, c(1,2,nrow(m)-1,nrow(m)), c("--","-","+","++"),lwd=0,lwd.ticks=0,line=-1)
    for (x in 1:ncol(m))
	for (y in 1:nrow(m))
	    text(x, y, paste( (m[y,x]/50)*100, "%"),cex=.8,col="white")
    mtext("Openness",3,2)
    mtext("Creativity",2,2)
    dev.off()

    pdf(file.path(tofilefolder,"plotGoodSimu.pdf"))
    plotSiteWithGood(matGood ) 
    dev.off()
    pdf(file.path(tofilefolder,"plotBadSimu.pdf"))
    plotSiteWithGood(matBad ) 
    dev.off()
    pdf(file.path(tofilefolder,"plotData.pdf"))
    plotSiteWithGood(ceramics) 
    dev.off()

    pdf(file.path(tofilefolder,"hmGoodSimu.pdf"))
    heatmapSiteWithGood(good) 
    dev.off()
    pdf(file.path(tofilefolder,"hmBadSimu.pdf"))
    heatmapSiteWithGood(bad) 
    dev.off()
    pdf(file.path(tofilefolder,"hmData.pdf"))
    barplot(t(ceramics),legend=colnames(ceramics),space=0,border=NA,main="Number of sites with the different goods",args.legend=list(x="topleft"),col=colrs)
    dev.off()


    for(g in 1:5){
	i=ceramics[,g]
	pdf(file.path(tofilefolder,paste("plotGoodSimu",g,"A.pdf",sep="")))
	plotSiteWithGood(matGood,g)
	dev.off()
	pdf(file.path(tofilefolder,paste("plotGoodSimu",g,"B.pdf",sep="")))
	plotSiteWithGood(matGood,g)
	lines(i,lwd=3,lty=2,col=colrs[g])    
	dev.off()
	pdf(file.path(tofilefolder,paste("plotGoodSimu",g,"C.pdf",sep="")))
	plotSiteWithGood(matGood,g)
	lines(i,lwd=3,lty=2,col=colrs[g])    
	polygon(c(1:length(i),rev(1:length(i))),c(i,rev(matGood[,g])),col=colrs[g],density=20)
	dev.off()

	pdf(file.path(tofilefolder,paste("plotBadSimu",g,"A.pdf",sep="")))
	plotSiteWithGood(matBad,g)
	dev.off()
	pdf(file.path(tofilefolder,paste("plotBadSimu",g,"B.pdf",sep="")))
	plotSiteWithGood(matBad,g)
	lines(i,lwd=3,lty=2,col=colrs[g])    
	dev.off()
	pdf(file.path(tofilefolder,paste("plotBadSimu",g,"C.pdf",sep="")))
	plotSiteWithGood(matBad,g)
	lines(i,lwd=3,lty=2,col=colrs[g])    
	polygon(c(1:length(i),rev(1:length(i))),c(i,rev(matBad[,g])),col=colrs[g],density=20)
	dev.off()

	pdf(file.path(tofilefolder,paste("plotData",g,".pdf",sep="")))
	plotSiteWithGood(ceramics,g) 
	dev.off()
    }

    pdf(file.path(tofilefolder,paste("plotGoodSimuEnd.pdf",sep="")))
    plotSiteWithGood(matGood)
    for(g in 1:5){
	i=ceramics[,g]
	lines(i,lwd=3,lty=2,col=colrs[g])    
	polygon(c(1:length(i),rev(1:length(i))),c(i,rev(matGood[,g])),col=colrs[g],density=20)
    }
    dev.off()

    pdf(file.path(tofilefolder,paste("plotBadSimuEnd.pdf",sep="")))
    plotSiteWithGood(matBad)
    for(g in 1:5){
	i=ceramics[,g]
	lines(i,lwd=3,lty=2,col=colrs[g])    
	polygon(c(1:length(i),rev(1:length(i))),c(i,rev(matBad[,g])),col=colrs[g],density=20)
    }
    dev.off()



    ###distribution of score amoung all simu
    pdf(file.path(tofilefolder,"histScore.pdf"))
    hist(allCount,ylab="num simulations",xlab="Distance to data",main="")
    dev.off()

    tiff(file.path(tofilefolder,"histScoreGB.tiff"))
    hist(allCount,ylab="num simulations",xlab="Distance to data")
    abline(v=30.39,col="blue")
    abline(v=41.181,col="red")
    text(41.191,150,"Score expB",col="red")   
    text(30.39,150,"Score expG",col="blue")     
    dev.off()



    mean(apply(abs(ceramics - apply(siteWith,2,function(i)getChunk(i,nrow(ceramics))/5)),2,mean))

    ###Compute distance
    #allCount=sapply(list.dirs("/home/scarrign/result/testCopyMutEvents/",recursive=F),function(fold){
    #tmpCount=getAllSiteWithAmount(fold)
    #meanTmp=getStatFromAll(tmpCount)
    #png(paste("heatMap-",basename(fold),".pdf",sep=""))
    #heatmapGoodPerSite(meanTmp)
    #dev.off()
    #png(paste("plot-",basename(fold),".pdf",sep=""))
    #plotGoodPerSite(meanTmp)
    #dev.off()
    #return(tmpCount)
    #})



    png("elibind/numsiteBis.png")
    plotSiteWithGood(agentWith(deb))
    dev.off()

    png("elibind/scoreBis.png")
    plot(deb$score ~ deb$timeStep,col=deb$p_good)
    dev.off()

    png("elibind/out0Bis.png")
    plot(deb$g0_q ~ deb$timeStep,col=deb$p_good)
    dev.off()
    png("elibind/out1Bis.png")
    plot(deb$g1_q ~ deb$timeStep,col=deb$p_good)
    dev.off()
    png("elibind/out2Bis.png")
    #plot(deb$g2_q ~ deb$timeStep,col=deb$p_good)
    dev.off()

    png("elibind/pout0Bis.png")
    plot(deb$g0_p ~ deb$timeStep,col=deb$p_good)
    dev.off()
    png("elibind/pout1Bis.png")
    plot(deb$g1_p ~ deb$timeStep,col=deb$p_good)
    dev.off()
    png("elibind/pout2Bis.png")
    #plot(deb$g2_p ~ deb$timeStep,col=deb$p_good)
    dev.off()
}



##take a vector `dat` and un integer `nchunk` in entry
##this function return a vector of length `nchunk` where hich elemetn is the mean of the element of the corresponding chunk of `dat` 
getChunk <- function(dat,nchunk){sapply(split(dat,ceiling(seq_along(dat)/(length(dat)/nchunk))),mean)}

##take a vector matrix `expe` in entry
##reduce the number of timestep byt taking the mean
chunkExpe  <- function(expe,nchunk)apply(expe,2,function(i)getChunk(i,nchunk))


##compute A - B with A the matrix for the data for all good an b 
distToData  <-  function(expe,dat)abs(dat-chunkExpe(expe,nrow(dat)))

#return the cumulate distance by good for one experiment
meanDist <- function(expe,dat,fun=sum)apply(distToData(expe,dat),2,fun)

##get the mean distance of all exp for one set of parameters
getMeanDistFold  <-  function(foldExp){
    sapply( list.dirs(foldExp,recursive=F),function(fold){
	   print(paste("compute dist for",fold)) 
	   siteWith=agentWith(read.csv(file.path(fold,"agents.csv"),sep=";"))
	   mean(meanDist(siteWith/5,ceramics),fun=sum)
	})
}




newTest <- function(){
    ulu=getAllMeanScore("~/result/testCopyMutEvents/1_mu0.001-copy0.0001/")
    ulu=getAllMeanScore("~/share_res/TEST/")
    getMeanDistFold("~/share_res/TEST/")
    distrib=read.csv("Population-distribution_Wilson2011.txt",header=F)


    maxSize=max(distrib$V1)
    logMax=log2(maxSize)
	breaks=2^(0:logMax+1)
	labs=2^(1:logMax)
	binCOEV=cut(distrib$V1,labels=labs,breaks=breaks)
	countSize=table(binCOEV)
	countSize=countSize[countSize>0]
	fit=lm(log(countSize) ~ log(as.numeric(names(countSize))))                                                     

	plot(names(countSize),countSize,log="xy",ylab="number of settlement",xlab="settlement size")
	points(as.numeric(names(fit$fitted.values)),exp(fit$fitted.values),col="red",type="l")

	plot(names(countSize),countSize,ylab="number of settlement size",xlab="settlement size")
	points(as.numeric(names(fit$fitted.values)),exp(fit$fitted.values),col="red",type="l")

	points(names(countSize),countSize,ylab="settlement size",xlab="number of settlement")
	abline(seq(0.1,250000,100),fit$coefficients[1]*exp(seq(0.1,250000,100)*(fit$coefficients[2])),col="red")
	points(as.numeric(names(fit$fitted.values)),exp(fit$fitted.values),col="red",type="l")

	axis(2)

	points(seq(0.1,250000,100),fit$coefficients[1]+(seq(0.1,250000,100)^(-fit$coefficients[2])),col="red")






    matrixGoodPerSite(ulu)
    plotSiteWithGood(siteWithAmount(deb))

    x1 = 1000000           # Maximum value
    x0 = 1000         # It can't be zero; otherwise X^0^(neg) is 1/0.
    alpha = fit$coefficients[2]     # It has to be negative.
    y = runif(1e5)   # Number of samples
    x = fit$coefficients[1]*((x1^(alpha+1) - x0^(alpha+1))*y + x0^(alpha+1))^(1/(alpha+1))
	simubinCOEV=cut(x,labels=labs,breaks=breaks)
	simucountSize=table(simubinCOEV)
	simucountSize=simucountSize[simucountSize>000]
	plot(as.numeric(names(simucountSize)),simucountSize)
}

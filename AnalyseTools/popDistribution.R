#take a list of vallue and plot it's distribution
    plotBinDist <- function(dat,add=F,...){
	maxSize=max(dat) #max of dataset
	logMax=log2(maxSize) #log of the max 
	breaks=2^(0:logMax+1) 
	labs=2^(1:logMax)
	binCOEV=cut(dat,labels=labs,breaks=breaks)
	countSize=table(binCOEV)
	countSize=countSize[countSize>0]/sum(countSize)
	fit=lm(log2(countSize/sum(countSize)) ~ log2(as.numeric(names(countSize))))                                                     

	fit=lm(log2(countSize/sum(countSize)) ~ log2(as.numeric(names(countSize))))                                                     

	if(add)
	    points(as.numeric(names(countSize)),as.numeric(countSize),...)
	else
	    plot(as.numeric(names(countSize)),as.numeric(countSize),log="xy",ylab="frequencies",xlab="settlement size",...)
    }

#return a variable randomly distributed following a power law
simulateDist <- function(dat,len,x1,x0,alpha){
#taken from:
	#https://stackoverflow.com/questions/918736/random-number-generator-that-produces-a-power-law-distribution/46984446?noredirect=1#comment81267263_46984446

    y = runif(len)   # Number of samples
    x = ((x1^(alpha+1) - x0^(alpha+1))*y + x0^(alpha+1))^(1/(alpha+1))
    return(x)

}


drawGraphs<-function(){

    png("ProbabilityDistributionFucntion.png")
hist(x, prob = T, breaks=40,   border=F,  col="yellowgreen", xlab="settlement size",main="Power law density")
lines(density(x), col="chocolate", lwd=1)
lines(density(x, adjust=2), lty="dotted", col="darkblue", lwd=2)
h = hist(x, prob=T, breaks=40, plot=F)
dev.off()
 
distrib2=read.csv("Population-distribution_Wilson2011.txt",header=F)
distrib=read.csv("population-distribution-Hanson-2016.txt",header=F)
distrib=distrib$V1
distrib2=distrib2$V1


png("distribRank.png")
    points(sort(distrib) ~ order(distrib) ,xlab="rank",ylab="settlement size",log="yx")
    plot(sort(distrib2) ~ order(distrib2) ,xlab="rank",ylab="settlement size",log="yx",col="red")
dev.off() 
png("distribRankFit.png")
	fit=lm(log(sort(distrib))~log(order(distrib)))
	fit=lm(log(sort(distrib))~log(order(distrib)))
	fit2=lm(log(sort(distrib2))~log(order(distrib2)))
	plot(sort(distrib) ~ order(distrib) ,xlab="rank",ylab="settlement size",log="yx")
	points(exp(.001:7),exp(fit$coefficients[1]+(.001:7)*fit$coefficients[2]),col="black",type="l",lwd=3)
	points(exp(.001:7),exp(fit2$coefficients[1]+(.001:7)*fit2$coefficients[2]),col="red",type="l",lwd=3)
	text(exp(7/2-3),exp((fit$coefficients[1]+(7/2)*fit$coefficients[2])),label=paste("y=ax+b :\n",round(fit$coefficients[2],2),"x+",round(fit$coefficients[1],2),"\nalpha=1-1/a ~",round(alpha,2)),col="orange",adj=0,lwd=4,cex=1.2)
	text(exp(7/2-3),exp((fit2$coefficients[1]+(7/2)*fit2$coefficients[2])),label=paste("y=ax+b :\n",round(fit2$coefficients[2],2),"x+",round(fit2$coefficients[1],2),"\nalpha=1-1/a ~",round(alpha2,2)),col="black",adj=0,lwd=4,cex=1.2)

		alpha = -1 + 1/fit$coefficients[2]     # It has to be negative.
		alpha2 = -1 + 1/fit2$coefficients[2]     # It has to be negative.
dev.off()

png("distribRankSimAll.png")
    plot(sort(distrib) ~ order(distrib) ,xlab="rank",ylab="yellowgreen",log="yx",type="n")
	points(sort(distrib)~order(distrib))
	points(sort(distrib)~order(distrib),pch=20,col="white")
sapply(1:10,function(i){
       #png("distribRankSimAll3.png")
       	ul=simulateDist(distrib,length(distrib),min(distrib),max(distrib),alpha)
	fomrage=softenedData(ul)
	points((fomrage)~(seq_along(fomrage)),col=alpha("yellowgreen",.3),pch=20)
	})
	dev.off()

ev.off()
	plot(sort(distrib)~order(distrib),log="xy",type="n",xlab="rank",ylab="settlement size")

	#the fit is done on ranked distribution (cdf?) and we assume ths ranked distribution is a power low thus it should have a power low probability distribution 


	#in that case from the alpha computed in this CDF we can deduce that the alpha of the power law distribution is 1+1/alpha
	#https://stats.stackexchange.com/questions/91670/connection-between-power-law-and-zipfs-law#114582
	#and more detailled stuff: http://www.hpl.hp.com/research/idl/papers/ranking/ranking.html
		alpha = -1 + 1/fit$coefficients[2]     # It has to be negative.

sapply(1:100,function(i){
       	ul=simulateDist(distrib,length(distrib),min(distrib),max(distrib),-1*alpha)
	#fomrage=softenedData(ul)
	fomrage=sort(ul,decreasing=T)
	points((fomrage)~(seq_along(fomrage)),col=alpha("dark green",.1),pch=20)
	})
	points(sort(distrib)~order(distrib),log="xy",pch=20,col="white")
	points(sort(distrib)~order(distrib),log="xy")

    png("distribDataBined.png")
    plotBinDist(distrib,main="Frequencies Distributions Binned",add=T)
    plotBinDist(ul,main="Frequencies Distributions Binned",add=F,col="red")
    dev.off()

    png("distribDataBinedAndSimRepeated.png")
    plotBinDist(distrib,main="Frequencies Distributions Binned")
       	sapply(1:10,function(i){       	ul=simulateDist(distrib,length(distrib),min(distrib),max(distrib),alpha)
	fomrage=softenedData(ul)
    plotBinDist(fomrage,add=T,col=alpha("yellowgreen",.2),pch=20,cex=1.5)})
sapply(1:10,function(i){       	       	ul=simulateDist(distrib,length(distrib)*10,min(distrib),max(distrib),alpha)
	fomrage=softenedData(ul)
    plotBinDist(fomrage,add=T,col=alpha("green",.2),pch=20,cex=1.5)})
sapply(1:10,function(i){       	ul=simulateDist(distrib,length(distrib)*100,min(distrib),max(distrib),alpha)
	fomrage=softenedData(ul)
    plotBinDist(fomrage,add=T,col=alpha("purple",.2),pch=20,cex=1.5)})
    legend("topright",legend=c("data","sim(len x 1)","sim(len x 10)","sim(len x 100)"),col=c(1,"yellowgreen","orange","purple"),pch=21)
    dev.off()
   

    distrib=read.csv("~/share_res/dis",header=F)
}


##this function take simulated data and cast it in mor similare data than the one we hae
softenedData <- function(dat){
	softed=sort(signif(dat,digits=3),decreasing=T) 
	softed[softed<5000]=2000
	softed[softed<11750 && softed >= 10000]=10000
	softed[softed<14500 && softed >= 14000]=14000
	return(softed)

}

plotDist <- function(dat) plot(as.numeric(table(signif(dat,3))) ~ as.numeric(names(table(signif(dat,2)))),log="xy")

fitNew <- function(dat){
    dat=distrib
    N=length(dat)
    r =1:length(dat)
    ex <- function(x,u,a,b)(u*(length(x)+1-x)^b)/(x^a)
    points(r,ex(r,85199,.69,.21),log="yx",type="l",col="blue")
        x=seq(0.01,7,length.out=length(r))
    plot(dat~r, log = "xy",xlab = "Rank", ylab = "Population")
    points(r,ex(r,exp(co[[1]]),-1*co[[3]],co[[2]]),col="green")
    points(r,ex(r,coef(aa)[1],-1/coef(aa)[3],coef(aa)[2]),col="blue")
    points(r,ex(r,coef(aa)[1],coef(aa)[2],coef(aa)[3]),col="blue")
    points(r,ex(r,50000,.95,.54),col="blue",log="y")
    lines(r,ex(r,exp(co[[1]]),-1*co[[3]],co[[2]]),col="green")

    ##plot the fitting using the parameter determined using lm
    points(exp(x),exp(fit$coefficients[1]+(x)*fit$coefficients[2]),col="orange")
    lines(exp(x),exp(fit$coefficients[1]+(x)*fit$coefficients[2]),col="red")
    log.f = log(dat)
    log.inverse.rank = log(length(r)+1-r)
    co=coef(lm(log(dat)~log.inverse.rank + log(r)))##fiting using a linear model
    zmf = function(x) exp(co[[1]]+ co[[2]]*log(length(x)+1-x) + co[[3]]*log(x))
    xx=c(1:length(Rank))
    ###plot the fiting of the linear model
    lines(zmf(xx)~xx, col = "red")
    points(zmf(xx)~xx, col = "purple")
    #####my fitting using nls
    fit3=coef(nls(dat ~ ex(r,u,a,b),lower=list(u=00,a=0,b=0.28),start=list(u=70000,a=0.7,b=0.5),algorithm="port"))
    ##plot the fitting using the parameter determined using nls
    points(r,ex(r,fit3[["u"]],fit3[["a"]],fit3[["b"]]),col="blue")
}

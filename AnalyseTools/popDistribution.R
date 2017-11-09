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

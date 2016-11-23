if(require("XML")){library(XML)}

###Thsi fuction read a csv and add the propertie associeted to a elelment find in the xml config file
##Warning : do it just for "network" pro, should be extend to put a list of properties.
readAddProp<-function(docname,...){
    res=read.csv(paste(docname,"/agents.csv",sep=""),sep=";")

    i=xmlTreeParse(paste(docname,"/config.xml",sep=""))
    i=xmlRoot(i)
    net=i[["network"]]
    print(net)
    for( u in 1:xmlSize(net)){
	attri=xmlAttrs(net[[u]])
	if(length(attri)>0){
	    paramId=attri[["id"]]
	    paramValue=as.numeric(attri[["value"]])
	    newCol=rep(paramValue,nrow(res))
	    res=cbind(res,newCol)
	    names(res)[length(res)]=paramId
	}

    }
    return(res)
}


###This return aa properties in an XML config file.
##the XML file shoudl be on the form:
## <type>
##	<XXX id=propId value=valeureturned/>
## </type>
getPropFromXml <- function(expeDir,type,propId,format=as.numeric){
    filename=paste(expeDir,"/config.xml",sep="")
    i=xmlTreeParse(filename)
    i=xmlRoot(i)
    net=i[[type]]
    if(length(net)<1){
	print(paste("Object\'",type,"\' not defined in the XML file",filename))
	return(NULL)
    }
    child=xmlChildren(net)
    for(j in child){
	a=xmlAttrs(j)
	print(propId)
	if(a["id"]==propId)
	    return(format(a["value"]))
    }
    print(paste("Property \'", propId, "\' not defined in the XML file",filename))
    return(NULL)
}


getProp  <-  function(configfile,type,cla){
    return( xmlAttrs( getNodeSet(xmlParse(configfile),paste("//",type,sep=""))[[1]])[cla] )
}


test <- function(){

    means=c()
    steps=c()
    plot(1,1,xlim=c(0,30),ylim=c(0,10),type="n")
    for(i in list.dirs("~/share_res/",recursive=F)){
	print(i)
	af=file.path(i,"agents.csv")
	cf=file.path(i,"config.xml")
	if(file.exists(af) & file.exists(cf)){
	    ag=read.csv(af,sep=";")
	    ag=ag[ag$timeStep > 0,]
	    print(cf)
	    curs=as.numeric(getProp(cf,"controller/culture","step"))
	    print(curs)
	    print(mean(ag$scores))
	    steps=c(steps,curs)
	    means=c(means,mean(ag$scores))
	    points(curs,means[length(means)])
	}
	else{
	print("bad")
	}
    }
}

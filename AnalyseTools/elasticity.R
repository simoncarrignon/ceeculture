##Fichier script pour tester si les partages en secteurs fonctionnes i.e. les sommes donnent 1 et ce genre de truc


ngood=10
nsegments=as.integer(runif(1,1,ngood))
hist(.3+(2-.3)*runif(1000)) #la function de gintis fait que runif(1,.3,2)


s=runif(1000,.3,2)
s=sort(s)
gama  = (s-1)/s
plot(s, gama,type="l")  #gamma for choosen elas

alpha=runif(nsegments)
alpha=alpha/sum(alpha)
xval=runif(nsegments)

util <- function(alpha,xval,gama)(sum((alpha*xval^gama)))^(1/gama)
plot(gama,sapply(seq_along(gama),function(i)util(alpha,xval,gama[i])),type="l")


gama=0.449036
xval=0.458;alpha=0.036965
util(alpha,xval,gama) 
xval=0.418;alpha=0.463035
util(alpha,xval,gama) 


Roman <- 92                                          
gama=0.0749306;xval=0.917;ces=0.478464;alpha= 0.48158
gama=c(gama,0.0749306);xval=c(xval,0);ces=0.478464;alpha= c(alpha,0.15323)
gama=c(gama,0.0749306);xval=c(xval,11.346);ces=0.916549;alpha=c(alpha,0.36519)
segement:0,weight:0.256195,n <- good:3,contrib:0.312565

util(alpha,xval,gama) 

gama=0.0749306;xval=0.919;ces=0.0467943;alpha= 0.0470914
gama=0.0749306;xval=0.598;ces=0.484985;alpha= 0.455402
gama=0.0749306;xval=0;ces=0.484985;alpha= 0.497507 
segement:1,weight:0.213877,n <- good:3,contrib:6.39477e-05
gama=0.0749306;xval=0;ces=0;alpha= 0.416785
gama=0.0749306;xval=0;ces=0;alpha= 0.583215          
segement:2,weight:0.0446054,n <- good:2,contrib:1         
gama=0.0749306;xval=0;ces=0;alpha= 0.764628          
gama=0.0749306;xval=0.946;ces=0.234395;alpha= 0.235372
segement:3,weight:0.232177,n <- good:2,contrib:3.90428e-09
gama=0.0749306;xval=0.874;ces=0.347354;alpha= 0.350877
gama=0.0749306;xval=0;ces=0.347354;alpha= 0.524123
gama=0.0749306;xval=0;ces=0.347354;alpha= 0.125       
segement:4,weight:0.212352,n <- good:3,contrib:7.43529e-07
gama=0.0749306;xval=0;ces=0;alpha= 0.61098 
gama=0.0749306;xval=0;ces=0;alpha= 0.38902            
segement:5,weight:0.040793,n <- good:2,contrib:1    
0.16549                                              
Roman <- 58                                         
gama=0.280576;xval=0;ces=0;alpha= 0.0699797         
gama=0.280576;xval=0;ces=0;alpha= 0.221349       
gama=0.280576;xval=0.67;ces=0.0145025;alpha= 0.0162272
gama=0.280576;xval=0.421;ces=0.0485153;alpha= 0.043357
gama=0.280576;xval=0;ces=0.0485153;alpha= 0.0238337  
gama=0.280576;xval=0;ces=0.0485153;alpha= 0.167343
gama=0.280576;xval=0;ces=0.0485153;alpha= 0.188895  
gama=0.280576;xval=0.456;ces=0.204939;alpha= 0.19498 
gama=0.280576;xval=0.725;ces=0.272588;alpha= 0.0740365
segement:0,weight:0.482562,n <- good:9,contrib:0.00972946

gama=0.280576;xval=0;ces=0;alpha= 0.118562
gama=0.280576;xval=0.593;ces=0.122552;alpha= 0.141904
gama=0.280576;xval=0;ces=0.122552;alpha= 0.219711
gama=0.280576;xval=0;ces=0.122552;alpha= 0.308262
gama=0.280576;xval=10.742;ces=0.53223;alpha= 0.210448
gama=0.280576;xval=0;ces=0.53223;alpha= 0.00111152
segement:1,weight:0.517438,n <- good:6,contrib:0.105631
0.0593524   

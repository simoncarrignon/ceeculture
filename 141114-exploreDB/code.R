#!/usr/bin/Rscript
#yo

library(ggplot2)
library(gridExtra)


# frequency of stamps and places

dataAllStamp<- read.csv('allStamps.csv', sep=';', header=TRUE)
tmpTable<- data.frame(lst=dataAllStamp$SimpleText)
allStampFreq <- as.data.frame(table(tmpTable$lst))

dataStampTit<- read.csv('stampsWithTit.csv', sep=';', header=TRUE)
tmpTable<- data.frame(lst=dataStampTit$SimpleText)
stampTitFreq <- as.data.frame(table(tmpTable$lst))


# temporal distribution of values    

dataTemporal<- read.csv('dating.tmp.csv', sep=';', header=TRUE)
minYear = min(dataTemporal$StartYear)
maxYear = max(dataTemporal$EndYear)

time <- data.frame(year=seq(minYear, maxYear))
time$volume = 0


countRepetitions <- function(tmpDataRow, year)
{
    if(year>=tmpDataRow['StartYear'] & year<=tmpDataRow['EndYear'])
    {
        return(1)
    }
    return(0)
}

computeVolume <- function(row)
{
    row['volume'] = sum(apply(dataTemporal, 1, countRepetitions, row['year']))
}

time$volume = apply(time, 1, computeVolume)



g1 <- ggplot(allStampFreq, aes(x=Freq)) + geom_histogram(binwidth=1) + ggtitle('frequency distribution of all stamps') + scale_x_sqrt(breaks=c(1,5,10, seq(10,50,10),seq(50,max(allStampFreq$Freq),50))) + xlab('Freq (sqrt scale)') + ylab('stamp instances')
g2 <- ggplot(stampTitFreq, aes(x=Freq)) + geom_histogram(binwidth=1) + ggtitle('frequency distribution stamps with titulus pictus') + xlab('Freq') + ylab('stamp instances')
g3 <- ggplot(time, aes(x=year, y=volume)) + geom_line() + ggtitle('temporal distribution of titulus pictus findings') + xlim(c(0,300))

pdf('stats_db.pdf')
grid.arrange(g1, g2, g3, ncol=1)
dev.off()


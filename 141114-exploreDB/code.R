
library(ggplot2)
library(gridExtra)

data<- read.csv('la_catria_dataset_space.txt', sep='\t', header=TRUE)

# frequency of stamps and places

tmpTable<- data.frame(lst=data$Stamp_SimpleText)
stampFreq <- as.data.frame(table(tmpTable$lst))

tmpTable<- data.frame(lst=data$FindingPlace)
locFreq <- as.data.frame(table(tmpTable$lst))

# temporal distribution of values    

tmpData<- read.csv('la_catria_dataset_time.txt', sep='\t', header=TRUE)
minYear = min(tmpData$Dating_StartYear)
maxYear = max(tmpData$Dating_EndYear)

time <- data.frame(year=seq(minYear, maxYear))
time$volume = 0


countRepetitions <- function(tmpDataRow, year)
{
    if(year>=tmpDataRow['Dating_StartYear'] & year<=tmpDataRow['Dating_EndYear'])
    {
        return(1)
    }
    return(0)
}

computeVolume <- function(row)
{
    row['volume'] = sum(apply(tmpData, 1, countRepetitions, row['year']))
}

time$volume = apply(time, 1, computeVolume)



g1 <- ggplot(stampFreq, aes(x=Freq)) + geom_histogram(binwidth=1) + ggtitle('frequency distribution of stamps') + scale_x_sqrt(breaks=c(1,5,10, seq(10,50,10),seq(50,max(stampFreq$Freq),50))) + xlab('Freq (sqrt scale)') + ylab('stamp instances')
g2 <- ggplot(locFreq, aes(x=Freq)) + geom_histogram(binwidth=1) + ggtitle('frequency distribution of finding locations') + scale_x_sqrt(breaks=c(0,1,5,10, seq(10,50,10),seq(50,max(locFreq$Freq),50))) + xlab('Freq (sqrt scale)') + ylab('stamps per location')
g3 <- ggplot(time, aes(x=year, y=volume)) + geom_line() + ggtitle('temporal distribution of stamp findings')

pdf('stats_lacatria.pdf')
grid.arrange(g1, g2, g3, ncol=1)
dev.off()


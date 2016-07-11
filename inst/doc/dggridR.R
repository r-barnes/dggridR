## ---- fig.width=5, fig.height=5, results='hide', warning=FALSE, error=FALSE----
#Include libraries
library(dggridR)
library(dplyr)

#Construct a global grid with cells approximately 1000 miles across
dggs          <- dgconstruct(spacing=1000, metric=FALSE, resround='down')

#Load included test data set
data(dgquakes)

#Get the corresponding grid cells for each earthquake epicenter (lat-long pair)
dgquakes$cell <- dgtransform(dggs,dgquakes$lat,dgquakes$lon)

#Get the number of earthquakes in each cell
quakecounts   <- dgquakes %>% group_by(cell) %>% summarise(count=n())

#Get the grid cell boundaries for the whole Earth using this dggs in a form
#suitable for plotting with ggplot2
grid          <- dgearthgrid(dggs,frame=TRUE)

#Update the grid cells' properties to include the number of earthquakes
#in each cell
grid          <- merge(grid,quakecounts,by.x="Name",by.y="cell")

#Make adjustments so the output is more visually interesting
grid$count    <- log(grid$count)
cutoff        <- quantile(grid$count,0.9)
grid          <- grid %>% mutate(count=ifelse(count>cutoff,cutoff,count))

#Polygons crossing the -180/180 degree line cause polygons will end up getting
#stretched all over the map. Let's fix that.

#Find dangerous polygons based on how many degrees of longitude they span
groups_to_wrap <- grid %>% group_by(group) %>% summarise(diff=max(long)-min(long)) %>% filter(diff>180) %>% select(group)

#Adjust them so they appear on the eastern side of the map
grid           <- grid %>% mutate(long=ifelse(group %in% groups_to_wrap$group, ifelse(long<0,long+360,long), long))

#Arrange polygon points so they are ordered appropriately, otherwise the results
#will not be nice, closed cells, but weird triangular thingies
grid           <- grid %>% arrange(group,order)

#Get polygons for each country of the world
countries <- map_data("world")

## ---- fig.width=6, fig.height=4------------------------------------------
#Plot everything on a flat map
p<- ggplot() + 
    geom_polygon(data=countries, aes(x=long, y=lat, group=group), fill=NA, color="black")   +
    geom_polygon(data=grid,      aes(x=long, y=lat, group=group, fill=count), alpha=0.4)    +
    geom_path   (data=grid,      aes(x=long, y=lat, group=group), alpha=0.4, color="white") +
    scale_fill_gradient(low="blue", high="red")
p

## ---- fig.width=6, fig.height=6------------------------------------------
#Replot on a spherical projection
p+coord_map("ortho", orientation = c(-38.49831, -179.9223, 0))+
  xlab('')+ylab('')+
  theme(axis.ticks.x=element_blank())+
  theme(axis.ticks.y=element_blank())+
  theme(axis.text.x=element_blank())+
  theme(axis.text.y=element_blank())+
  ggtitle('Your data could look like this')

## ---- eval=FALSE---------------------------------------------------------
#  library(rgdal)
#  
#  #Get the grid cell boundaries for the whole Earth using this dggs in a form
#  #suitable for printing to a KML file
#  grid          <- dgearthgrid(dggs,frame=FALSE)
#  
#  #Update the grid cells' properties to include the number of earthquakes
#  #in each cell
#  grid@data$count <- merge(grid@data, quakecounts, by.x="Name", by.y="cell", all.x=TRUE)$count
#  
#  #Write out the grid
#  writeOGR(grid, "quakes_per_cell.kml", "quakes", "KML")


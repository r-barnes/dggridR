\name{dgquakes}
\alias{dgquakes}
\title{All earthquakes with magnitude >=3.0 earthquakes for 2015}
\description{ 
  A data frame with 19914 observations on the following 4 variables.
  \describe{ 
    \item{\code{time}}{Time of the quake. Example: 2015-12-31T23:39:28.940Z}
    \item{\code{lat}}{Latitude of the epicenter. Example: -7.0711}
    \item{\code{lon}}{Longitude of the epicenter. Example: -173.5178} 
    \item{\code{mag}}{Magnitude of the quake. Example: 3.2} 
  } 
} 
\usage{data(dgquakes)} 
\format{data frame} 
\source{The USGS Earthquake Hazards Program (\url{http://earthquake.usgs.gov/earthquakes/}). Retrieved with the query \url{http://earthquake.usgs.gov/fdsnws/event/1/query.csv?starttime=2015-01-01%2000:00:00&endtime=2015-12-31%2023:59:59&minmagnitude=3.0&orderby=time}} 
\keyword{datasets} 
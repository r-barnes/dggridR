LIB = dglib.a 
SRCS = DgAddressBase.cpp DgBase.cpp DgBoundedHexC2RF2D.cpp DgBoundedIDGG.cpp DgBoundedIDGGS.cpp DgBoundedRF2D.cpp DgBoundedRFS2D.cpp DgCell.cpp DgColor.cpp DgConverterBase.cpp DgDiscRFS2D.cpp DgDistanceBase.cpp DgDmdD4Grid2D.cpp DgDmdD4Grid2DS.cpp DgDmdD8Grid2D.cpp DgDmdD8Grid2DS.cpp DgDVec2D.cpp DgDVec3D.cpp DgEllipsoidRF.cpp DgGeoSphRF.cpp DgHexC1Grid2D.cpp DgHexC2Grid2D.cpp DgHexGrid2DS.cpp DgIcosaMap.cpp DgIDGG.cpp DgIDGGS3H.cpp DgIDGGS43H.cpp DgIDGGS4D.cpp DgIDGGS4H.cpp DgIDGGS4T.cpp DgIDGGS.cpp DgInAIGenFile.cpp DgInLocTextFile.cpp DgInputStream.cpp DgInShapefileAtt.cpp DgInShapefile.cpp DgIVec2D.cpp DgIVec3D.cpp DgLocation.cpp DgLocBase.cpp DgLocList.cpp DgLocVector.cpp DgOutAIGenFile.cpp DgOutGeoJSONFile.cpp DgOutKMLfile.cpp DgOutLocFile.cpp DgOutLocTextFile.cpp DgOutPtsText.cpp DgOutputStream.cpp DgOutRandPtsText.cpp DgOutShapefile.cpp DgParamList.cpp DgPolygon.cpp DgProjFuller.cpp DgProjGnomonicRF.cpp DgProjISEA.cpp DgProjTriRF.cpp DgRandom.cpp DgRFBase.cpp DgRFNetwork.cpp DgSeriesConverter.cpp DgSqrD4Grid2D.cpp DgSqrD4Grid2DS.cpp DgSqrD8Grid2D.cpp DgSqrD8Grid2DS.cpp DgSuperfund.cpp DgTriGrid2D.cpp DgTriGrid2DS.cpp DgUtil.cpp 
OBJS = DgAddressBase.o DgDiscRFS2D.o DgHexC1Grid2D.o DgIDGGS.o DgLocList.o DgOutShapefile.o DgSeriesConverter.o DgBase.o DgDistanceBase.o DgHexC2Grid2D.o DgInAIGenFile.o DgLocVector.o DgParamList.o DgSqrD4Grid2D.o DgBoundedHexC2RF2D.o DgDmdD4Grid2D.o DgHexGrid2DS.o DgInLocTextFile.o DgOutAIGenFile.o DgPolygon.o DgSqrD4Grid2DS.o DgBoundedIDGG.o DgDmdD4Grid2DS.o DgIcosaMap.o DgInputStream.o DgOutGeoJSONFile.o DgProjFuller.o DgSqrD8Grid2D.o DgBoundedIDGGS.o DgDmdD8Grid2D.o DgIDGG.o DgInShapefileAtt.o DgOutKMLfile.o DgProjGnomonicRF.o DgSqrD8Grid2DS.o DgBoundedRF2D.o DgDmdD8Grid2DS.o DgIDGGS3H.o DgInShapefile.o DgOutLocFile.o DgProjISEA.o DgSuperfund.o DgBoundedRFS2D.o DgDVec2D.o DgIDGGS43H.o DgIVec2D.o DgOutLocTextFile.o DgProjTriRF.o DgTriGrid2D.o DgCell.o DgDVec3D.o DgIDGGS4D.o DgIVec3D.o DgOutPtsText.o DgRandom.o DgTriGrid2DS.o DgColor.o DgEllipsoidRF.o DgIDGGS4H.o DgLocation.o DgOutputStream.o DgRFBase.o DgUtil.o DgConverterBase.o DgGeoSphRF.o DgIDGGS4T.o DgLocBase.o DgOutRandPtsText.o DgRFNetwork.o
#DEPS = DgAddressBase.depends DgBase.depends DgBoundedHexC2RF2D.depends DgBoundedIDGG.depends DgBoundedIDGGS.depends DgBoundedRF2D.depends DgBoundedRFS2D.depends DgCell.depends DgColor.depends DgConverterBase.depends DgDiscRFS2D.depends DgDistanceBase.depends DgDmdD4Grid2D.depends DgDmdD4Grid2DS.depends DgDmdD8Grid2D.depends DgDmdD8Grid2DS.depends DgDVec2D.depends DgDVec3D.depends DgEllipsoidRF.depends DgGeoSphRF.depends DgHexC1Grid2D.depends DgHexC2Grid2D.depends DgHexGrid2DS.depends DgIcosaMap.depends DgIDGG.depends DgIDGGS3H.depends DgIDGGS43H.depends DgIDGGS4D.depends DgIDGGS4H.depends DgIDGGS4T.depends DgIDGGS.depends DgInAIGenFile.depends DgInLocTextFile.depends DgInputStream.depends DgInShapefileAtt.depends DgInShapefile.depends DgIVec2D.depends DgIVec3D.depends DgLocation.depends DgLocBase.depends DgLocList.depends DgLocVector.depends DgOutAIGenFile.depends DgOutGeoJSONFile.depends DgOutKMLfile.depends DgOutLocFile.depends DgOutLocTextFile.depends DgOutPtsText.depends DgOutputStream.depends DgOutRandPtsText.depends DgOutShapefile.depends DgParamList.depends DgPolygon.depends DgProjFuller.depends DgProjGnomonicRF.depends DgProjISEA.depends DgProjTriRF.depends DgRandom.depends DgRFBase.depends DgRFNetwork.depends DgSeriesConverter.depends DgSqrD4Grid2D.depends DgSqrD4Grid2DS.depends DgSqrD8Grid2D.depends DgSqrD8Grid2DS.depends DgSuperfund.depends DgTriGrid2D.depends DgTriGrid2DS.depends DgUtil.depends 
SHAPELIBDIR = ../../shapelib-1.3.0
PROJ4LIBDIR = ../../proj4lib
INCDIRS = -I../include -I$(PROJ4LIBDIR)/include -I$(SHAPELIBDIR)

.PHONY: clean all

all: $(LIB)

$(LIB): $(SRC) $(OBJS) #$(DEPS)
	$(AR) rcs $(LIB) $(OBJS)

%.o: %.cpp 
	$(CXX1X) -c $< $(CXX1XFLAGS) $(INCDIRS)

#%.depends: %.cpp
#	$(CXX) -MM $(CXXFLAGS) $(INCDIRS) $< > $@

clean:
	rm -rf $(OBJS) $(LIB) ii_files ti_files make.out #$(DEPS)

#-include $(DEPS)

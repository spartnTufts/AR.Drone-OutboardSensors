#
# Package Installer Makefile
#
# Installer program for the Sendata Server Package
#  
# (C) J.Weeks Tufts University 10/10/2012
#
# See help rule for instructions.
#

SDKPATH = 
COREDIR = $(SDKPATH)/ARDroneLib/Soft/Lib/ardrone_tool
SERVERDIR = $(SDKPATH)/SendataServer/source
DEMODIR = $(SDKPATH)/Examples/Linux/sdk_demo/Sources


MKDIR = mkdir -pv

all: help

install: cpsafe core control server client
	@$(CP) Makefiles/ServerMakefile $(SDKPATH)/SendataServer/Makefile
	@$(CP) SendataREADME.pdf $(SDKPATH)/

install_fresh: cpow core control server client
	@$(CP) Makefiles/ServerMakefile $(SDKPATH)/SendataServer/Makefile
	@$(CP) SendataREADME.pdf $(SDKPATH)/

install_sdkmods: cpow sdk

sdk:
	@$(CP) Makefiles/custom.makefile $(SDKPATH)/ARDroneLib/Soft/Build/
	@$(CP) Makefiles/CoreMakefile $(SDKPATH)/ARDroneLib/Soft/Lib/Build/Makefile
	@$(CP) Makefiles/ExMakefile $(SDKPATH)/Examples/Linux/Build/Makefile
	@$(CP) Makefiles/DemMakefile $(DEMODIR)/../Build/Makefile
	@$(CP) Demo/ardrone_testing_tool.c $(DEMODIR)/
	@$(CP) Demo/navdata.h $(DEMODIR)/Navdata/
	@$(CP) Demo/navdata.c $(DEMODIR)/Navdata/ 
	@$(CP) Demo/ardrone.xml $(SDKPATH)/Examples/Linux/Build/

cpsafe:
	$(eval CP = cp -iv --preserve=mode,timestamps)

cpow:
	$(eval CP = cp -fv --preserve=mode,timestamps)

cpud:
	$(eval CP = cp -uv --preserve=mode,timestamps)

core:
	@$(MKDIR) $(COREDIR)/Sendata
	@$(CP) Core/* $(COREDIR)/Sendata/

control:
	@$(CP) Control/* $(COREDIR)/Control/

server:
	@$(MKDIR) $(SDKPATH)/SendataServer
	@$(MKDIR) $(SERVERDIR)/../src
	@$(MKDIR) $(SERVERDIR)
	@$(CP) Server/* $(SERVERDIR)/

client:
	@$(MKDIR) $(DEMODIR)/Sendata
	@$(CP) Client/* $(DEMODIR)/Sendata/

mkdirs:
	@$(MKDIR) Core Control Server Client Demo Makefiles

sync_package: cpud mkdirs sdk core client control server
#remaining sync from package
	@$(CP) Makefiles/ServerMakefile $(SDKPATH)/SendataServer/Makefile
	@$(CP) SendataREADME.pdf $(SDKPATH)/
#sync from installed
#sendata
	@$(CP) $(COREDIR)/Sendata/* Core/
	@$(CP) $(COREDIR)/Control/ardrone_sendata* Control/
	@$(CP) $(SERVERDIR)/* Server/
	@$(CP) $(DEMODIR)/Sendata/* Client/
	@$(CP) $(SERVERDIR)/../Makefile Makefiles/ArduinoMakefile
#other key SDK programs
	@$(CP) $(SDKPATH)/ARDroneLib/Soft/Build/custom.makefile Makefiles/custom.makefile
	@$(CP) $(SDKPATH)/ARDroneLib/Soft/Lib/Build/Makefile Makefiles/CoreMakefile
	@$(CP) $(SDKPATH)/Examples/Linux/Build/Makefile Makefiles/ExMakefile
	@$(CP) $(DEMODIR)/../Build/Makefile Makefiles/DemMakefile
	@$(CP) $(DEMODIR)/ardrone_testing_tool.c Demo/ardrone_testing_tool.c
	@$(CP) $(DEMODIR)/Navdata/navdata.h Demo/navdata.h
	@$(CP) $(DEMODIR)/Navdata/navdata.c Demo/navdata.c 
	@$(CP) $(SDKPATH)/Examples/Linux/Build/ardrone.xml Demo/ardrone.xml 

help:
	@echo ""
	@echo "Instructions:" 
	@echo ""
	@echo " 1. Place SendataServerPackage in SDK Directory or update"
	@echo "    SKDPATH to be the path of the SDK."
	@echo "    (ie: ARDrone_SDK_Version_X_Y_YYYYMMDD/)" 
	@echo ""
	@echo " 2. Call install to install Sendata Server" 
	@echo ""
	@echo " 3. Call install_sdkmods to install the modified SDK files." 
	@echo "    DANGER: This will overwrite existing SDK files."
	@echo "    (See the list of Modified SDK Files in the Readme)" 
	@echo ""
	@echo " 4. Call install_fresh to reinstall Sendata Server."
	@echo "    This will overwrite the previously installed files."
	@echo ""
	@echo " 5. Call sync_package to run a sync between the Package and"
	@echo "    Installed files, updating the respective files to the"
	@echo "    newest version. (For re-syncing during Git updates only.)"
	@echo ""

.PHONY: all install install_fresh sync_package core client control server cpsafe cpow cpud install_sdkmods mkdirs sdk help
##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=DBI_3
ConfigurationName      :=Release
WorkspacePath          :=/home/ankifor/Documents/CPP
ProjectPath            :=/home/ankifor/Documents/CPP/DBI_3
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Andrey Nikiforov
Date                   :=20/11/16
CodeLitePath           :=/home/ankifor/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="DBI_3.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O3 -fexpensive-optimizations -std=c++11 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/neworderrandom.cpp$(ObjectSuffix) $(IntermediateDirectory)/schema_1.cpp$(ObjectSuffix) $(IntermediateDirectory)/Types.cpp$(ObjectSuffix) $(IntermediateDirectory)/join_query.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ankifor/Documents/CPP/DBI_3/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix)main.cpp

$(IntermediateDirectory)/neworderrandom.cpp$(ObjectSuffix): neworderrandom.cpp $(IntermediateDirectory)/neworderrandom.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ankifor/Documents/CPP/DBI_3/neworderrandom.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/neworderrandom.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/neworderrandom.cpp$(DependSuffix): neworderrandom.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/neworderrandom.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/neworderrandom.cpp$(DependSuffix) -MM neworderrandom.cpp

$(IntermediateDirectory)/neworderrandom.cpp$(PreprocessSuffix): neworderrandom.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/neworderrandom.cpp$(PreprocessSuffix)neworderrandom.cpp

$(IntermediateDirectory)/schema_1.cpp$(ObjectSuffix): schema_1.cpp $(IntermediateDirectory)/schema_1.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ankifor/Documents/CPP/DBI_3/schema_1.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/schema_1.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/schema_1.cpp$(DependSuffix): schema_1.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/schema_1.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/schema_1.cpp$(DependSuffix) -MM schema_1.cpp

$(IntermediateDirectory)/schema_1.cpp$(PreprocessSuffix): schema_1.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/schema_1.cpp$(PreprocessSuffix)schema_1.cpp

$(IntermediateDirectory)/Types.cpp$(ObjectSuffix): Types.cpp $(IntermediateDirectory)/Types.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ankifor/Documents/CPP/DBI_3/Types.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Types.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Types.cpp$(DependSuffix): Types.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Types.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Types.cpp$(DependSuffix) -MM Types.cpp

$(IntermediateDirectory)/Types.cpp$(PreprocessSuffix): Types.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Types.cpp$(PreprocessSuffix)Types.cpp

$(IntermediateDirectory)/join_query.cpp$(ObjectSuffix): join_query.cpp $(IntermediateDirectory)/join_query.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ankifor/Documents/CPP/DBI_3/join_query.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/join_query.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/join_query.cpp$(DependSuffix): join_query.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/join_query.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/join_query.cpp$(DependSuffix) -MM join_query.cpp

$(IntermediateDirectory)/join_query.cpp$(PreprocessSuffix): join_query.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/join_query.cpp$(PreprocessSuffix)join_query.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/



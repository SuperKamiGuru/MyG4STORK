close all;
%clear all;
clc;

numIso=10;
numFS=43;
fileDir = '/home/SuperKamiGuru/NuclearSim/ComplimentarySoftware/CompareCSData/bin/Debug/test.txt';
Directory = '/mnt/264F50EB72F960F5/Data/MCNPSimCS/';

count=1;
    fid = fopen(fileDir,'rt','n');
    if((fid>-1)&&(~feof(fid)))
        numIso=0;
        while((numIso<100)&&(feof(fid)~=1))
            line=fgetl(fid);
            strEnd=strfind(line,':');
            if(size(strEnd)~=0)
                isoName=line(1:strEnd(1)-1)
                PlotCSData(isoName,Directory);
                
                numIso=numIso+1;
            end
        end
	      fclose(fid);
    end

% pararrayfun(nproc, @PlotFSHistLinux, fileNameVec);


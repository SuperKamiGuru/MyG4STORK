close all;
%clear all;
clc;

numIso=10;
numFS=43;
Directory = '/mnt/264F50EB72F960F5/Data/G4NDLMCNPComp/';
%Directory = 'C:/Users/Wesley/Downloads/';
FSDir = {'Elastic/FS/', 'Capture/FS/', 'Fission/FS/', 'Fission/FC/', 'Fission/SC/', 'Fission/TC/', 'Fission/LC/', 'Inelastic/F01/',...
        'Inelastic/F02/', 'Inelastic/F03/', 'Inelastic/F04/', 'Inelastic/F05/', 'Inelastic/F06/', 'Inelastic/F07/', 'Inelastic/F08/', 'Inelastic/F09/',...
        'Inelastic/F10/', 'Inelastic/F11/', 'Inelastic/F12/', 'Inelastic/F13/', 'Inelastic/F14/', 'Inelastic/F15/', 'Inelastic/F16/', 'Inelastic/F17/',...
        'Inelastic/F18/', 'Inelastic/F19/', 'Inelastic/F20/', 'Inelastic/F21/', 'Inelastic/F22/', 'Inelastic/F23/', 'Inelastic/F24/', 'Inelastic/F25/',...
        'Inelastic/F26/', 'Inelastic/F27/', 'Inelastic/F28/', 'Inelastic/F29/', 'Inelastic/F30/', 'Inelastic/F31/', 'Inelastic/F32/', 'Inelastic/F33/',...
        'Inelastic/F34/', 'Inelastic/F35/', 'Inelastic/F36/'};
FSName = {'Elastic Scattering', 'Radiative Capture', 'Combined Fission', 'First Chance Fission', 'Second Chance Fission', 'Third Chance Fission', 'Last Chance Fission', 'MT=4 Inelastic Scattering',...
        'MT=5 Inelastic Scattering', 'MT=11 Inelastic Scattering', 'MT=16 Inelastic Scattering', 'MT=17 Inelastic Scattering', 'MT=22 Inelastic Scattering', 'MT=23 Inelastic Scattering', 'MT=24 Inelastic Scattering',...
        'MT=25 Inelastic Scattering', 'MT=28 Inelastic Scattering', 'MT=29 Inelastic Scattering', 'NA', 'MT=32 Inelastic Scattering', 'MT=33 Inelastic Scattering', 'MT=34 Inelastic Scattering',...
        'NA','NA','MT=37 Inelastic Scattering', 'MT=41 Inelastic Scattering', 'MT=42 Inelastic Scattering', 'MT=44 Inelastic Scattering', 'MT=45 Inelastic Scattering', 'MT=103 Inelastic Scattering',...
        'MT=104 Inelastic Scattering', 'MT=105 Inelastic Scattering', 'MT=106 Inelastic Scattering', 'MT=107 Inelastic Scattering', 'MT=108 Inelastic Scattering', 'NA', 'MT=111 Inelastic Scattering',...
        'MT=112 Inelastic Scattering', 'NA', 'MT=113 Inelastic Scattering', 'MT=115 Inelastic Scattering', 'MT=116 Inelastic Scattering', 'MT=117 Inelastic Scattering'};
isoList = {'1_1_Hydrogen', '1_2_Hydrogen', '8_16_Oxygen', '90_232_Thorium','92_235_Uranium', '92_238_Uranium', '94_239_Plutonium', '94_240_Plutonium', '94_241_Plutonium', '94_242_Plutonium'};
 
count=1;
for i=1:1:numFS
%    for j=1:1:numIso
%      fileName = strcat(Directory, FSDir(i), isoList(j), '.txt');
%      fileNameVec(count)=fileName;
%      PlotFSHistLinux(fileName,FSName{i});
%    end
    fileDiff = strcat(Directory, FSDir(i), 'FileDiffList.txt');
    fileDiff=[fileDiff{:}];
    fid = fopen(fileDiff,'rt','n');
    if((fid>-1)&&(~feof(fid)))
        fgetl(fid);
        fgetl(fid);
        numIso=0;
        while((numIso<3)&&(feof(fid)~=1))
            line=fgetl(fid);
            strEnd=strfind(line,'has');
            if(size(strEnd)~=0)
                isoName=line(1:strEnd(1)-1);
                fileName = strcat(Directory, FSDir(i), isoName, '.txt');
                fileNameVec(count)=fileName;
                count=count+1;
                [fileName{:}]
                %fflush(stdout);
                PlotFSHistLinux(fileName,FSName{i});
                numIso=numIso+1;
            end
        end
	      fclose(fid);
    end
end

% pararrayfun(nproc, @PlotFSHistLinux, fileNameVec);

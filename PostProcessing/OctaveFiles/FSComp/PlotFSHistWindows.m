function PlotFSHistWindows(fileName)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

%Create 2D surface plot with the x axis as the range of the property being
%measured and the y axis as the incoming neutron energy, do this for both
%data sets and for the difference between the two
fileName=[fileName{:}];
fid = fopen(fileName,'rt','n');
idx = find(ismember(fileName,'./\:'),1,'last');
if fileName(idx) == '.'; fileName(idx:end) = []; end

if((fid>-1)&&(~feof(fid)))
    dashPos =  strfind(fileName,'\');
    isoName = fileName((dashPos(end)+1):end);
    dataTypes = {'Comparing the primary neutron x-y angular direction', 'Comparing the primary neutron z-xy angular direction','Comparing the secondary neutron x-y angular direction',...
                'Comparing the secondary neutron z-xy angular direction','Comparing the delayed neutron x-y angular direction','Comparing the delayed neutron z-xy angular direction',... 
                'Comparing the secondary photon x-y angular direction', 'Comparing the secondary photon z-xy angular direction','Comparing the primary neutron kinetic energy','Comparing the secondary neutron kinetic energy',...
                'Comparing the delayed neutron kinetic energy', 'Comparing the secondary photon kinetic energy', 'Comparing the secondary neutron yield', 'Comparing the delayed neutron yield',...
                'Comparing the secondary photon yield'};
    properties = {'Primary neutron x-y angular direction (Radians)', 'Primary neutron z-xy angular direction (Radians)','Secondary neutron x-y angular direction (Radians)',...
                'Secondary neutron z-xy angular direction (Radians)','Delayed neutron x-y angular direction (Radians)','Delayed neutron z-xy angular direction (Radians)',...
                'Secondary photon x-y angular direction (Radians)', 'Secondary photon z-xy angular direction (Radians)', 'Primary neutron kinetic energy (MeV)', 'Secondary neutron kinetic energy (MeV)',...
                'Delayed neutron kinetic energy (MeV)', 'Secondary photon kinetic energy (MeV)', 'Secondary neutron yield', 'Delayed neutron yield',...
                'Secondary photon yield'};
    libName = {'G4NDL', 'MCNP', 'Diff'};

    fgetl(fid);
    str = fgetl(fid);
    numEner = sscanf(str, '%d', 1);
    str = fgetl(fid);
    numBins = sscanf(str, '%d', 1);
    fgetl(fid);

    str = fgetl(fid);
    relevantData = sscanf(str, '%d', 15);
    fgetl(fid);

    numDataTypes=0;
    for i=1:1:15
        numDataTypes=numDataTypes+relevantData(i);
        if(relevantData(i)==1)
            if((i>8)&&(i<13))
                energyFlag(numDataTypes)=1;
            else
                energyFlag(numDataTypes)=0;
            end
            heading(numDataTypes)=dataTypes(i);
            yAxisName(numDataTypes)=properties(i);
            fileSubName(numDataTypes)=regexprep(properties(i),'[^\w'']','');
        end
    end
    
    %if you have a nvidia graphics card use the function gpuArrays to
    %create these arrays on your graphics cards for much faster rendering
    incEner = (zeros(1,numEner));
    yValues = (zeros(numBins+1, numDataTypes));
    yValuesAvg = (zeros(numBins));
    fsData = (zeros(numBins, numEner, numDataTypes,3));% G4NDL, MCNP, Diff
    hasData=1;
    
    incEner(1,1:numEner) = (fscanf(fid, '%g', numEner)).';
    incEner=log10(incEner);
    fgetl(fid);
    fgetl(fid);
    
    j=1;
%     fig = figure('position',[0 0 1600 900]);
%     set(fig,'visible','off');
    while (j<=numDataTypes)
        [fid, heading, j, numDataTypes, yValues, fileSubName, fsData, yAxisName, energyFlag] = Move2Data(fid, heading, j, numDataTypes, yValues, fileSubName, fsData, yAxisName, energyFlag);
        if(feof(fid))
            hasData=0;
            break;
        end
        fgetl(fid);
        yValues(1:numBins+1,j) = (fscanf(fid, '%g', numBins+1));
        for i = 1:1:numBins
            yValuesAvg(i) = (yValues(i,j)+yValues(i+1,j))/2;
        end
        
        takeVid=false;
%         if((strcmp(yAxisName{j},properties{9})~=1)||(strcmp(yAxisName{j},properties{10})~=1)||(strcmp(yAxisName{j},properties{11})~=1)||(strcmp(yAxisName{j},properties{13})~=1)||(strcmp(yAxisName{j},properties{14})~=1))
%             vid = VideoWriter(strcat(fileName,fileSubName{j},'.mp4'),'MPEG-4');
%             vid.FrameRate = 10;
%             vid.Quality = 100;
%             open(vid);
%             takeVid=true;
%         end
        for i = 1:1:numEner
            if(~feof(fid))                
                name = fgetl(fid);
                while ((strcmp(strcat(name),'G4NDL Hist')~=1)&&(~feof(fid)))
                    name = fgetl(fid);
                end
                fsData(1:numBins,i,j,1) = (fscanf(fid, '%g', numBins));
                name = fgetl(fid);
                while ((strcmp(strcat(name),'MCNP Hist')~=1)&&(~feof(fid)))
                    name = fgetl(fid);
                end
                fsData(1:numBins,i,j,2) = (fscanf(fid, '%g', numBins));
                name = fgetl(fid);
                while ((strcmp(strcat(name),'Diff Sq Hist')~=1)&&(~feof(fid)))
                    name = fgetl(fid);
                end
                fsData(1:numBins,i,j,3) = (fscanf(fid, '%g', numBins));
                if(takeVid)
                    subplot(2,1,1); plot(yValuesAvg,fsData(1:numBins,i,j,1),yValuesAvg,fsData(1:numBins,i,j,2));
                    title(strcat(isoName,fileSubName{j}));
                    legend('G4NDL Data', 'MCNP Data');
                    subplot(2,1,2); plot(yValuesAvg, fsData(1:numBins,i,j,3));
                    title('Difference Sqaured');
                    frame = getframe(fig);
                    writeVideo(vid,frame);
                    i
                end
            end
        end
        if(takeVid)
            close(vid);
        end
        j=j+1;
    end
    fclose(fid);
    clf;
    
    if(hasData==1)
        for j = 1:1:numDataTypes
            for k = 1:1:3
                set(gcf,'visible','off')
                if(sum(fsData(1:numBins,1:numEner,j,k))==0)
                    continue;
                end
                if(energyFlag(j))
                   %yValues(1:numBins,j)=log10(yValues(1:numBins,j));
                end
                if(yValues(1,j)==yValues(2,j))
                    yValues(:,j) = yValues(1,j):1/numBins:(yValues(1,j)+1);
                end
                imagesc(incEner,yValues(1:numBins,j),fsData(1:numBins,1:numEner,j,k),'CDataMapping','scaled');
                %surf(incEner,yValues(1:numBins,j),fsData(1:numBins,1:numEner,j,k))
                colormap jet;
                xlabel('Log of Incoming Neutron Energy (MeV)');
                ylabel(yAxisName(j));
                colorbar;
                saveas(gcf,strcat(fileName,libName{k},fileSubName{j},'.png'));
            end
        end
        
    end
else
    'Could not open file'
end
end

function [fid, heading, j, numDataTypes, yValues, fileSubName, fsData, yAxisName, energyFlag] = Move2Data(fid, heading, j, numDataTypes, yValues, fileSubName, fsData, yAxisName, energyFlag)
    test=1;
    jBeg=j;
    while((test)&&(~feof(fid)))
        name = fgetl(fid);
         for i=j:1:numDataTypes
             if(strcmp(strcat(name),heading(i))==1)
                 j=i;
                 test=0;
                 break;
             end
         end
    end
    if(jBeg<j)
        numDataTypes=numDataTypes-j+jBeg;
        heading(jBeg:j-1)=[];
        energyFlag(jBeg:j-1)=[];
        yValues(:,jBeg:j-1)=[];
        fileSubName(:,jBeg:j-1)=[];
        fsData(:,:,jBeg:j-1,:)=[];
        yAxisName(jBeg:j-1)=[];
        j=jBeg;
    end
end


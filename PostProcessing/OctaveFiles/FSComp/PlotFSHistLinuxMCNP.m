function PlotFSHistLinuxMCNP(fileName, FSName)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

%Create 2D surface plot with the x axis as the range of the property being
%measured and the y axis as the incoming neutron energy, do this for both
%data sets and for the difference between the two
fileName=[fileName{:}]
fflush(stdout);
fid = fopen(fileName,'rt','n');
idx = find(ismember(fileName,'./\:'),1,'last');
if fileName(idx) == '.'; fileName(idx:end) = []; end

if((fid>-1)&&(~feof(fid)))
    dashPos =  strfind(fileName,'/');
    isoName = fileName((dashPos(end)+1):end);
    dataTypes = {'Comparing the secondary neutron deflection angle','Comparing the delayed neutron deflection angle', 'Comparing the secondary photon deflection angle',...
		 'Comparing the secondary neutron kinetic energy','Comparing the delayed neutron kinetic energy', 'Comparing the secondary photon kinetic energy',...
		 'Comparing the secondary neutron yield', 'Comparing the delayed neutron yield','Comparing the secondary photon yield'};
     dataTypesTitle = {'Comparing The Secondary Neutron Deflection Angle','Comparing The Delayed Neutron Deflection Angle', 'Comparing The Secondary Photon Deflection Angle',...
		 'Comparing The Log of The Secondary Neutron Kinetic Energy','Comparing The Log of The Delayed Neutron Kinetic Energy', 'Comparing The Log of The Secondary Photon Kinetic Energy',...
		 'Comparing The Secondary Neutron Yield', 'Comparing The Delayed Neutron Yield','Comparing The Secondary Photon Yield'};
    properties = {'Secondary neutron deflection angle (Radians)','Delayed neutron deflection angle (Radians)','Secondary photon deflection angle (Radians)',...
		 'The Log of secondary neutron kinetic energy (MeV)','The Log of delayed neutron kinetic energy (MeV)', 'The Log of secondary photon kinetic energy (MeV)',...
		 'Secondary neutron yield', 'Delayed neutron yield', 'Secondary photon yield'};
    libName = {'G4NDL', 'MCNP', 'Diff'};


    fgetl(fid);
    str = fgetl(fid);
    numEner = sscanf(str, '%d', 1);
    str = fgetl(fid);
    numBins = sscanf(str, '%d', 1);
    fgetl(fid);

    str = fgetl(fid);
    relevantData = sscanf(str, '%d', 9);
    fgetl(fid);

    numDataTypes=0;
    for i=1:1:9
        numDataTypes=numDataTypes+relevantData(i);
        if(relevantData(i)==1)
            if((i>3)&&(i<7))
                energyFlag(numDataTypes)=1;
            else
                energyFlag(numDataTypes)=0;
            end
            heading(numDataTypes)=dataTypes(i);
            OneDTitles(numDataTypes)=dataTypesTitle(i);
            yAxisName(numDataTypes)=properties(i);
            fileSubName(numDataTypes)=regexprep(properties(i),'[^\w'']','');
        end
    end
    
    %if you have a nvidia graphics card use the function gpuArrays to
    %create these arrays on your graphics cards for much faster rendering
    incEner = (zeros(1,numEner));
    yValues = (zeros(numBins+1, numDataTypes));
    fsData = (zeros(numBins, numEner, numDataTypes,3));% G4NDL, MCNP, Diff
    hasData=1;
    
    incEner(1,1:numEner) = (fscanf(fid, '%g', numEner)).';
    incEner=log10(incEner);
    fgetl(fid);
    
    j=1;
%     fig = figure('position',[0 0 1600 900]);
%     set(fig,'visible','off');
    while (j<=numDataTypes)
        [fid, heading, j, numDataTypes, yValues, fileSubName, fsData, OneDTitles, yAxisName, energyFlag] = Move2Data(fid, heading, j, numDataTypes, yValues, fileSubName, fsData, OneDTitles, yAxisName, energyFlag);
        if(feof(fid))
            hasData=0;
            break;
        end
        fgetl(fid);
        yValues(1:numBins,j) = (fscanf(fid, '%g', numBins));
        
        takeVid=false;
%         if((strcmp(yAxisName{j},properties{9})~=1)||(strcmp(yAxisName{j},properties{10})~=1)||(strcmp(yAxisName{j},properties{11})~=1)||(strcmp(yAxisName{j},properties{13})~=1)||(strcmp(yAxisName{j},properties{14})~=1))
%             vid = VideoWriter(strcat(fileName,fileSubName{j},'.mp4'),'MPEG-4');
%             vid.FrameRate = 10;
%             vid.Quality = 100;
%             open(vid);
%             takeVid=true;
%         end
        max=0;
        maxi=1;
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
                    subplot(2,1,1); plot(yValues,fsData(1:numBins,i,j,1),yValues,fsData(1:numBins,i,j,2));
                    title(strcat(isoName,fileSubName{j}));
                    legend('G4NDL Data', 'MCNP Data');
                    subplot(2,1,2); plot(yValues, fsData(1:numBins,i,j,3));
                    title('Difference Sqaured');
                    frame = getframe(fig);
                    writeVideo(vid,frame);
                end
                if(sum(fsData(1:numBins,i,j,3))>max)
                  max=sum(fsData(1:numBins,i,j,3));
                  maxi=i;
                end
            end
        end
%        if(~(fsData(1:numBins,1:numEner,j,3)))
        if(yValues(1,j)==yValues(2,j))
            yValues(:,j) = yValues(1,j):1/numBins:(yValues(1,j)+1);
        end
        incEnerStr=num2str(10^incEner(1,maxi));
%              hist(yValues(:,j),fsData(1:numBins,maxi,j,1));
        subplot(2,1,1); plot(yValues(1:numBins,j),fsData(1:numBins,maxi,j,1),yValues(1:numBins,j),fsData(1:numBins,maxi,j,2));
        title(strcat(OneDTitles{j},' For The',{' '},FSName,' of',{' '},isoName,' at',{' '},incEnerStr,'MeV'));
        ylabel('Normalized Counts');
        xlabel(yAxisName(j));
%        legend('G4NDL Data', 'MCNP Data','Location','bestoutside');
%              Location(loc,'East');
        subplot(2,1,2); plot(yValues(1:numBins,j), abs(fsData(1:numBins,maxi,j,1)-fsData(1:numBins,maxi,j,2)));
        title('Absolute Difference Between the Histograms');
        ylabel('Normalized Counts');
        xlabel(yAxisName(j));
%              legend('G4NDL Data', 'MCNP Data','Location','best');
        saveas(gcf,strcat(fileName,'MaxDiff',fileSubName{j},'.png'));
%        end
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
                %set(gcf,'visible','off')
                if(~(fsData(1:numBins,1:numEner,j,k)))
                    continue;
                end
                if(energyFlag(j))
                   %yValues(1:numBins,j)=log10(yValues(1:numBins,j));
                end
                if(yValues(1,j)==yValues(2,j))
                    yValues(:,j) = yValues(1,j):1/numBins:(yValues(1,j)+1);
                end
                %imagesc(incEner,yValues(1:numBins,j),fsData(1:numBins,1:numEner,j,k));
		            imagesc([incEner(1),incEner(numEner)],[yValues(1,j),yValues(numBins,j)],fsData(1:numBins,1:numEner,j,k));
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
    fflush(stdout);
end
end

function [fid, heading, j, numDataTypes, yValues, fileSubName, fsData, OneDTitles, yAxisName, energyFlag] = Move2Data(fid, heading, j, numDataTypes, yValues, fileSubName, fsData, OneDTitles, yAxisName, energyFlag)
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
        OneDTitles(jBeg:j-1)=[];
        yAxisName(jBeg:j-1)=[];
        j=jBeg;
    end
end


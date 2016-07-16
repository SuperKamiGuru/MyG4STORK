close all
clear all;
clc


%Create 2D surface plot with the x axis as the range of the property being
%measured and the y axis as the incoming neutron energy, do this for both
%data sets and for the difference between the two

Directory = '/home/SuperKamiGuru/NuclearSim/EnergyDist/';
filePartName = {'EnergyDis42100.txt','EnergyDis42100Slave1.txt','EnergyDis42100Slave2.txt','EnergyDis42100Slave3.txt','EnergyDis42100Slave4.txt'};
hasData=1;

for i=1:1:5
  fileName=strcat(Directory,filePartName(i));
  fileName=[fileName{:}]
  fflush(stdout);
  fid = fopen(fileName,'rt','n');

  if((fid>-1)&&(~feof(fid)))
      fgetl(fid);
      str = fgetl(fid);
      numEner = sscanf(str, '%d', 1);
      str = fgetl(fid);
      numBins = sscanf(str, '%d', 1);
      fgetl(fid);

      count=i;
      if(i==1)
        temp = zeros(numBins);
        incEner = (zeros(1,numEner));
        fsDataTemp = (zeros(numBins, numEner));
        fsData = (zeros(numBins, numEner,3));% MCNP, G4NDL, Diff
      else
        count=2;
      end
      
      incEner(1,1:numEner) = (fscanf(fid, '%g', numEner)).';

      for j=1:1:numEner
        temp = (fscanf(fid, '%g', numBins));
        fsDataTemp(1:numBins,j) = temp;
      end
      fsData(:,:,count)=fsData(:,:,count)+fsDataTemp(:,:); 
      
      fclose(fid);
      clf;
  else
      hasData=0;
      'Could not open file'
      fflush(stdout);
  end
end

for k=1:1:2
  for i=1:1:numEner
    Size=sum(fsData(:,i,k));
    if(Size>0)
      fsData(:,i,k)=fsData(:,i,k)./Size;
    end
  end
end

fsData(:,:,3)=fsData(:,:,1)-fsData(:,:,2);

if(hasData==1)
    for k = 1:1:3
        %set(gcf,'visible','off')
        if(~(fsData(1:numBins,1:numEner,k)))
            continue;
        end
        imagesc([incEner(1),incEner(numEner)],[incEner(1),incEner(numEner)],fsData(1:numBins,1:numEner,k));
        colormap jet;
        xlabel('Log of Incoming Neutron Energy (MeV)');
        ylabel('Log of Out-going Neutron Energy (MeV)');
        colorbar;
        if(k==1)
          saveas(gcf,strcat(Directory,'EnergyDist42100MCNP.png'));
        elseif(k==2)
          saveas(gcf,strcat(Directory,'EnergyDist42100G4NDL.png'));
        else
          saveas(gcf,strcat(Directory,'EnergyDist42100Diff.png'));
        end
    end
end



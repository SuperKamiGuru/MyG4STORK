close all
clear all;
clc


%Create 2D surface plot with the x axis as the range of the property being
%measured and the y axis as the incoming neutron energy, do this for both
%data sets and for the difference between the two

fileName = '/home/SuperKamiGuru/NuclearSim/EnergyDist/SampleEnerHigh.txt';
hasData=1;

fflush(stdout);
  fid = fopen(fileName,'rt','n');

  if((fid>-1)&&(~feof(fid)))
      str = fgetl(fid);
      numEner = sscanf(str, '%d', 1);

      outEner = (zeros(1,numEner));
      outProb = (zeros(1,numEner));

      for j=1:1:numEner
        outEner(j) = log10(fscanf(fid, '%g', 1));
        outProb(j) = (fscanf(fid, '%g', 1));
      end 
      
      fclose(fid);
      clf;
  else
      hasData=0;
      'Could not open file'
      fflush(stdout);
  end

if(hasData==1)
    plot(outEner,outProb)
    saveas(gcf,strcat(fileName,'.png'));
end



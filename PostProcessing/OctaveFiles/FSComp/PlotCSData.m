function PlotCSData(fileName, Directory)

  %Create 2D surface plot with the x axis as the range of the property being
%measured and the y axis as the incoming neutron energy, do this for both
%data sets and for the difference between the two

  hasData=1;

  for i=1:1:2
  
    if(i==2)
    
      pos1=strfind(fileName,'293.606K/');
      pos2=strfind(fileName,'_');
      fileName=strcat(Directory, fileName((pos1(1)+9):(pos2(2)-1)), '.txt')
      
    end
    fflush(stdout);
    fid = fopen(fileName,'rt','n');

    if((fid>-1)&&(~feof(fid)))
        str = fgetl(fid);
        
        if(i==1)
          strEnd=strfind(fileName,'Elastic/');
          if(size(strEnd)~=0)
            str = fgetl(fid);
            str = fgetl(fid);
          end
          strEnd=strfind(fileName,'Capture/');
          if(size(strEnd)~=0)
            str = fgetl(fid);
            str = fgetl(fid);
          end
        end
        numEner = sscanf(str, '%d', 1);
        if(i==1)
          incEner1 = zeros(1,numEner);
          csData1 = zeros(1,numEner);
          for j=1:1:numEner
            temp=(fscanf(fid, '%g', 1));
            if(!isnumeric(temp))
              break;
            end
            incEner1(1,j) = temp(1,1);
            incEner1(1,j) = log10(incEner1(1,j));
            csData1(1,j) = (fscanf(fid, '%g', 1));
          end
        else
          incEner2 = zeros(1,numEner);
          csData2 = zeros(1,numEner);
          for j=1:1:numEner
            incEner2(1,j) = (fscanf(fid, '%g', 1));
            incEner2(1,j) = log10(incEner2(1,j));
            csData2(1,j) = (fscanf(fid, '%g', 1));
          end
        end
       
        fclose(fid);
    else
        'Could not open file'
        break;
    end
  end
  
  if(incEner2(1,length(incEner2))>incEner1(1,length(incEner1)))
    incEner1(1,length(incEner1)+1)=incEner2(1,length(incEner2));
    csData1(1,length(csData1)+1)=csData1(1,length(csData1));
  else
    incEner2(1,length(incEner2)+1)=incEner1(1,length(incEner1));
    csData2(1,length(csData2)+1)=csData2(1,length(csData2));
  end
  
  for i=1:1:2
      if(i==2)
        plot(incEner2(1,:),csData2(1,:),'-b','LineWidth', 1.0)
      else
        plot(incEner1(1,:),csData1(1,:),'-r','LineWidth', 2.0)
        xlabel('Log10 of the Incoming Neutron Energy (eV)');
        ylabel('CrossSection (barns)');
      end
      
      hold on;
  end
  hold off;

  legend('ENDF Data','MCNP Data');
  pos2=strfind(fileName,'.txt');
  fileName=fileName(1:pos2(1));
  fileName=strcat(fileName,'png');
  saveas(gcf,fileName);

end

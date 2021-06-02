open('C:\Users\ali haider\Desktop\imageB.bmp');
fid=fopen('PIXELS.txt','w');

for i=1:110
    for j=1:77
        fprintf(fid,"%d ",cdata(i,j));
    end
        fprintf(fid,"\n");
end
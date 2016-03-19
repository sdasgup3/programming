m=10000;

tic()
a=zeros(m,m);
b=zeros(m,m);
c=zeros(m,m);
toc()

tic()
for i=1:m
   for j=1:m
	 a[i,j] = i*j; 
   end
end
toc()

tic()
for i=1:m
   for j=1:m
	 b[i,j]= i+j;
   end
end
toc()



tic()
for i=1:m
   for j=1:m
	 c[i,j]= a[i,j] + b[i,j];
   end
end
toc()

n=10000000;
v=rand(1,n);
sum1=0;
clear sum

%With Jit
feature accel on 
tic
for ii=1:n
    sum1 = sum1 +  v(ii);
end
toc
fprintf(' With Jit ON: Sum:%d \n\n', sum1);


%With Jit Off
feature accel off 
sum1=0;
tic
for ii=1:n
    sum1 = sum1 +  v(ii);
end
disp('With Jit Off')
toc
fprintf(' With Jit oFF: Sum:%d \n\n', sum1);


tic
sum2 = sum(v);
toc
fprintf(' With vector sum: Sum:%d \n\n', sum2);
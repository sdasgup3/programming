N=50;
R=10000;
w1=zeros(1,N);
w2=zeros(1,N);
x=randi(R,1, N);

tic
for ii=1:N
    for jj=1:N
        w1(ii)=w1(ii)+x(ii)+x(jj);
    end
end
toc


tic
a=repmat(x',1,N) + repmat(x,N,1);
w2=sum(a);
toc
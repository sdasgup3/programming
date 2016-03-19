m=1000000
println("One time allocation")
tic(); b=zeros(1,m); toc();

println("Iterative Allocation")
tic();
for i=1:m
  a(i)=0;
end
toc()

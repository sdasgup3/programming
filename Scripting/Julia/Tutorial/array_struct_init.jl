gc();
N = 10;

type Foo
  baz::Array{Int64,2}
  qux::Float64
end
        
fooA = [Foo([0 0],0.0) for i in 1:N]

for ii=1:N
  fooA[ii].baz= [ii ii+1];    
end

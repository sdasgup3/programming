> include("fileName.jl")
> tic(); m_non_vec(1600); toc()


~/Install/julia-0.4.0/julia  --track-allocation=user // WWhen  it exit dumps a .mem file
>@time m_vec(1600);

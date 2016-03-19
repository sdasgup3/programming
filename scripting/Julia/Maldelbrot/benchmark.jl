function benchmark()

    # Run the julia with --track-allocation=all/user	
	
    # Run once, to force compilation.
    println("======================= First run:")
    srand(666)
    @time mandelbrot_vectorization(160)

    # Run a second time, with profiling.
    println("\n\n======================= Second run:")
    srand(666)
    Profile.init(delay=0.01)
    Profile.clear()
    Profile.clear_malloc_data()
    @profile @time mandelbrot_vectorization(160)

    # Write profile results to profile.bin.
    r = Profile.retrieve()
    f = open("profile.bin", "w")
    serialize(f, r)
    close(f)
end

#@profile (for i = 1:100; myfunc(); end)
include("mandelbrot_vectorization.jl")
mandelbrot_vectorization(16)
~/Install/julia-0.4.0/julia  --track-allocation=user


type packedD
  v2df::Array{Float64,1}
end

function calc_row(y::Int64,  N::Int64, bytes_per_row::Int64, 
		Crvs::Array{packedD,1}, inverse_h::Float64, bitmap::Array{UInt8,2},
		zero::Array{Float64,1}, four::Array{Float64,1})
    
    row_bitmap::Int64 = (bytes_per_row * y)  + 1;
    Civ_init::Array{Float64,1} = [ y*inverse_h-1.0, y*inverse_h-1.0 ];

    for x=0:2:N-2
        Crv::Array{Float64,1} = Crvs[(x >> 1) + 1].v2df;
        Civ::Array{Float64,1} = Civ_init;
        Zrv::Array{Float64,1} = zero;
        Ziv::Array{Float64,1} = zero;
        Trv::Array{Float64,1} = zero;
        Tiv::Array{Float64,1} = zero;
        
	ii::Int64 = 50;
	two_pixels::Int64 = 1;

        while ii > 0  && two_pixels != 0 
            Ziv = (Zrv .* Ziv) + (Zrv .* Ziv) + Civ;
            Zrv = Trv - Tiv + Crv;
            Trv = Zrv .* Zrv;
            Tiv = Ziv .* Ziv;
      
            is_still_bounded::Array{Bool,1} = (Trv + Tiv) .<  four;

            two_pixels = 2*is_still_bounded[2] + is_still_bounded[1];
            ii = ii - 1;
        end

        
        two_pixels = (two_pixels <<  6);
	posn::Int64 = row_bitmap + (x  >> 3);
	value::UInt8 = UInt8( (two_pixels  >> (x & 7)));
        bitmap[posn] =   bitmap[posn] | value; 
    end
end


function  mandelbrot_non_vectorized(N::Int64)
#N=16;
    bytes_per_row::Int64 = (N + 7) >>  3;

    inverse_w::Float64 = 2.0 / (bytes_per_row <<  3);
    inverse_h::Float64 = 2.0 / N;

    zero = Float64[];
    four = Float64[];
    zero = [ 0.0, 0.0 ];
    four = [ 4.0, 4.0 ];
      
    Crvs::Array{packedD,1} = [packedD([0.0 , 0.0]) for i in 1:N/2]
    bitmap::Array{UInt8,2} = zeros(UInt8, bytes_per_row, N);

    for ii::Int64 = 1:1:N/2 
        Crv::Array{Float64,1} = [ ((2*ii-2) + 1.0)*inverse_w - 1.5, (2*ii-2)*inverse_w - 1.5 ];
        Crvs[ii].v2df = Crv;
    end

#for ii = 1:1:N/2 
#       println (Crvs[ii].v2df); 
#   end
   
    for ii::Int64 = 1:1:N
        calc_row(ii - 1, N, bytes_per_row, Crvs, inverse_h, bitmap, zero, four);
    end
#println(bitmap);

    fid = open("image_non_vec.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
end


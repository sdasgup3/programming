type packedD
  v2df::Array{Float64,1}
end
      
function calc(bitmap)
bitmap[1] = 5;
bitmap[2] = 6;
	end

function XXX(N)
    bitmap = zeros(2, 3);
    calc(bitmap[1:2]);

end


function calc_row(y::Int64, N::Int64, bytes_per_row::Int64, 
		Crv::Array{Float64,2}, inverse_h::Float64, bitmap,
		zero::Array{Float64,2}, vec_four::Array{Float64,2},
		vec_size::Int64, vec_size_for_reduction::Int64,X)
   
    Civ_init::Array{Float64,2} = [ y*inverse_h-1.0 y*inverse_h-1.0 ];
    row_bitmap = Int64((bytes_per_row * y)  + 1);
    
    
        
    Civ = repmat(Civ_init, vec_size,1);
    Zrv = repmat(zero, vec_size,1);
    Ziv = repmat(zero, vec_size,1);
    Trv = repmat(zero, vec_size,1);
    Tiv = repmat(zero, vec_size,1);

    for ii= 1:1:50 
        Ziv = (Zrv .* Ziv) + (Zrv .* Ziv) + Civ;
        Zrv = Trv - Tiv + Crv;
        Trv = Zrv .* Zrv;
        Tiv = Ziv .* Ziv;
      
        is_still_bounded = (Trv + Tiv) .<  vec_four;

        two_pixels = 2*is_still_bounded[:,2] + is_still_bounded[:,1];
    end
     
    two_pixels = (two_pixels .<<  6);
    posn = row_bitmap + (X  .>> 3);
    value =  (two_pixels  .>> (X & 7));

    #Reduction of vec_size_for_reduction value for each
    #of bytes_per_row locations
    
    reshaped_value = (reshape(value, vec_size_for_reduction, bytes_per_row)).';
    reshaped_posn  = (reshape(posn,  vec_size_for_reduction, bytes_per_row)).';
    
    for ii=1:1:vec_size_for_reduction
        index = reshaped_posn[:,ii];
        bitmap[index] =   bitmap[index] | reshaped_value[:,ii]; 
    end
end

function  mandelbrot_vectorization(N::Int64)
    println("N = " , N); 	
    bytes_per_row::Int64 = (N + 7) >>  3;
#bytes_per_row::Int64 = (N + 7) >>  3;

    inverse_w = 2.0 / (bytes_per_row <<  3);
    inverse_h = 2.0 / N;
#inverse_w::Float64 = 2.0 / (bytes_per_row <<  3);
#    inverse_h::Float64 = 2.0 / N;

    zero = Float64[];
    four = Float64[];
    zero = [ 0.0 0.0 ];
    four = [ 4.0 4.0 ];

    bitmap = zeros(UInt8, bytes_per_row, N);
    vec_size = Int64(N/2); 
    vec_size_for_reduction = Int64(N / (2*bytes_per_row)); 
    vec_four = repmat(four,vec_size,1);
#   bitmap::Array{UInt8,2} = zeros(UInt8, bytes_per_row, N);
#    vec_size::Int64 = N/2; 
#    vec_size_for_reduction::Int64 = N / (2*bytes_per_row); 
#    vec_four::Array{Float64,2} = repmat(four,vec_size,1);

#Crv::Array{Float64,2} = repmat([0.0 0.0], vec_size,1);
#   X::Array{Int64,1} = [Int64(i) for i in 0:2:N-2];
    Crv = repmat([0.0 0.0], vec_size,1);
    X = [Int64(i) for i in 0:2:N-2];

    for ii::Int64 = 1:1:N/2 
	Crv[ii] 		= ((2*ii-2) + 1.0)*inverse_w - 1.5;
	Crv[ii + vec_size ] 	= (2*ii-2)*inverse_w - 1.5;
    end
    
    for ii = 0:1:N-1
        calc_row(ii, N, bytes_per_row, Crv, inverse_h, bitmap, zero, vec_four, vec_size, vec_size_for_reduction,X);
    end
    
#println(bitmap);
    fid = open("image_vec.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
end

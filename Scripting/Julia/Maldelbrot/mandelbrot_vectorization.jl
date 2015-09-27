function calc_row(y::Int64, N::Int64, bytes_per_row::Int64, 
		Crv::Array{Float64,2}, inverse_h::Float64, bitmap,
		vec_four::Array{Float64,2},
		vec_size::Int64, vec_size_for_reduction::Int64,X,
			Civ, Zrv, Ziv, Trv, Tiv, Tmp, Tmp2, Tmp_3, is_still_bounded, two_pixels,
			posn, value);
   
    Civ_init = y*inverse_h-1.0;
    row_bitmap = Int64((bytes_per_row * y)  + 1);
    
    fill!(Civ, Civ_init);
    fill!(Zrv, 0.0);
    fill!(Ziv, 0.0);
    fill!(Trv, 0.0);
    fill!(Tiv, 0.0);

    for ii= 1:1:50 
        broadcast!(*,Tmp,Zrv,Ziv );
	broadcast!(*,Tmp,Tmp,2);
        broadcast!(+,Ziv,Tmp,Civ );

        broadcast!(-,Tmp,Trv,Tiv );
        broadcast!(+,Zrv,Tmp,Crv );

        broadcast!(*,Trv,Zrv,Zrv );
        broadcast!(*,Tiv,Ziv,Ziv );
      
        broadcast!(+,Tmp,Trv,Tiv );
        broadcast!(<,is_still_bounded,Tmp,vec_four );

	broadcast!(*,Tmp_3, is_still_bounded,[2 1]);
	sum!(two_pixels,Tmp_3);

    end

     
    broadcast!(<<,two_pixels,two_pixels,6)

    broadcast!(&,Tmp2, X,7)
    broadcast!(>>,value, two_pixels,Tmp2)

    #Reduction of vec_size_for_reduction value for each
    #of bytes_per_row locations
    
    for p in 1:1: bytes_per_row 
        startI = 1 + (p-1)*vec_size_for_reduction;
	endI = startI + vec_size_for_reduction - 1;
	bitmap[row_bitmap + p - 1] = reduce(|, value[startI:endI]);
     end
end

function  mandelbrot_vectorization2(N::Int64)
#println("N = " , N); 	
    bytes_per_row = Int64((N + 7) >>  3);

    inverse_w = 2.0 / (bytes_per_row <<  3);
    inverse_h = 2.0 / N;

    vec_size = Int64(N/2); 
    vec_size_for_reduction = Int64(N / (2*bytes_per_row)); 

    Civ = Array(Float64, vec_size,2);
    Zrv = Array(Float64, vec_size,2);
    Ziv = Array(Float64, vec_size,2);
    Trv = Array(Float64, vec_size,2);
    Tiv = Array(Float64, vec_size,2);
    vec_four = 	Array(Float64, vec_size,2);
    Crv = Array(Float64, vec_size,2);
    Tmp = Array(Float64, vec_size,2);
    Tmp2 = Array(Int64, vec_size,1);
    Tmp_3 = Array(Int64, vec_size,2);
    is_still_bounded = Array(Bool, vec_size,2);
    two_pixels = Array(Int64, vec_size,1);
    posn = Array(Int64, vec_size,1);
    value = Array(UInt8, vec_size,1);

    bitmap = zeros(UInt8, bytes_per_row, N);
    X = [Int64(i) for i in 0:2:N-2];
    fill!(vec_four, 4.0);
    for ii::Int64 = 1:1:N/2 
	Crv[ii] 		= ((2*ii-2) + 1.0)*inverse_w - 1.5;
	Crv[ii + vec_size ] 	= (2*ii-2)*inverse_w - 1.5;
    end

    
for ii::Int64 = 0:1:N-1
        calc_row(ii, N, bytes_per_row, Crv, inverse_h, bitmap, vec_four, vec_size, vec_size_for_reduction,X,
			Civ, Zrv, Ziv, Trv, Tiv, Tmp, Tmp2, Tmp_3,is_still_bounded,two_pixels,posn, value );
    end
    
#println(bitmap);
    fid = open("image_vec.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
end

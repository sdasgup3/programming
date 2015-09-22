type packedD
  v2df::Array{Float64,1}
end

function calc_row(y,  N, bytes_per_row, Crvs, inverse_h, bitmap)
    zero = [ 0.0, 0.0 ];
    four = [ 4.0, 4.0 ];
    
    row_bitmap = (bytes_per_row * y)  + 1;
    Civ_init = [ y*inverse_h-1.0, y*inverse_h-1.0 ];

    for x=0:2:N-2
        Crv = Crvs[(x >> 1) + 1].v2df;
        Civ = Civ_init;
        Zrv = zero;
        Ziv = zero;
        Trv = zero;
        Tiv = zero;
        
        ii = 50;
        two_pixels = 1;

        while ii > 0  && two_pixels != 0 
            Ziv = (Zrv .* Ziv) + (Zrv .* Ziv) + Civ;
            Zrv = Trv - Tiv + Crv;
            Trv = Zrv .* Zrv;
            Tiv = Ziv .* Ziv;
      
            is_still_bounded = (Trv + Tiv) .<  four;

            two_pixels = 2*is_still_bounded[2] + is_still_bounded[1];
            ii = ii - 1;
        end

        
        two_pixels = (two_pixels <<  6);
        posn = row_bitmap + (x  >> 3);
        value = uint8 ( (two_pixels  >> (x & 7)));
        bitmap[posn] =   bitmap[posn] | value; 
    end
end


#function  mandelbrot_non_vectorized(N)
tic();
N=1600;
    println ("N = " , N);
    bytes_per_row = (N + 7) >>  3;

    inverse_w = 2.0 / (bytes_per_row <<  3);
    inverse_h = 2.0 / N;

      
    Crvs = [packedD([0.0 , 0.0]) for i in 1:N/2]

    for ii = 1:1:N/2 
        Crv = [ ((2*ii-2) + 1.0)*inverse_w - 1.5, (2*ii-2)*inverse_w - 1.5 ];
        Crvs[ii].v2df = Crv;
    end

#for ii = 1:1:N/2 
#       println (Crvs[ii].v2df); 
#   end
   
    bitmap = zeros(Uint8, bytes_per_row, N);
    for ii = 1:1:N
        calc_row(ii - 1, N,bytes_per_row, Crvs, inverse_h, bitmap);
    end
    #println(bitmap);

    fid = open("image_non_vec.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
#end
    toc();


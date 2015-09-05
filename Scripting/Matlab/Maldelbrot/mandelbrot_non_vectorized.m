function  mandelbrot_non_vectorized
    global N ;
    global bytes_per_row;
    global Crvs;
    global inverse_h;
    global bitmap;
    
    N = 160;
    bytes_per_row = bitshift(N + 7, -3);

    inverse_w = 2.0 / bitshift(bytes_per_row, 3);
    inverse_h = 2.0 / N;


    Crvs = repmat(struct('v2df',[double(0),double(0)]), N/2, 1);

    for ii = 2:2:N 
        Crv = [ ((ii-2)+1.0)*inverse_w - 1.5, (ii-2)*inverse_w - 1.5 ];
        Crvs(bitshift(ii,-1)).v2df = Crv;
    end

    %for ii = 1:1:N/2 
    %    Crvs(ii).v2df 
    %end
   
    bitmap = zeros(bytes_per_row, N);
   
    for ii = 1:1:N
        calc_row(ii - 1);
    end
    %bitmap
   
    fid = fopen('image.pbm','w');
    fprintf(fid, 'P4\n%d %d\n', N, N);
    fwrite(fid, bitmap, 'uint8');
    fclose(fid);
end

function calc_row(y)
    global N;
    global bytes_per_row;
    global Crvs;
    global inverse_h;
    global bitmap;
    
    
    zero = [ 0.0, 0.0 ];
    four = [ 4.0, 4.0 ];
    
    row_bitmap = (bytes_per_row * y)  + 1;
    Civ_init = [ y*inverse_h-1.0, y*inverse_h-1.0 ];

    for x=0:2:N-2
        Crv = Crvs(bitshift(x,-1) + 1).v2df;
        Civ = Civ_init;
        Zrv = zero;
        Ziv = zero;
        Trv = zero;
        Tiv = zero;
        
        ii = 50;
        two_pixels = 1;

        while ii > 0  && two_pixels 
            Ziv = (Zrv .* Ziv) + (Zrv .* Ziv) + Civ;
            Zrv = Trv - Tiv + Crv;
            Trv = Zrv .* Zrv;
            Tiv = Ziv .* Ziv;
      
            is_still_bounded = (Trv + Tiv) <  four;

            two_pixels = 2*is_still_bounded(2) + is_still_bounded(1);
            ii = ii - 1;
        end

        
        two_pixels = bitshift(two_pixels, 6);
        posn = row_bitmap + bitshift(x, -3);
        value = uint8 ( bitshift(two_pixels, -bitand(x, 7)));
        bitmap(posn) = bitor ( bitmap(posn), value); 
    end
end






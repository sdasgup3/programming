function  mandelbrot_parfor
    clear;
    N = 16000
    
    bytes_per_row = bitshift(N + 7, -3);

    inverse_w = 2.0 / bitshift(bytes_per_row, 3);
    inverse_h = 2.0 / N;

    Crvs = repmat(struct('v2df',[double(0),double(0)]), N/2, 1);
    bitmap = repmat(struct('v2i',zeros(bytes_per_row, 1)), N, 1);

    parfor ii = 1:1:N/2 
        Crv = [ ((2*ii-2) + 1.0)*inverse_w - 1.5, (2*ii-2)*inverse_w - 1.5 ];
        Crvs(ii).v2df = Crv;
    end
    
    handleFunc = @calc_row;
    parfor ii = 0:1:N-1
        bitmap(ii+1).v2i  = feval(handleFunc, ii, N, bytes_per_row, Crvs, inverse_h);
    end
    
    fid = fopen('image_parfor.pbm','w');
    fprintf(fid, 'P4\n%d %d\n', N, N);
    for ii = 1:1:N
        fwrite(fid, bitmap(ii).v2i, 'uint8'); 
    end
    fclose(fid);
end

function bitmapslice = calc_row(y,N, bytes_per_row, Crvs, inverse_h)
    zero = [ 0.0, 0.0 ];
    four = [ 4.0, 4.0 ];
    bitmapslice = zeros(bytes_per_row, 1);
    
 
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

        %Here the check for two_pixels can be safely removed because
        %once two_pixels got a value 0, it will remain 0 for the rest of
        %the iteration of ii.
        %Removing two_pixels has the benefit that we can now 
        %vectorize the loop, but the drawback is we cannot make
        %an early termination of the loop.
        
        while ii > 0  %&& two_pixels  
            Ziv = (Zrv .* Ziv) + (Zrv .* Ziv) + Civ;
            Zrv = Trv - Tiv + Crv;
            Trv = Zrv .* Zrv;
            Tiv = Ziv .* Ziv;
      
            is_still_bounded = (Trv + Tiv) <  four;

            two_pixels = 2*is_still_bounded(2) + is_still_bounded(1);
            ii = ii - 1;
        end
        
        two_pixels = bitshift(two_pixels, 6);
        
        posn = bitshift(x, -3) + 1;
        value = uint8 ( bitshift(two_pixels, -bitand(x, 7)));
        bitmapslice(posn) = bitor ( bitmapslice(posn), value); 
    end
end






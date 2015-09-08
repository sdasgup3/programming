function  mandelbrot_par_n_vector(N)
    N 
    
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
    
    fid = fopen('image_vec.pbm','w');
    fprintf(fid, 'P4\n%d %d\n', N, N);
    for ii = 1:1:N
        fwrite(fid, bitmap(ii).v2i, 'uint8'); 
        %bitmap(ii).v2i
    end
    fclose(fid);
end

function bitmapslice = calc_row(y,N, bytes_per_row, Crvs, inverse_h)
    zero = [ 0.0, 0.0 ];
    four = [ 4.0, 4.0 ];
    bitmapslice = uint8(zeros(bytes_per_row, 1));
   
    Civ_init = [ y*inverse_h-1.0, y*inverse_h-1.0 ];
    
    vec_size = N/2; 
    vec_size_for_reduction = N / (2*bytes_per_row); 
    
    vec_four = repmat(four,vec_size,1);
    Crv = repmat([double(0),double(0)], vec_size,1);
    X = zeros(vec_size,1);
        
    temp = 0;
    for ii=1:1:vec_size 
        Crv([ii (ii+vec_size)]) = Crvs(bitshift(temp,-1) + 1).v2df;
        X(ii) = temp;
        temp = temp +2;    
    end
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
      
        is_still_bounded = (Trv + Tiv) <  vec_four;

        two_pixels = 2*is_still_bounded(:,2) + is_still_bounded(:,1);
    end
        
    two_pixels = bitshift(two_pixels, 6);
        
    posn = bitshift(X, -3) + 1;
    value = uint8 ( bitshift(two_pixels, -bitand(X, 7)));
    
    %Reduction of vec_size_for_reduction value for each
    %of bytes_per_row locations
    
    reshaped_value = (reshape(value, vec_size_for_reduction, ...
                        bytes_per_row)).';
    reshaped_posn  = (reshape(posn,  vec_size_for_reduction, ...
                        bytes_per_row)).';
    
    for ii=1:1:vec_size_for_reduction
        index = reshaped_posn(:,ii);
        bitmapslice(index) = bitor ( bitmapslice(index), reshaped_value(:,ii)); 
    end
end






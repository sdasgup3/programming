# This version will not work as 
# the bitmap array is not a array of 
# bytes as required by m_non_vec

function  m_non_vec_v2(N::Int64)
    LIMIT_SQUARED = 4.0;
    MAX_ITER = 50;
    image_Width_And_Height = Int64(((N + 7) >>  3) << 3 );
    inverse_w  = 2.0 / image_Width_And_Height;
    inverse_h  = 2.0 / image_Width_And_Height;
    
#bytes_per_row = (N + 7) >>  3;

    bitmap = ones(UInt8, image_Width_And_Height, image_Width_And_Height);
    fill!(bitmap,1);

    initial_r = Array(Float64, image_Width_And_Height,  1);
    initial_i = Array(Float64, image_Width_And_Height,  1);

    for ii in 1: image_Width_And_Height
      initial_r[ii]= inverse_w* (ii - 1 ) - 1.5;
      initial_i[ii]= inverse_h* (ii - 1) - 1.0;
    end

    for ii in 1: image_Width_And_Height
      prefetched_Initial_i=initial_i[ii];

      for jj in 1: image_Width_And_Height

        cr  = initial_r[jj];
	ci = prefetched_Initial_i;
	zr = 0.0;
	zi = 0.0;
	tr = 0.0;
	ti = 0.0;

	for iter = 1 : MAX_ITER
	  zi = 2.0 * (zr * zi) + ci;
	  zr = tr - ti + cr;
	  tr = zr * zr;
	  ti = zi *zi;
	  if (tr + ti)  > LIMIT_SQUARED
	    bitmap[jj,ii] = 0;
            break;
	  end  
	end

      end	
    end    
println(bitmap);

    fid = open("non_vec_v2.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
 end

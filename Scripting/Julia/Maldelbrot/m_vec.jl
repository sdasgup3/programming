function  m_vec(N::Int64)
    LIMIT_SQUARED = 4.0;
    MAX_ITER = 50;

    image_Width_And_Height = Int64(((N + 7) >>  3) << 3 );
    inverse_w  = 2.0 / image_Width_And_Height;
    inverse_h  = 2.0 / image_Width_And_Height;
    bytes_per_row = Int64((N + 7) >>  3);

    bitmap = ones(UInt8, bytes_per_row, image_Width_And_Height);
    fill!(bitmap,0);

    initial_r = Array(Float64, image_Width_And_Height);
    initial_i = Array(Float64, image_Width_And_Height);

    II = collect(0:image_Width_And_Height-1);
    initial_r = inverse_w * II - 1.5;
    initial_i = inverse_h * II - 1.0;

    shift = repmat(collect(7:-1:0), bytes_per_row,1);
    bit_mask = Array(UInt8,image_Width_And_Height);

    Zrv = Array(Float64, image_Width_And_Height);
    Ziv = Array(Float64, image_Width_And_Height);
    Trv = Array(Float64, image_Width_And_Height);
    Tiv = Array(Float64, image_Width_And_Height);
    Tiv = Array(Float64, image_Width_And_Height);
    is_still_bounded = Array(UInt8, image_Width_And_Height);
    Tmp = Array(Float64, image_Width_And_Height);
    

    for jj in 1: image_Width_And_Height

      fill!(Zrv, 0.0);
      fill!(Ziv, 0.0);
      fill!(Trv, 0.0);
      fill!(Tiv, 0.0);

      for iter = 1 : 50
        broadcast!(*,Ziv,Zrv,Ziv );
	broadcast!(*,Ziv,Ziv,2);
        broadcast!(+,Ziv,Ziv, initial_i);

	broadcast!(-,Tmp,Trv,Tiv );
        broadcast!(+,Zrv,Tmp,initial_r[jj] );

	broadcast!(*,Trv,Zrv,Zrv );
        broadcast!(*,Tiv,Ziv,Ziv );

	broadcast!(+,Tmp,Trv,Tiv );
      end

      broadcast!(<,is_still_bounded,Tmp,4.0 )
      broadcast!(<<,bit_mask, is_still_bounded,shift[jj] )
      posn = Int64((jj -1)  >> 3);
      bitmap[posn + 1,:] =  bitmap[posn + 1,:] | bit_mask';
    end    
#println(bitmap);

    fid = open("vec.pbm","w");
    print(fid, "P4\n", N, " " ,N, "\n");
    write(fid, bitmap);
    close(fid);
 end

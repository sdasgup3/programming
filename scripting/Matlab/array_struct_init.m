aclear;
N = 10000;

%1) GROWING A STRUCT
        tic;
        for ii=1:N
            a(ii).x(1)=1;    
        end
        noPreAll = toc;        

    %2)PREALLOCATING A STRUCT
        tic;
        b = repmat( struct( 'x', 1 ), N, 1 );
        for ii=1:N
            b(ii).x(1)=1;    
        end;  
        repmatBased=toc;        

    %3)Index to preallocate
        c(N).x = 1;
        for ii=1:N
            c(ii).x(1)=1;    
        end;  
        preIndex=toc;

        disp(['No preallocation:        ' num2str(noPreAll)])            
        disp(['Preallocate Indexing:    ' num2str(preIndex)])
        disp(['Preallocate with repmat: ' num2str(repmatBased)])
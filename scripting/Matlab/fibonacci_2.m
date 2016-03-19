
function f = fibonacci_2(n)
    % No Help
    
    f  = zeros(1,n);
    
    f(1) = 1;
    f(2) =   2;
    for ii = 3:n
        f(ii) = f(ii-1) + f(ii-2);
    end
end
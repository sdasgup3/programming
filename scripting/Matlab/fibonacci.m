function [ ret ] = fibonacci(n)
%UNTITLED2 Summary of this function goes here
%   Sandeep
    if (n == 1) || (n == 2)
        ret = 1;
    else
        ret  = fibonacci(n-1) + fibonacci(n-2 );
    end
end


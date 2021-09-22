function f = polyEval(x, a)
    a = a(:);
    x = x(:);
    [M, ~] = size(a);
    [N, ~] = size(x);
    
    f = zeros(N,1);
    for i = 1 : N
        x_i = x(i);
        for j = 1 : M
            term = a(j) * (x_i ^ (j-1));
            f(i) = f(i) + term;
        end
    end
end
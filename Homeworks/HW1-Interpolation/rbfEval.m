function f = rbfEval(x, p, b, d, b0)
    b = b(:);
    x = x(:);
    p = p(:);
    [M, ~] = size(b);  % b0 + b_1 + ... + b_M
    [N, ~] = size(x);  % sample points
    
    f = zeros(N, 1);
    for i = 1: N
        f(i) = b0;
        for j = 1: M
            term = b(j) * (1.0 / ((x(i) - p(j))^2 + d));
            f(i) = f(i) + term;
        end
    end
    
end
function b = rbfInterpolate(p, q, d, b0)
    % ensures column vector for input
    p = p(:);
    q = q(:);
    [N, ~] = size(p);
    K = zeros(N, N);
    rhs = zeros(N, 1);
    
    for i = 1: N
        rhs(i) = q(i) - b0;
        for j = 1: N
            % ith equation, jth coefficient
            K(i, j) = 1.0 / ((p(i) - p(j)) ^ 2 + d);
        end
    end
    
    b = linsolve(K, rhs);
end

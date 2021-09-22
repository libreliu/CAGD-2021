function a = polyInterpolate(p, q)
    % ensures column vector for input
    p = p(:);
    q = q(:);
    [N, ~] = size(p);
    K = zeros(N, N);
    for i = 1: N
        for j = 1: N
            K(i, j) = p(i) ^ (j - 1);
        end
    end
    
    [a, ~] = linsolve(K, q);
end

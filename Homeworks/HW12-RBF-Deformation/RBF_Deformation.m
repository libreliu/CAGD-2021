% run the following command to enable the current Matlab session as a server
% enableservice('AutomationServer', true)

% to start the c++/OpenGL application, run the next line
% !start glvu.exe

nv = size(x, 1);

% == m in the paper
n = numel(P2PVtxIds);

d = 1000;
p = 1;

%% construct matrix for linear system
P2PVtxIds
p2pDsts
size(x)

% psrc: source of the vertices to be moved
psrc = x(P2PVtxIds,:);

Phi = zeros(n, n);
% P = zeros(n, 10);
P = zeros(n, 4);

for i = 1:n
   for j = 1:n
        Phi(i, j) = norm(psrc(i,:)-psrc(j,:))^3;
   end
end

for i = 1:n
    P(i,1) = 1;
    P(i,2) = psrc(i,1);
    P(i,3) = psrc(i,2);
    P(i,4) = psrc(i,3);
%     P(i,5) = psrc(i,1) * psrc(i,1);
%     P(i,6) = psrc(i,2) * psrc(i,2);
%     P(i,7) = psrc(i,3) * psrc(i,3);
%     P(i,8) = psrc(i,1) * psrc(i,2);
%     P(i,9) = psrc(i,2) * psrc(i,3);
%     P(i,10) = psrc(i,3) * psrc(i,1);
end

% A = [Phi P; P' zeros(10)];
% B = [p2pDsts; zeros(10, 3)];
A = [Phi P; P' zeros(4)];
B = [p2pDsts; zeros(4, 3)];

%% solve for warp coefficients
coeff = linsolve(A'*A, A'*B);
 %coeff = linsolve(A, B);
result = zeros(size(x));

if n <= 3
    disp("Use original one");
    y = x;
else

    for i = 1:size(x, 1)
        for j = 1:n
            result(i, :) = result(i, :) + coeff(j) .* (norm(x(i,:) - psrc(j,:))^3);
        end
            result(i, :) = result(i, :) + coeff(n+1, :) * 1;
            result(i, :) = result(i, :) + coeff(n+2, :) .* x(i,1);
            result(i, :) = result(i, :) + coeff(n+3, :) .* x(i,2);
            result(i, :) = result(i, :) + coeff(n+4, :) .* x(i,3);
%             result(i, :) = result(i, :) + coeff(n+5, :) .* x(i,1) .* x(i,1);
%             result(i, :) = result(i, :) + coeff(n+6, :) .* x(i,2) .* x(i,2);
%             result(i, :) = result(i, :) + coeff(n+7, :) .* x(i,3) .* x(i,3);
%             result(i, :) = result(i, :) + coeff(n+8, :) .* x(i,1) .* x(i,2);
%             result(i, :) = result(i, :) + coeff(n+9, :) .* x(i,2) .* x(i,3);
%             result(i, :) = result(i, :) + coeff(n+10, :) .* x(i,3) .* x(i,1);

    end
    
    y = result;
end

%% apply the map to x, output to var y, which will be read by the c++ application
% y = x;
% y(P2PVtxIds,:) = p2pDsts;  % the p2p constraints
%y = x;





% This has been tested working with MATLAB R2021a
% Meaning on @: https://ww2.mathworks.cn/help/matlab/math/parameterizing-functions.html
x = @(u,v) (1-u.^2).*(1-v.^2) ./ ((1+u.^2).*(1+v.^2));
y = @(u,v) (1-u.^2).*(v * 2) ./ ((1+u.^2).*(1+v.^2));
z = @(u,v) (1+v.^2).*(u * 2) ./ ((1+u.^2).*(1+v.^2));

x = @(u,v) (1-u.^2).*(1-v.^2) ./ ((1+u.^2).*(1+v.^2));
y = @(u,v) (1-u.^2).*(v * 2) ./ ((1+u.^2).*(1+v.^2));
z = @(u,v) (1+v.^2).*(u * 2) ./ ((1+u.^2).*(1+v.^2));


fmesh(x,y,z,[0 1 0 1])

%x = @(u,v) (1-u.^2).*(1-v.^2) ./ ((1+u.^2).*(1+v.^2));
%y = @(u,v) (1-u.^2).*(v * 2) ./ ((1+u.^2).*(1+v.^2));
%z = @(u,v) (1+v.^2).*(u * 2) ./ ((1+u.^2).*(1+v.^2));
%fmesh(x,y,z,[0 1 0 1])
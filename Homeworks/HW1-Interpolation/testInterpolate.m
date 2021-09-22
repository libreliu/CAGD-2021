p = [1,2,3,5,8,10];
q = [1,5,10,20,5,3];

%figure;
%h = impoly('closed', false);
%pq = h.getPosition;
%p = pq(:, 1);
%q = pq(:, 2);

scatter(p, q)

hold on
a = polyInterpolate(p, q);
d = 5;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_poly = polyEval(x, a);
f_rbf = rbfEval(x, p, b, d, b0);
plot(x, f_poly, 'r')
plot(x, f_rbf, 'g')
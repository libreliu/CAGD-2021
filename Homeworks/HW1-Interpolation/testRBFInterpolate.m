p = [1,2,3,5,8,10];
q = [1,5,10,20,5,3];

scatter(p, q)



subplot(1,5,1);
d = 5;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_rbf = rbfEval(x, p, b, d, b0);
plot(x, f_rbf, 'r')
scatter(p, q)
title('d=5')

subplot(1,4,1);
hold on
d = 3;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_rbf = rbfEval(x, p, b, d, b0);
scatter(p, q)
plot(x, f_rbf, 'r')
title('d=3')

subplot(1,4,2);
hold on
d = 1;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_rbf = rbfEval(x, p, b, d, b0);
scatter(p, q)
plot(x, f_rbf, 'r')
title('d=1')


subplot(1,4,3);
hold on
d = 0.5;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_rbf = rbfEval(x, p, b, d, b0);
scatter(p, q)
plot(x, f_rbf, 'r')
title('d=0.5')

subplot(1,4,4);
hold on
d = 0.1;
b0 = 10;
b = rbfInterpolate(p, q, d, b0);

x = linspace(min(p), max(p));
f_rbf = rbfEval(x, p, b, d, b0);
scatter(p, q)
plot(x, f_rbf, 'r')
title('d=0.1')
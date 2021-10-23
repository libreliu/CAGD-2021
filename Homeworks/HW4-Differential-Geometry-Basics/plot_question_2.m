clear;
R = 3;
r = 1;

theta = linspace(0, 2*pi);

inner_x = R * cos(theta);
inner_y = R * sin(theta);

outer_x = r * cos(theta) + (R + r);
outer_y = r * sin(theta);

x = (R + r) * cos(theta) + r * cos(R * theta / r + theta);
y = (R + r) * sin(theta) + r * sin(R * theta / r + theta);

figure;
hold on;
axis([-5 5 -5 5])
plot(x,y);
plot(inner_x, inner_y);
plot(outer_x, outer_y);
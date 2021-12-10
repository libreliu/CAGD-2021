[x, t] = readObj('cathead.obj');

figure; subplot(121); trimesh(t, x(:,1), x(:,2), x(:,3), 'edgecolor', 'k'); axis equal; axis off; title('input mesh');

y = tutte_embed(x, t);

subplot(122); trimesh(t, y(:,1), y(:,2), y(:,1)*0, 'edgecolor', 'k'); axis equal; axis off; title('Tutte embedding'); view(2);


%% TODO: finish the Tutte embedding algorithm below 
function y = tutte_embed(x, t)
    B = findBoundary(x, t); % find the exterior boundary of the mesh
    y = x(:, 1:2);
end
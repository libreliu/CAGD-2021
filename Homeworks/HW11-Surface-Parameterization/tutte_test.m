[x, t] = readObj('cathead.obj');

figure; subplot(121); trimesh(t, x(:,1), x(:,2), x(:,3), 'edgecolor', 'k'); axis equal; axis off; title('input mesh');

y = tutte_embed(x, t);

subplot(122); trimesh(t, y(:,1), y(:,2), y(:,1)*0, 'edgecolor', 'k'); axis equal; axis off; title('Tutte embedding'); view(2);


%% TODO: finish the Tutte embedding algorithm below 
function y = tutte_embed(x, t)
    B = findBoundary(x, t); % find the exterior boundary of the mesh
    numBoundaryVertices = size(B, 2);
    numVertices = size(x, 1);
    
    matAdj = zeros(numVertices, numVertices);
    for faceIdx = 1: size(t, 1)
        faceVert = t(faceIdx, :);
        updateList = [1,2; 2,3; 3,1];
        for i = 1 : 3
            matAdj(faceVert(updateList(i, 1)),faceVert(updateList(i, 2))) = 1;
            matAdj(faceVert(updateList(i, 2)),faceVert(updateList(i, 1))) = 1;
        end
    end
    
    A = zeros(numVertices, numVertices);
    b = zeros(numVertices, 2);
    
    for i = 1: numBoundaryVertices
        A(i, B(i)) = 1;
        b(i, 1) = cos(2 * pi * i / numBoundaryVertices);
        b(i, 2) = sin(2 * pi * i / numBoundaryVertices);
    end
    
    nextIdx = numBoundaryVertices + 1;
    for i = 1: numVertices
        isBoundaryPoint = 0;
        for j = 1: numBoundaryVertices
            if i == B(j)
                isBoundaryPoint = 1;
                break
            end
        end
        
        if isBoundaryPoint == 0
            A(nextIdx, i) = 1;
            % find neighbour
            numNeighbours = 0;
            for j = 1: numVertices
                if matAdj(i, j) == 1
                    numNeighbours = numNeighbours + 1;
                end
            end
            for j = 1: numVertices
                if matAdj(i, j) == 1
                    A(nextIdx, j) = -1 / numNeighbours;
                end
            end
            nextIdx = nextIdx + 1;
        end
    end
    
    x = linsolve(A, b);
    
    
    y = x(:, 1:2);
end
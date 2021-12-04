clear;

[x, t] = readObj('ball.obj');

figure; subplot(121); trimesh(t, x(:,1), x(:,2), x(:,3), 'edgecolor', 'k'); axis equal; axis off; title('coarse mesh');

[x2, t2] = loop(x, t, 1);

subplot(122); trimesh(t2, x2(:,1), x2(:,2), x2(:,3), 'edgecolor', 'k'); axis equal; axis off; title('subdivision mesh');


%% TODO: finish the loop subdivision algorithm below 
% x: numVec * 3
% t: numFace * 3, index starts from 1
function [x2, t2] = loop(x, t, n)
    numVertices = size(x, 1);
    numFaces = size(t, 1);
    
    % old adjacency matrix
    oldAdj = zeros(numVertices, numVertices);
    % old (vs, vd) => [num of trigs]
    
    
    for faceIdx = 1: numFaces
        faceVert = t(faceIdx, :);
        updateList = [1,2; 2,3; 3,1];
        for i = 1 : 3
            oldAdj(faceVert(updateList(i, 1)),faceVert(updateList(i, 2))) = 1;
            oldAdj(faceVert(updateList(i, 2)),faceVert(updateList(i, 1))) = 1;
        end
    end
    
    x2 = x;
    t2 = zeros(0, 3);
    % (vs, vd) => new_vs index, 0 for not found
    subdivided = zeros(numVertices, numVertices, 1);
    for i = 1: numVertices
        for j = 1: numVertices
            if oldAdj(i, j) == 1 && subdivided(i, j) == 0
                % had not been subdivided
                new_vert = (x(i, :) + x(j, :)) / 2;
                x2 = [x2; new_vert];
                subdivided(i, j) = size(x2, 1);
                subdivided(j, i) = size(x2, 1);
            end
        end
    end
    
    % form new t2
    for faceIdx = 1: numFaces
        v = t(faceIdx, :);
        v12 = subdivided(v(1), v(2));
        v13 = subdivided(v(1), v(3));
        v23 = subdivided(v(2), v(3));
        assert(v12 ~= 0 && v13 ~= 0 && v23 ~= 0);
        
        % notice winding order - useful for manifold based mesh processing
        newFaces = [
            v(1), v12, v13;
            v(2), v23, v12;
            v(3), v13, v23;
            v12 , v23, v13;
        ];
    
        t2 = [t2; newFaces];
    end
    
    newAdj = zeros(size(x2, 1), size(x2, 1));
    for faceIdx = 1: size(t2, 1)
        faceVert = t2(faceIdx, :);
        updateList = [1,2; 2,3; 3,1];
        for i = 1 : 3
            newAdj(faceVert(updateList(i, 1)),faceVert(updateList(i, 2))) = 1;
            newAdj(faceVert(updateList(i, 2)),faceVert(updateList(i, 1))) = 1;
        end
    end
    
    % compute fixup for new vertices
    for aIdx = 1 : numVertices
        for bIdx = aIdx + 1: numVertices
            % check nearby vertices, and the triangle associated with this edge
            vIdx = subdivided(aIdx, bIdx);
            if vIdx == 0
                continue
            end
            % find c and d
            
            adjFaces = zeros(0, 3);
            myVerts = sort([aIdx, bIdx]);
            for faceIdx = 1: numFaces
                faceVerts = t(faceIdx, :);
                faceVerts = sort(faceVerts);
                if isequal(faceVerts(1:2), myVerts) || isequal(faceVerts(2:3), myVerts) || isequal([faceVerts(1), faceVerts(3)], myVerts)
                    adjFaces = [adjFaces; faceVerts];
                end
            end
            
            % no boundary cases is considered
            assert(size(adjFaces, 1) == 2);
            
            oppoVerts = zeros(2);
            for i = 1: 2
                if isequal(adjFaces(i, 1:2), myVerts)
                    oppoVerts(i) = adjFaces(i, 3);
                elseif isequal(adjFaces(i, 2:3), myVerts)
                    oppoVerts(i) = adjFaces(i, 1);
                elseif isequal([adjFaces(i, 1), adjFaces(i, 3)], myVerts)
                    oppoVerts(i) = adjFaces(i, 2);
                else
                    assert(0 == 1)
                end
            end
            
            % finally collected all vertices!
            x2(vIdx, :) = (3.0/8.0) * (x(aIdx, :) + x(bIdx, :)) + (1.0/8.0) * (x(oppoVerts(1), :) + x(oppoVerts(2), :));
        end
    end
    
    % compute fixup for old vertices
    for vIdx = 1: numVertices
        % get neighbour associated
        numNeighbour = 0;
        neighSum = zeros(1, 3);
        for neighIdx = 1: numVertices
            if oldAdj(vIdx, neighIdx) > 0
                numNeighbour = numNeighbour + 1;
                neighSum = neighSum + x(neighIdx, :);
            end
        end
        u = 3 / (8 * numNeighbour);
        if numNeighbour == 3
            u = 3 / 16;
        end
        
        x2(vIdx, :) = (1 - numNeighbour * u) * x(vIdx, :) + u * neighSum;
    end
end
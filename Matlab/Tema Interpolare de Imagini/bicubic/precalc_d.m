function [Ix, Iy, Ixy] = precalc_d(I)
    % =========================================================================
    % Prealculeaza matricile Ix, Iy si Ixy ce contin derivatele dx, dy, dxy ale 
    % imaginii I pentru fiecare pixel al acesteia
    % =========================================================================
    
    % obtine dimensiunea imaginii
    [m n nr_colors] = size(I);
    
    % TODO: fa cast matricii I la double
    I = double(I);
    Ix = zeros(m,n);
    Iy = zeros(m,n);
    Ixy = zeros(m,n);

    
for i = 1 : m
  for j = 1 : n
    % TODO: calculeaza matricea cu derivate fata de x Ix
    Ix(i, j) = fx(I, j, i);
    % TODO: calculeaza matricea cu derivate fata de y Iy
    Iy(i, j) = fy(I, j, i);
    % TODO: calculeaza matricea cu derivate fata de xy Ixy
    Ixy(i, j) = fxy(I, j, i);
  endfor
endfor
endfunction

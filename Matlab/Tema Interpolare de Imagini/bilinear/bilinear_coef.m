function a = bilinear_coef(f, x1, y1, x2, y2)
    % =========================================================================
    % Calculeaza coeficientii a pentru interpolarea biliniara intre punctele
    % (x1, y1), (x1, y2), (x2, y1) si (x2, y2)
    % =========================================================================
    
    % TODO: calculeaza matricea A
    %A = [1 x1 y1 x1*y1; 1 x1 y2 x1*y2; 1 x2 y1 x2*y1; 1 x2 y2 x2*y2];
    A = [1 y1 x1 x1*y1; 1 y2 x1 x1*y2; 1 y1 x2 x2*y1; 1 y2 x2 x2*y2];
    

    % TODO: calculeaza vectorul b
    b = [f(y1, x1); f(y2, x1); f(y1, x2); f(y2, x2)];
    

    % TODO: calculeaza coeficientii
    %a = inv(A) * b;
    a = A\b;
    %A*a == b
    b = a;
    %b
    b(2) = a(3);
    b(3) = a(2);
    a = round(b);
endfunction

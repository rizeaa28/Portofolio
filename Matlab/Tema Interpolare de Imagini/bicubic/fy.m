function r = fy(f, x, y)
    % =========================================================================
    % Aproximeaza derivata fata de y a lui f in punctul (x, y).
    % =========================================================================
    
    % TODO: calculeaza derivata
    
    [m n] = size(f);
    
    if(y == 1)
      f2 = 0;
    else
      f2 = f(y-1, x);
    endif
    
    if(y == m)
      f1 = 0;
    else
      f1 = f(y+1, x);
    endif
    
    if(y == 1 || y == m)
      r = 0;
    else
      r = (f1 - f2)/2;
    endif
    
endfunction
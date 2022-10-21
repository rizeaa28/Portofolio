function r = fxy(f, x, y)
    % =========================================================================
    % Aproximeaza derivata fata de x si y a lui f in punctul (x, y).
    % =========================================================================
    
    % TODO: calculeaza derivata
    [m n] = size(f);
    
    if(x == 1 || y == 1)
      f1 = 0;
    else
      f1 = f(y-1, x-1);
    endif
      
    if(x == 1 || y == m)
      f4 = 0;
    else
      f4 = f(y+1, x-1);
    endif
      
    if(y == 1 || x == n)
      f3 = 0;
    else
      f3 = f(y-1, x+1);
    endif
    
    if(y == m || x == n)
      f2 = 0;
    else
      f2 = f(y+1, x+1);
    endif
    
    if(x == 1 || y == 1 || x == n || y == m)
      r = 0;
    else
      r = (f1 + f2 - f3 - f4)/4;
    endif

endfunction
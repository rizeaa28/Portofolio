function r = fx(f, x, y)
    % =========================================================================
    % Aproximeaza derivata fata de x a lui f in punctul (x, y).
    % =========================================================================

    % TODO: calculeaza derivata
    [m n] = size(f);
    
    if(x == n)
      f1 = 0;
    else
      f1 = f(y, x+1);
    endif
      
    if(x == 1)
        f2 = 0;
    else
      f2 = f(y, x-1);
    endif
    
    if(x == 1 || x == n)
      r = 0;
    else  
      r = (f1 - f2)/2;
    endif

endfunction
function out = bilinear_rotate_RGB(img, rotation_angle)
    % =========================================================================
    % Aplica interpolare bilineara pentru a roti o imagine RGB cu un unghi dat.
    % =========================================================================
    
    % TODO: extrage canalul rosu al imaginii
    rosu = img(:,:,1);
    
    % TODO: extrage canalul verde al 
    verde = img(:,:,2);

    % TODO: extrace canalul albastru al imaginii
    albastru = img(:,:,3);

    % TODO: aplică rotația pe fiecare canal al imaginii
    out_rosu = bilinear_rotate(rosu, rotation_angle);
    out_verde = bilinear_rotate(verde, rotation_angle);
    out_albastru = bilinear_rotate(albastru, rotation_angle);

    % TODO: reconstruiește imaginea RGB finala (hint: cat)
    out = cat(3, out_rosu, out_verde, out_albastru);
    
endfunction
function out = nn_2x2_RGB(img, STEP = 0.1)
    % =========================================================================
    % Aplica interpolare nearest neighbour pe imaginea 2x2 img cu puncte
    % intermediare echidistante.
    % img este o imagine colorata RGB.
    % =========================================================================

    % TODO: extrage canalul rosu al imaginii
    rosu = img(:,:,1);

    % TODO: extrage canalul verde al imaginii
    verde = img(:,:,2);
    
    % TODO: extrace canalul albastru al imaginii
    albastru = img(:,:,3);

    % TODO: aplica functia nn pe cele 3 canale ale imaginii
    out_rosu = nn_2x2(rosu, STEP);
    out_verde = nn_2x2(verde, STEP);
    out_albastru = nn_2x2(albastru, STEP);

    % TODO: formeaza imaginea finala cu cele 3 canale de culori
    % Hint: functia cat
    out = cat(3, out_rosu, out_verde, out_albastru);
endfunction

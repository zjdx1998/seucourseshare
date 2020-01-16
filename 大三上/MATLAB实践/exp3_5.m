function y = exp3_5(t)
    [~, N] = size(t);
    y = window(@bartlett, N);
return
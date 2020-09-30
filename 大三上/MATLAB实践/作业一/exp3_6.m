function y = exp3_6(t)
    [~, N] = size(t);
    y = zeros(1, N);
    for i = 1:N
        y(i) = 2 * sin(4 * pi * t(i)) + 5 * cos(6 * pi * t(i));
    end
return
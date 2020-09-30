function y = exp3_3_3(t)
    [~, N] = size(t);
    y = zeros(1, N);
    for i = 1:N
        y(i) = heaviside(t(i) + 1) - 2 * heaviside(t(i)) + heaviside(t(i) - 1);
    end
return
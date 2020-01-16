function y = exp3_3_1(t)
    [~, N] = size(t);
    y = zeros(1, N);
    for i = 1:N
        if t(i) > - pi && t(i) < pi
            y(i) = -abs(t(i)) + pi;
        else
            y(i) = 0;
        end
    end
return
% for i = 1:1:pi * 1000 - 1
%     f(i) = i/1000 + 1000 * pi;
%     w(i) = (i + 1) * 0.2 * pi;
% end

% for i = int32(pi * 1000) + 1:1:int32(2 * 1000 * pi)
%     f(i) = - i/1000 + 1000 * pi;
%     w(i) = (i - 1) * 0.2 * pi;
% end

% for i = int32(2 * 1000 * pi):1:10000
%     f(i) = 0;
%     w(i) = (i - 1) * 0.2 * pi;
% end

% F = fft(f)/1000;
% plot(w, F)
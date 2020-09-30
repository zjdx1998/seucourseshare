function y = exp3_3_2(t)
    [~, N] = size(t);
    y = zeros(1, N);
    for i = 1:N
        y(i) = cos(pi / 2 * t(i)) .* (heaviside(t(i) + 1) - heaviside(t(i) - 1));
    end
return

% for i = 1:10000
%     %f(i) = stepfun(i / 1000 + 1, 0) - 2 * stepfun(i / 1000, 0) + stepfun(i / 1000 - 1, 0);
%     f(i) = cos(pi / 2000 * i) .* (stepfun(i / 1000 + 1, 0) - stepfun(i / 1000 - 1, 0));
%     w(i) = (i - 1) * 0.2 * pi;
% end

% F = fft(f) / 1000;
% subplot(1,2,1);
% plot(w(1:200), abs(f(1:200)));    title('幅度');
% subplot(1,2,2);
% plot(w(1:200), angle(f(1:200)));
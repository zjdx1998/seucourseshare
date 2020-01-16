function [an, ab] = DFT(func ,N, fs)
%func 为字符串
%N = 10000; % 总采样次数
%fs = 2000; % 采样率
    test = str2func(func);
    ws = 2 * pi * fs;
    deltat = 1 / fs;
    deltaw = ws / N;
    deltaf = deltaw / 2 / pi;

    t = zeros(1, N);
    w = zeros(1, N);
    for i = 1 : N
        t(i) = deltat * (i - 1);
        w(i) = deltaf * (i - 1);
    end
    g = test(t);
    %plot(t,g)
    m = fft(g) / fs;
    an = angle(m);
    ab = abs(m);
    if N > 32
        max = 32;
    else
        max = N;
    end
    subplot(1,2,1);
    plot(w(1:max), ab(1:max));
    xlabel('w');ylabel('G');title('Amplitude');
    subplot(1,2,2);
    plot(w(1:max),an(1:max));
    xlabel('w');ylabel('f');title('Angle');

% function y = test(t, j)
%     [~, N] = size(t);
%     y = zeros(1, N);
%     if j == 1
%         for i = 1:N
%             y(i) = 2 * sin(4 * pi * t(i)) + 5 * cos(6 * pi * t(i));
%         end
%     end

%     if j == 2
%         for i = 1:N
%             if t(i) > - pi && t(i) < pi
%                 y(i) = -abs(t(i)) + pi;
%             else
%                 y(i) = 0;
%             end
%         end
%     end

%     if j == 3
%         for i = 1:N
%         y(i) = heaviside(t(i) + 1) - 2 * heaviside(t(i)) + heaviside(t(i) - 1);
%         end
%     end
% end

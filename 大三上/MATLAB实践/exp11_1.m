clear;
% k = 1; % 取样系数的倍数
% Fn = 1; % 奈奎斯特频率
% Fs = Fn * k; % 实际取样频率
% T = 1 / Fs; % 取样间隔
T = 0.01;
% 在-100~100间取样
Nc = 100 / T;
for n = -Nc:1:Nc
    nT(n + Nc +1) = T * n;
    f(n + Nc + 1) = (sin(10 * pi * nT(n + Nc + 1)) + eps) / (pi * nT(n + Nc + 1) + eps);
end
figure;
[yfreq, freqrang] = positiveFFT(f, 1);
plot(freqrang,abs(yfreq));
xlabel('frequncy(Hz)')
ylabel('Amplitude')
title('Frequency Domain')
grid

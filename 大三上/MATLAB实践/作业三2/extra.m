clear;
Fs = 10000; % 实际取样频率
T = 1 / Fs; % 取样间隔
% 在0~2间取样
Nc = 2 / T;
for n = 0:1:Nc
    nT(n + Nc +1) = T * n;
    f(n + Nc + 1) = cos(2 * pi * 2000 * nT(n + Nc +1) + sin(2 * pi * 10 * nT(n + Nc +1)));
end
figure;
[yfreq, freqrang] = positiveFFT(f, 1);
plot(freqrang,abs(yfreq));
xlabel('frequncy(Hz)')
ylabel('Amplitude')
title('Frequency Domain')
grid
axis([0.195, 0.205, 0, 0.2])

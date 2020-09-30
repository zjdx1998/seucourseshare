t = 1:200;
y = cos(10 * t) .* cos(600 * t);
figure;
subplot(2,1,1);
plot(t, y);
xlabel('time(s)')
ylabel('Amplitude')
title('Time Domain')
grid
[yfreq, freqrang] = positiveFFT(y, 10000);
subplot(2,1,2);
plot(freqrang,abs(yfreq));
xlabel('frequncy(Hz)')
ylabel('Amplitude')
title('Frequency Domain')
grid
% 解调
y0 = y .* cos(600 * t);
figure;
subplot(2,1,1);
plot(t, y0);
xlabel('time(s)')
ylabel('Amplitude')
title('Time Domain')
grid
[yfreq, freqrang] = positiveFFT(y0, 50000);
subplot(2,1,2);
plot(freqrang,abs(yfreq));
xlabel('frequncy(Hz)')
ylabel('Amplitude')
title('Frequency Domain')
grid
clear;
p = 8;
q = 2;
n = 1: 50;
for i = n
    if i <= 15
        xa(i) = exp(- (i - p) ^ 2 / q);
    else
        xa(i) = 0;
    end
end
subplot(1,2,1);
stem(n, xa);
xlabel('time(s)')
ylabel('Amplitude')
title('Time Domain')
grid
[yfreq, freqrang] = centeredFFT(xa, 1);
subplot(1,2,2);
stem(freqrang,abs(yfreq));
xlabel('frequncy(Hz)')
ylabel('Amplitude')
title('Frequency Domain')
grid
axis([-0.5,0.5,0,0.5])
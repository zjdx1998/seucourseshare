clear;
clc;
Fs=10000;
t=0:1/Fs:2;
L=length(t);
F0=linspace(-Fs/2,Fs/2,L);

f=cos(2*pi*2000*t+sin(2*pi*10*t));
F=fftshift(fft(f));

subplot(2,1,1);plot(F0,abs(F)*2/L);
subplot(2,1,2);plot(F0,angle(F));

% function y = extra(t)
%     [~, N] = size(t);
%     y = zeros(1, N);
%     for i = 1:N
%         y(i) = cos(2 * pi * 2000 * t(i) + sin(2 * pi * 10 * t(i)));
%     end
% return

% clear;
% for i = 1:20000
%     t(i) = (i - 1) * 0.0001;
%     w(i) = (i - 1) * pi;
%     f(i) = cos(2 * pi * 2000 * t(i) + sin(2 * pi * 10 * t(i)));
% end
% F = fft(f) / 10000;
% subplot(2, 1, 1);
% plot(w, abs(F));
% title('Amplitude');
% subplot(2, 1, 2);
% title('angle');
% plot(w, angle(F));
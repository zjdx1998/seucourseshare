clear
%1
% z = [1, 2]';
% p = [-1, -2]';
% k = 3;
%2
% z = [];
% p = [0];
% k = 1;
%3
% Ns = [1, 0, 1];
% Ds = [1, 2, 5];
% [z, p] = tf2zp(Ns, Ds);
%4
% Ns = 0.6 * [3, 2, 2, 5];
% Ds = [1, 3, 2, 1];
% [z, p] = tf2zp(Ns, Ds);
%5
A = [0, 1, 0; 0, 0, 1; -6, -11, -6];
B = [0; 0; 1];
C = [4, 5, 1];
D = [0];
[z, p] = ss2zp(A, B, C, D);
zplane(z, p);
disp('zeros:');disp(z);
disp('poles:');disp(p);

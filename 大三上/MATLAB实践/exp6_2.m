%1
% z = [0];
% p = [-1, -4]';
% k = 1;
% sys1 = zpk(z, p, k)
%2
% Ns = [1, 1];
% Ds = [1, 0, -1];
% sys2 = tf(Ns, Ds)
%3
% Ns = [1, 6, 6, 0];
% Ds = [1, 6, 8];
% sys2 = tf(Ns, Ds)
%5
z = [];
p = [0, 1, 1]';
k = 1;
sys1 = zpk(z, p, k)
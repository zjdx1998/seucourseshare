%2
% A = [2, 2, 1; 1, 3, 1; 1, 2, 2];
% B = [3; 3; 4];
% C = [1, 1, 1];
% D = [0];
% [z, p] = ss2zp(A, B, C, D);
%4
% z = [-3];
% p = [-1, -5, -15];
% k = 15;
%6
Ns = [1, -1.414, 1];
Ds = [1, 0.9, 0.81];
sys1 = tf(Ns, Ds);
z = -1;
p = 0.3;
k = 1;
sys2 = zpk(z, p, k);
if sum(p < 0) == length(p)
    disp('unstable');
else
    if sum(p == 0) == 1 && sum(abs(p) == 1) == 1
        disp('marginally stable');
    else
        disp('stable');
    end
end
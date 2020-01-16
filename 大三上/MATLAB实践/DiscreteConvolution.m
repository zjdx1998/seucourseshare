function ret = DiscreteConvolution(f1, f2)
% 离散卷积
for k = 1:length(f1) + length(f2) - 1
    ret(k) = 0;
    for i = 1:length(f1)
        if k - i >= 0 && k - i < length(f2)
            ret(k) = ret(k) + f1(i) * f2(k - i + 1);
        end
    end
end
return
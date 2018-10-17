format short;
A=    [
 1,1,0,0,0,0,0,0;
 1,2,1,0,0,0,0,0;
 0,1,3,1,0,0,0,0;
 0,0,1,4,1,0,0,0;
 0,0,0,1,5,1,0,0;
 0,0,0,0,1,6,1,0;
 0,0,0,0,0,1,7,1;
 0,0,0,0,0,0,1,8
 ];
b =[7;1;1;1;5;1;1;2];
X=    inv(A)*b

function smile_04 
clear all, clf, hold off 
dt = pi/20;  t=0:dt:2*pi;
xf0=cos(t); yf0=sin(t);    % face 
xe0(1,:)=0.08*xf0-0.3; ye0(1,:)=0.12*yf0+0.2;    % left eye 
xe0(2,:)=0.08*xf0+0.3; ye0(2,:)=ye0(1,:);           % right eye 
s1 = 3*pi/2-1.1;  s2 = 3*pi/2+1.1;  s = s1:dt:s2;
xm0 = 0.5*cos(s); ym0 = 0.5*sin(s);   % mouth 
%figure(1)    %ªÊ÷∆≥ı º–¶¡≥Õº
subplot(2,2,1)
T0=eye(3);
[xf1,yf1,xe1,ye1,xm1,ym1]=DrawSmileFace(xf0,yf0,xe0,ye0,xm0,ym0,T0);
title('Õº1£∫–¶¡≥≥ı ºÕº'); 
%figure(2)    %ªÊ÷∆–¶¡≥Õº
subplot(2,2,2) 
T1=[2.5 0 0;0 1 0;0 0 1];%∫·÷·2.5±∂,◊›÷·1±∂
%T1=[1 0 0;0 2.5 0;0 0 1];          %∫·÷·1±∂,◊›÷·2.5±∂ 
[xf2,yf2,xe2,ye2,xm2,ym2]=DrawSmileFace(xf0,yf0,xe0,ye0,xm0,ym0,T1); 
title('Õº2£∫–¶¡≥∫·÷·Àı∑≈2.5±∂,◊›÷·≤ª±‰'); 
%figure(3)    %ªÊ÷∆–¶¡≥Õº 
subplot(2,2,3)
T2=[1 0 1;0 1 1.5;0 0 1];%∫·÷·∆Ω“∆1,◊›÷·∆Ω“∆1.5
[xf3,yf3,xe3,ye3,xm3,ym3]=DrawSmileFace(xf2,yf2,xe2,ye2,xm2,ym2,T2);
title('Õº3£∫–¶¡≥∫·÷·∆Ω“∆1,◊›÷·∆Ω“∆1.5'); 
%figure(4)    %ªÊ÷∆–¶¡≥Õº 
subplot(2,2,4)
theta=pi/4;   %ƒÊ ±’Î–˝◊™45∂» 
x0=sum(xf3)/length(xf3); 
y0=sum(yf3)/length(yf3); 
xy0=[x0;y0]; T3_1=eye(3);
T3_1(1:2,3)=-xy0; 
T3_2=[cos(theta),-sin(theta),0;sin(theta),cos(theta),0;0 0 1];
T3_3=2*eye(3)-T3_1;
T3=T3_3*T3_2*T3_1; 
%T3=[cos(theta),-sin(theta),0;sin(theta),cos(theta),0;0 0 1];
[xf4,yf4,xe4,ye4,xm4,ym4]=DrawSmileFace(xf3,yf3,xe3,ye3,xm3,ym3,T3);
title('Õº4£∫»∆–¶¡≥÷––ƒƒÊ ±’Î–˝◊™45∂»'); 
%ªÊ÷∆±‰ªª–¶¡≥Õº◊”≥Ã–Ú 
function [xf1,yf1,xe1,ye1,xm1,ym1]=DrawSmileFace(xf,yf,xe,ye,xm,ym,T) 
n1=length(xf); n2=length(xe); n3=length(xm); 
Pf0=[xf;yf;ones(1,n1)]; Pf1=T*Pf0; xf1=Pf1(1,:); yf1=Pf1(2,:);
fill(xf1,yf1,'r');        % face 
hold on
Pe01=[xe(1,:);ye(1,:);ones(1,n2)];
Pe02=[xe(2,:);ye(2,:);ones(1,n2)]; 
Pe1=T*Pe01;
Pe2=T*Pe02; 
xe1(1,:)=Pe1(1,:); ye1(1,:)=Pe1(2,:); 
xe1(2,:)=Pe2(1,:); ye1(2,:)=Pe2(2,:);
plot(xe1(1,:),ye1(1,:),'k','linewidth',5)   % left eye 
plot(xe1(2,:),ye1(2,:),'k','linewidth',5)        % right eye 
Pm0=[xm;ym;ones(1,n3)];
Pm1=T*Pm0;
xm1=Pm1(1,:);
ym1=Pm1(2,:); 
plot(xm1,ym1,'k','linewidth',2)        % mouth 
grid on;
axis([-3, 3.5, -2.5, 3.5])
hold off
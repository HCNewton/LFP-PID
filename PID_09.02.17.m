clear all;
clc;                                        % Clear workspace and command window
close all;

%%          Set variables
dt                  =   1/600 ;
t                   =   [0:1/600:10];              % dt = 0.00003 s = 0.03ms = 30us 
A                   =   1;                         % amplitude : 1
%f                   =   0.5 + (2*t);               % central frequency : 10 Hz
f                   =   10;   
y                   =   A*sin(2*pi*(f.*t));        % Local field potential 
xy(1:100)           =   1;
loop                =   1;                         % initialise variable for scaling loop
n                   =   1;                         % initialise for array location within scaling loop      

filter13            =   [1 0 -1];   % derivative kernel 
 
filter3             =   -[0 -0.3 -0.6 -1 -0.6 -0.3 0 0.3 0.6 1 0.6 0.3 0]; % 13 taps 
figure;plot(filter3);legend('derivative kernel of 13 taps')

math                =   diff(y);                   % build in matlab differentiation 

 
%%          D type operation 
%%          Convolution of original input function (y)
j                  =   1;                         % initialise loop variables
s                  =   1;
i                  =   1;
%           original signal (y) do convolution (self convolution to replace build-in conv function ) with 13 taps filters 
while j < length (y + 1)                    % will loop 6001 time
    
    while i < length(filter3 + 1)           % will loop 13 times        
    product_1(i)   =   y(j) * filter3(i);   % product of y and filter elements
    i              =   i+1;                 % increment loop
    j              =   j+1;
    conv_1(s)      =   sum(product_1);      % sum product and store in array
    s              =   s + 1;               % increment storage array element   
    end
    
    i = 1;                                  %  reset i
end


%%          Convolution of primary average ( average_1 )
while n < length(y)                                       % length(y) = 6001 run until all neighbouring values have been summed
    average_1(loop)     =   y(n)+y(n+1) +y(n+2) +y(n+3);  % Sum neighbouring values [y(1)+y(2), y(3)+y(4)...]
    n                   =   n+4;
    loop                =   loop+1;
end
j  =   1;                                                 % initialise loop variables
s  =   1;
i  =   1;
%         Paramidal average signal ( average_1) do convolution (self convolution to replace build-in conv function ) with 13 taps filters 
while j < length (average_1 + 1)                          % will loop 1500 time
    
    while i < length(filter3 + 1)                        % will loop 13 times
    product_2(i)   =   average_1(j) * filter3(i);        % product of average_1 and filter elements
    i              =   i+1;                               % increment loop
    j              =   j+1;
    conv_2(s)      =   sum(product_2);                    % sum product and store in array
    s              =   s + 1;                             % increment storage array element
    end
    
    i              =   1;                                 % reset i
end


%%        Convolution of second average ( average_2 )
n    =  1;
loop =  1;

%         This is first paramidal averaging 
while n < length(average_1)                               % length(y) = 6001 run until all neighbouring values have been summed
    average_2(loop)    =   average_1(n)+average_1(n+1) +average_1(n+2) +average_1(n+3); %Sum neighbouring values [y(1)+y(2), y(3)+y(4)...]
    n                  =   n+4;
    loop               =   loop+1;
end

j  =   1; 
s  =   1;
i  =   1;

%         Second average signal ( average_2) do convolution (self convolution to replace build-in conv function ) with 13 taps filters 

while j < length (average_2 + 1)   % will loop 375 time
    while i < length(filter3 + 1) % will loop 13 times
    product_3(i)   =   average_2(j) * filter3(i); %product of y and filter elements
    i =   i+1; %increment loop
    j =   j+1;
    
if (j == length(average_2))
    break;
end
    conv_3(s)  =   sum(product_3); %sum product and store in array
    s          =   s + 1; %increment storage array element
    end
    i          = 1; %reset i
end


figure; subplot(311) ; plot(conv_1) ; legend('Primary Convolution After Paramidal Filtering ');  
         subplot(312) ; plot(conv_2,'r') ; legend('Secondary convolution After Paramidal Filtering ');
           subplot(313) ; plot(conv_3,'g') ; legend(' Third convolution After Paramidal Filtering ');


% figure; subplot(311) ; plot(y) ; legend('raw sin wave ');  title('Scali9ng Results');
%          subplot(312) ; plot(average_1,'r') ; legend('Primary scaling'); 
%            subplot(313) ; plot(average_2,'g') ; legend('Secondary scaling'); 
%            
           
           
%%  Summation  of convoluted signals
conv_1  =   conv_1/17;
conv_2  =   conv_2/29;

for r   = 1:length(conv_3)
    final(r) =  conv_1(r) + conv_2(r) + conv_3(r);
end

for r =  length(conv_3 + 1):length(conv_2)
    final(r) =  conv_1(r) + conv_2(r);
end

for r =  length(conv_2 + 1):length(conv_1)
    final(r) =  conv_1(r);
end 

figure ;  subplot(311); plot(y)     ; legend(' Raw Data ');  
          subplot(312); plot(final) ; legend(' Convolution Derivative Results  ');  
          subplot(313); plot(math)  ; legend(' Build in Convolution Derivative '); 

                 
%% I type : INTEGRATION
for i =   1:length(y);%loop between 1 and final array position
    integral(i) =   sum (y( : , i:length(y) ));%integrate between n and the last array place
end

%% PID
for i    =   1:length(final)
    My_PID(i) =   final(i) + integral(i) +y(i);
end

for i = 1:length(math)
    Math_PID(i) = math(i) + integral(i) + y(i);
end 


figure;  subplot(311)  ; plot(y)            ; legend(' raw data ');  
         subplot(312)  ; plot(My_PID)       ; legend(' self designed PID output ');  
         subplot(313)  ; plot(Math_PID,'r') ; legend(' build in math PID output');
         
         
         
%%  Give a go for fun for derivative for comparison purpose 
self_derivative       =   conv(y,filter3);
build_in_diff         =   diff(y);  

figure ;  subplot(311); plot(y)                ; legend(' Raw Data ');   
          subplot(312); plot(self_derivative)  ; legend(' Convolution derivative ');  
          subplot(313); plot(build_in_diff)    ; legend(' Matlab Build in Convolution '); 
          

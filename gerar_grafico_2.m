clc;
clear;
close all;

arquivo = "dados_ensaio.csv";
dados = csvread(arquivo);

temperatura = dados(:,1);
alvo = dados(:,2);

N = length(temperatura);
t = 1:N;

figure(1);
plot(t, temperatura, 'b', 'linewidth', 2);
hold on;
plot(t, alvo, 'r--', 'linewidth', 2);

grid minor;
box on;

title('Controle de Temperatura');
xlabel('Amostra');
ylabel('Temperatura (°C)');
legend('Temperatura', 'Setpoint', 'location', 'southeast');

set(gca,'FontSize',12);
set(gcf,'Color','w');

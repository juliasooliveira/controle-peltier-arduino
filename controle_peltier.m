% Carrega o arquivo do ensaio
dados = csvread('dados_ensaio.csv');
tempo = dados(:, 1);
temperatura = dados(:, 2);
pwm = dados(:, 3);

% Plota o ensaio em malha aberta
figure;
plot(tempo, temperatura, 'b', 'LineWidth', 2);
grid on;
title('Resposta ao Degrau - Pastilha Peltier');
xlabel('Tempo (segundos)');
ylabel('Temperatura (°C)');

% --- CÁLCULOS DA PLANTA ---
T_inicial = temperatura(1);
T_final = temperatura(end);
delta_y = T_final - T_inicial;
delta_u = 50; % Nosso PWM degrau aplicado

% 1. Ganho Estático (K)
K_planta = delta_y / delta_u;

% 2. Constante de Tempo (tau)[cite: 3]
valor_alvo = T_inicial + 0.632 * delta_y;
[~, idx_tau] = min(abs(temperatura - valor_alvo));
tau_planta = tempo(idx_tau) - 10; % Subtrai os 10s iniciais de atraso forçado

% 3. Tempo Morto (L)[cite: 3]
% Analise visualmente seu gráfico para refinar esse valor.
% É o tempo que demorou entre os 10 segundos (aplicação do degrau)
% e a reação perceptível do sensor. Exemplo padrão:
L_planta = 3.0;

% --- SINTONIA PID DE ZIEGLER-NICHOLS ---
Kp = (1.2 * tau_planta) / (K_planta * L_planta); %
Ti = 2 * L_planta; %
Td = 0.5 * L_planta; %

% Ganhos convertidos para a biblioteca do Arduino (PID_v1.h)[cite: 2]
Ki = Kp / Ti; %[cite: 2]
Kd = Kp * Td; %[cite: 2]

% Mostra os valores reais calculados no Command Window
fprintf('\n================ RESULTADOS ================\n');
fprintf('Modelo Obtido: G(s) = (%.4f / (%.2fs + 1)) * e^(-%.1fs)\n', K_planta, tau_planta, L_planta);
fprintf('--------------------------------------------\n');
fprintf('Valores reais para colar no seu código do Arduino:\n');
fprintf('double Kp = %.4f;\n', Kp);
fprintf('double Ki = %.4f;\n', Ki);
fprintf('double Kd = %.4f;\n', Kd);
fprintf('============================================\n');

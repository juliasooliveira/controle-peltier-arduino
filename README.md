# Controle Digital de Temperatura - Pastilha Peltier (UFOP)

Este repositório contém o código-fonte e as ferramentas desenvolvidas para o projeto de controle térmico digital em malha fechada utilizando uma plataforma Arduino e uma Pastilha Termoelétrica Peltier (TEC1-12706).

## 📋 Estrutura do Repositório

*   `/codigo_peltier_pid.ino`: Código principal do Arduino contendo a lógica do PID, leitura do sensor DS18B20 e display OLED.
*   `/gerar_grafico.m`: Script em GNU Octave para tratamento dos dados e geração das curvas dinâmicas.
*   `/dados_ensaio.csv`: Dados de temperatura e PWM obtidos durante os ensaios práticos.

## 🛠️ Requisitos de Hardware

*   Arduino Uno
*   Pastilha Peltier TEC1-12706
*   MOSFET IRLZ44N
*   Sensor de temperatura DS18B20
*   Display OLED SSD1306
*   Dissipador térmico de alumínio com Cooler 12V (essencial para dissipação ativa)

## ✍️ Autores

Trabalho desenvolvido para a disciplina de Laboratório de Controle e Automação - UFOP.
*   Lorena C. C. Melo
*   Geovanne G. Vieira
*   Julia S. Oliveira
*   Guilherme de Oliveira Rosa

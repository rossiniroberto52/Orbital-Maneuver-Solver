import pandas as pd
import matplotlib.pyplot as plt

# 1. Carregar os dados
try:
    df = pd.read_csv('telemetry_suicide_burn.csv')
except FileNotFoundError:
    print("ERRO: O arquivo 'telemetry_suicide_burn.csv' não foi encontrado.")
    print("Rode o executável C++ primeiro (make run).")
    exit()

# 2. Configurar a área de plotagem (2 gráficos)
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10), sharex=True)

# --- GRÁFICO 1: O "Suicide Burn" (Altitude vs Velocidade) ---
# O eixo X é o Tempo, mas queremos ver a correlação Altura/Velocidade
# Vamos plotar duas linhas no mesmo eixo temporal
color = 'tab:red'
ax1.set_xlabel('Tempo (s)')
ax1.set_ylabel('Altitude (m)', color=color)
ax1.plot(df['Time'], df['Altitude'], color=color, linewidth=2, label='Altitude')
ax1.tick_params(axis='y', labelcolor=color)
ax1.grid(True, linestyle='--', alpha=0.6)

# Criar um segundo eixo Y (twin) para a velocidade
ax1_twin = ax1.twinx()  
color = 'tab:blue'
ax1_twin.set_ylabel('Velocidade (m/s)', color=color)
ax1_twin.plot(df['Time'], df['Velocity'], color=color, linewidth=2, linestyle='-', label='Velocidade')
ax1_twin.tick_params(axis='y', labelcolor=color)

ax1.set_title('Simulação de Suicide Burn: Perfil de Voo')

# --- GRÁFICO 2: Aceleração e Força G (Prova de Física) ---
# Aqui veremos se você simulou a massa variável corretamente.
# A aceleração DEVE aumentar conforme o tempo passa (nave mais leve).
color = 'tab:green'
ax2.set_xlabel('Tempo (s)')
ax2.set_ylabel('Força G (gs)', color=color)
ax2.plot(df['Time'], df['G_Force'], color=color, linewidth=2, label='Força G')
ax2.tick_params(axis='y', labelcolor=color)
ax2.grid(True)
ax2.set_title('Telemetria do Motor: Aumento de TWR (Thrust-to-Weight)')

# Anotação Crítica
final_g = df['G_Force'].iloc[-1]
initial_g = df['G_Force'].iloc[0]
ax2.annotate(f'Início: {initial_g:.2f} G', xy=(0, initial_g), xytext=(0.5, initial_g+0.5),
             arrowprops=dict(facecolor='black', shrink=0.05))
ax2.annotate(f'Fim: {final_g:.2f} G', xy=(df['Time'].iloc[-1], final_g), xytext=(df['Time'].iloc[-1]-1, final_g-0.5),
             arrowprops=dict(facecolor='black', shrink=0.05))

plt.tight_layout()
plt.savefig('suicide_burn_analysis.png') # Salva a imagem
plt.show() # Mostra a janela
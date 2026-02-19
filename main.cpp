#include <iostream>
#include "PhysicsCore.hpp" 

// Usar o namespace específico é aceitável aqui na main para limpar a sintaxe,
// mas evite isso em headers (.hpp).
using namespace KSPPhysics;

int main() {
    // 1. Configurar a nave de teste
    // Ordem dos campos na Struct: { massTotal, massDry, maxThrust, isp, height }
    // Preenchemos TODOS para evitar erro do compilador.
    VesselStats myLander = { 
        10000.0,   // Massa Total (kg)
        8000.0,    // Massa Seca (kg) - Estimativa realista
        200000.0,  // Empuxo (N) - 200kN convertidos
        300.0,     // ISP (s)
        0.0        // Altura (Placeholder, não usado no input da simulação)
    };

    // 2. Cenário: Caindo a 100 m/s (Negativo ou positivo, tratamos com abs)
    double currentSpeed = 100.0; 

    std::cout << "--- SISTEMA DE GUIAGEM E NAVEGACAO (GNC) ---" << std::endl;
    std::cout << "Veiculo: Lander Class-A (10t)" << std::endl;
    std::cout << "Velocidade Vertical: " << currentSpeed << " m/s" << std::endl;

    // 3. Executar o cálculo numérico
    // Chamamos a função estática da classe ManeuverSolver
    double burnAltitude = ManeuverSolver::CalculateSuicideBurnHeight(myLander, currentSpeed);

    std::cout << "---------------------------------------" << std::endl;
    std::cout << "SOLUCAO DE MANOBRA ENCONTRADA:" << std::endl;
    // Formatando para leitura fácil
    std::cout << ">> ALTITUDE DE QUEIMA (SUICIDE BURN): " << burnAltitude << " metros" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // 4. Exportar dados para análise externa (Python)
    ExportTelemetry(myLander, currentSpeed, "telemetry_suicide_burn.csv");

    return 0;
}
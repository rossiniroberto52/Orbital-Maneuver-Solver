#pragma once
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>

namespace KSPPhysics { // Namespace consistente com sua main
    
    // Constantes organizadas
    namespace Constants {
        constexpr double G_CONST = 6.67430e-11;
        constexpr double KERBIN_RADIUS = 600000.0;
        constexpr double KERBIN_MASS = 5.2915158e22; 
        constexpr double G0_STANDARD = 9.80665; 
        constexpr double KERBIN_GRAVITY_ASL = 9.81; 
    }

    struct VesselStats {
        double massTotal; // kg
        double massDry;   // kg
        double maxThrust; // Newtons
        double isp;       // Segundos
        double height;    // m (opcional, mas estava na sua struct)
    };

    class ManeuverSolver {
    public:
        // Função auxiliar para gravidade
        static double GetLocalGravity(double altitude) {
            double r = Constants::KERBIN_RADIUS + altitude;
            return (Constants::G_CONST * Constants::KERBIN_MASS) / (r * r);
        }

        // A CORREÇÃO REAL: Simulação Numérica
        static double CalculateSuicideBurnHeight(const VesselStats& vessel, double currentVerticalSpeed, double dt = 0.01) {
            
            double simHeight = 0.0;       
            double simVelocity = 0.0;     
            double simMass = vessel.massTotal; 
            
            // Taxa de consumo: Thrust / (Isp * g0)
            double massFlowRate = vessel.maxThrust / (vessel.isp * Constants::G0_STANDARD);

            double targetVelocity = std::abs(currentVerticalSpeed);

            // Loop de Integração (Euler)
            while (simVelocity < targetVelocity) {
                double localG = GetLocalGravity(simHeight);
                double accelThrust = vessel.maxThrust / simMass;
                double netAccel = accelThrust - localG;

                simVelocity += netAccel * dt;
                simHeight   += simVelocity * dt;
                simMass     -= massFlowRate * dt;

                // Fail-safe para massa não virar negativa
                if (simMass < vessel.massDry) simMass = vessel.massDry;
            }

            return simHeight;
        }
    };

    static void ExportTelemetry(const VesselStats& vessel, double currentVerticalSpeed, std::string filename) {
    std::ofstream file(filename);
    
    // Cabeçalho do CSV
    file << "Time,Altitude,Velocity,Mass,Acceleration,G_Force\n";

    double simHeight = 0.0;       
    double simVelocity = 0.0;     
    double simMass = vessel.massTotal; 
    double time = 0.0;
    double dt = 0.05; // Passo um pouco maior para não gerar linhas demais

    double massFlowRate = vessel.maxThrust / (vessel.isp * Constants::G0_STANDARD);
    double targetVelocity = std::abs(currentVerticalSpeed);

    std::cout << "Gerando telemetria para: " << filename << "..." << std::endl;

    while (simVelocity < targetVelocity) {
        double localG = ManeuverSolver::GetLocalGravity(simHeight);
        double accelThrust = vessel.maxThrust / simMass;
        double netAccel = accelThrust - localG;

        // Salva os dados ANTES de atualizar
        // G_Force = Aceleração sentida pela tripulação (Apenas o empuxo / 9.81)
        double gForce = accelThrust / 9.81;

        file << time << "," 
             << simHeight << "," 
             << simVelocity << "," 
             << simMass << "," 
             << netAccel << ","
             << gForce << "\n";

        // Física
        simVelocity += netAccel * dt;
        simHeight   += simVelocity * dt;
        simMass     -= massFlowRate * dt;
        time        += dt;

        if (simMass < vessel.massDry) simMass = vessel.massDry;
    }

    file.close();
    std::cout << "Telemetria salva com sucesso!" << std::endl;
    }
}
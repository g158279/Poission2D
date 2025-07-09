#pragma once

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

#include "mesh.h"
#include "utils.h"
#include "solver.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>

namespace CPPPOISSON
{

    class RunPoisson2D
    {
    public:
        explicit RunPoisson2D(int argc, char* argv[]) {};
        ~RunPoisson2D() = default;
        RunPoisson2D(const RunPoisson2D&) = delete;
        RunPoisson2D& operator=(const RunPoisson2D&) = delete;

        MYLIBRARY_API bool readFromJson(const std::filesystem::path& jsonPath);
        MYLIBRARY_API bool simulate() const;

        struct PoissonDef
        {
            int m_nx;
            int m_ny;
            int m_maxIter;
            double m_lx;
            double m_ly;
            double m_leftBC;
            double m_rightBC;
            double m_topBC;
            double m_bottomBC;
            double m_rTol;
            double m_aTol;
            size_t m_gaussN;
            std::string m_shape;
            std::string m_outputPath;
            std::string m_logPath;
            Eigen::VectorXd m_uSol;
        };

        std::unique_ptr<PoissonDef> mp_def;
        std::unique_ptr<FEMProblem> mp_problem;
        std::shared_ptr<Mesh> mp_mesh;

    private:
        ;
    };

}   // namespace CPPPOISSON

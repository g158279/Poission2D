#ifndef PREPROCESS_H
#define PREPROCESS_H

#ifdef POISSON_EXPORTS
#define PREPROCESS_API __declspec(dllexport)
#else
#define PREPROCESS_API __declspec(dllimport)
#endif

#include <Eigen/Dense> //´ýmuparserÌæ»»
#include <iostream>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>

namespace CPPPOISSON
{
    struct PoissonDef
    {
        int nx;
        int ny;
        int maxIter;
        double lx;
        double ly;
        double leftBC;
        double rightBC;
        double topBC;
        double bottomBC;
        double rTol;
        double aTol;
        size_t gaussN;
        std::string shape;
        std::string u0;
        std::string f;
        std::string df_du;
        std::string outputPath;
        std::string logPath;
        Eigen::VectorXd uSol;
    };

    class PREPROCESS_API PreProcess
    {
    public:
        explicit PreProcess() {};
        bool readFromJson(const std::filesystem::path& jsonPath);
    private:
        PoissonDef m_def;
    };
    
}

#endif // PREPROCESS_H

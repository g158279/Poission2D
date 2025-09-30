#pragma once

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
#include "geometry.h"

namespace CPPPOISSON
{
	struct PoissonDef
	{
		int nx{ 0 };
		int ny{ 0 };
		int maxIter{ 0 };
		double lx{ 0.0 };
		double ly{ 0.0 };
		double leftBC{ 0.0 };
		double rightBC{ 0.0 };
		double bottomBC{ 0.0 };
		double topBC{ 0.0 };
		double rTol{ 0.0 };
		double aTol{ 0.0 };
		size_t gaussN{ 0 };
		std::string shape{ "" };
		std::string outputPath{ "" };
		std::string logPath{ "" };
		Eigen::VectorXd uSol;
		std::function<double(double)> f;
		std::function<double(double)> df_du;
		std::function<double(double, double)> u0;
		std::map<BoundaryType, double> dirichletBC;
		Boundary boundary;
	};

	class PREPROCESS_API PreProcess
	{
	public:
		explicit PreProcess(const std::filesystem::path& jsonPath);
		~PreProcess() = default;
		PreProcess(const PreProcess&) = delete;
		PreProcess& operator=(const PreProcess&) = delete;
		PreProcess(PreProcess&&) = delete;
		PreProcess& operator=(PreProcess&&) = delete;

		const PoissonDef& getDef() { return m_def; };
	private:
		void readFromJson(const std::filesystem::path& jsonPath);
		std::function<double(double)> parserFuncX(const std::string& exprStr);
		std::function<double(double, double)> parserFuncXY(const std::string& exprStr);
		PoissonDef m_def;
	};
}


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
		std::string u0{ "" };
		std::string f{ "" };
		std::string df_du{ "" };
		std::string outputPath{ "" };
		std::string logPath{ "" };
		Eigen::VectorXd uSol;
		Boundary boundary;
	};

	class PREPROCESS_API PreProcess
	{
	public:
		explicit PreProcess() {};
		~PreProcess() = default;
		PreProcess(const PreProcess&) = delete;
		PreProcess& operator=(const PreProcess&) = delete;
		PreProcess(PreProcess&&) = delete;
		PreProcess& operator=(PreProcess&&) = delete;

		void readFromJson(const std::filesystem::path& jsonPath);
		PoissonDef& getDef() { return m_def; };
	private:
		PoissonDef m_def;
	};
}


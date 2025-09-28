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

#include "mesh.h"

namespace CPPPOISSON
{
	enum class BoundaryType {
		Left,
		Right,
		Top,
		Bottom,
	};

	struct Boundary {
		bool isOnBoundary(const Point& point, BoundaryType boundaryType) const;
		double xLeft{ 0.0 };
		double xRight{ 1.0 };
		double yBottom{ 0.0 };
		double yTop{ 1.0 };
	private:
		double tol{ 1e-8 };
	};

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
		PreProcess() {};
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


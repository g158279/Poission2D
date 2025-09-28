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
		int nx;
		int ny;
		int maxIter;
		double lx;
		double ly;
		double leftBC;
		double rightBC;
		double bottomBC;
		double topBC;
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
		Boundary boundary;
	};

	class PREPROCESS_API PreProcess
	{
	public:
		explicit PreProcess() {};
		void readFromJson(const std::filesystem::path& jsonPath);
		PoissonDef& getDef() { return m_def; };
	private:
		PoissonDef m_def;
	};
}


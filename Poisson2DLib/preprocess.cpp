#include "preprocess.h"

namespace CPPPOISSON
{
	void PreProcess::readFromJson(const std::filesystem::path& jsonPath)
	{
		using json = nlohmann::json;

		std::ifstream input_file(jsonPath);

		json config;
		input_file >> config;

		m_def.boundary.xRight = config["mesh"]["lx"].get<double>();
		m_def.boundary.yTop = config["mesh"]["ly"].get<double>();
		m_def.nx = config["mesh"]["nx"].get<int>();
		m_def.ny = config["mesh"]["ny"].get<int>();
		m_def.shape = config["mesh"]["shape"].get<std::string>();

		m_def.gaussN = config["gauss_quadrature"]["n"].get<int>();

		if (config["boundary_conditions"].contains("left"))
		{
			m_def.leftBC = config["boundary_conditions"]["left"].get<double>();
		}
		else
		{
			m_def.leftBC = -1.0;
			std::cout << "Left boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("right"))
		{
			m_def.rightBC = config["boundary_conditions"]["right"].get<double>();
		}
		else
		{
			m_def.rightBC = -1.0;
			std::cout << "Right boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("top"))
		{
			m_def.topBC = config["boundary_conditions"]["top"].get<double>();
		}
		else
		{
			m_def.topBC = -1.0;
			std::cout << "Top boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("bottom"))
		{
			m_def.bottomBC = config["boundary_conditions"]["bottom"].get<double>();
		}
		else
		{
			m_def.bottomBC = -1.0;
			std::cout << "Bottom boundary condition not set." << std::endl;
		}

		m_def.rTol = config["solver"]["rtol"].get<double>();
		m_def.aTol = config["solver"]["atol"].get<double>();
		m_def.maxIter = config["solver"]["max_iter"].get<double>();
		m_def.outputPath = config["output"]["outputPath"].get<std::string>();
		m_def.logPath = config["output"]["logPath"].get<std::string>();
		m_def.u0 = config["functions"]["u0"].get<std::string>();
		m_def.f = config["functions"]["f"].get<std::string>();
		m_def.df_du = config["functions"]["df_du"].get<std::string>();
	}

	bool CPPPOISSON::Boundary::isOnBoundary(const Point& point, BoundaryType boundaryType) const
	{
		if (boundaryType == BoundaryType::Left)
		{
			return std::fabs(point.x() - xLeft) < tol;
		}
		else if (boundaryType == BoundaryType::Right)
		{
			return std::fabs(point.x() - xRight) < tol;
		}
		else if (boundaryType == BoundaryType::Bottom)
		{
			return std::fabs(point.y() - yBottom) < tol;
		}
		else if (boundaryType == BoundaryType::Top)
		{
			return std::fabs(point.y() - yTop) < tol;
		}
	}
}
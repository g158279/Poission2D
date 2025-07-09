#include "mesh.h"
#include "utils.h"
#include "solver.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include "RunPoisson2D.h"

using json = nlohmann::json;

namespace CPPPOISSON
{
	bool RunPoisson2D::readFromJson(const std::filesystem::path& jsonPath)
	{
		using json = nlohmann::json;

		mp_def = std::make_unique<RunPoisson2D::PoissonDef>();
		std::ifstream input_file(jsonPath);

		json config;
		input_file >> config;

		mp_def->m_lx = config["mesh"]["lx"].get<double>();
		mp_def->m_ly = config["mesh"]["ly"].get<double>();
		mp_def->m_nx = config["mesh"]["nx"].get<int>();
		mp_def->m_ny = config["mesh"]["ny"].get<int>();
		mp_def->m_shape = config["mesh"]["shape"].get<std::string>();

		mp_def->m_gaussN = config["gauss_quadrature"]["n"].get<int>();

		if (config["boundary_conditions"].contains("left"))
		{
			mp_def->m_leftBC = config["boundary_conditions"]["left"].get<double>();
		}
		else
		{
			mp_def->m_leftBC = -1.0;
			std::cout << "Left boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("right"))
		{
			mp_def->m_rightBC = config["boundary_conditions"]["right"].get<double>();
		}
		else
		{
			mp_def->m_rightBC = -1.0;
			std::cout << "Right boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("top"))
		{
			mp_def->m_topBC = config["boundary_conditions"]["top"].get<double>();
		}
		else
		{
			mp_def->m_topBC = -1.0;
			std::cout << "Top boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("bottom"))
		{
			mp_def->m_bottomBC = config["boundary_conditions"]["bottom"].get<double>();
		}
		else
		{
			mp_def->m_bottomBC = -1.0;
			std::cout << "Bottom boundary condition not set." << std::endl;
		}

		mp_def->m_rTol = config["solver"]["rtol"].get<double>();
		mp_def->m_aTol = config["solver"]["atol"].get<double>();
		mp_def->m_maxIter = config["solver"]["max_iter"].get<double>();
		mp_def->m_outputPath = config["output"]["outputPath"].get<std::string>();
		mp_def->m_logPath = config["output"]["logPath"].get<std::string>();
		std::string meshShape = mp_def->m_shape;
		std::transform(meshShape.begin(), meshShape.end(), meshShape.begin(), ::tolower);
		if (meshShape == "triangular" || meshShape == "tri" || meshShape == "t")
		{
			std::cout << "Calculating based on triangular mesh" << std::endl;
			mp_mesh = std::make_shared<TriangularMesh>(mp_def->m_lx, mp_def->m_ly, mp_def->m_nx, mp_def->m_ny);
		}
		else if (meshShape == "rectangular" || meshShape == "rect" || meshShape == "r")
		{
			std::cout << "Calculating based on rectangular mesh" << std::endl;
			mp_mesh = std::make_shared<RectangularMesh>(mp_def->m_lx, mp_def->m_ly, mp_def->m_nx, mp_def->m_ny);
		}
		else
		{
			std::cerr << "Wrong mesh name." << std::endl;
			return false;
		}

		// 函数输入的parsing待实现
		const auto u0 = [](const std::vector<Eigen::Vector2d>& points) -> Eigen::VectorXd
		{
			Eigen::VectorXd u(points.size());
			for (size_t i = 0; i < points.size(); ++i)
			{
				u(i) = std::exp(-(points[i][0] + points[i][1]));
			}
			return u;
		};

		const auto f = [](double u) -> double
		{
			return std::exp(-u);
		};
		const auto df_du = [](double u) -> double
		{
			return -std::exp(-u);
		};

		// 用于解析结果对比
		//const auto f = [](double u) -> double {
		//    return 0.;
		//    };
		//const auto df_du = [](double u) -> double {
		//    return 0.;
		//    };

		std::vector<Eigen::Vector2d> gauss_points;
		std::vector<double> weights;
		gauss_quadrature_2d(mp_def->m_gaussN, gauss_points, weights); // 计算高斯积分网格点与权重
		mp_problem = std::make_unique<FEMProblem>(mp_mesh, f, df_du, u0, gauss_points, weights);
		if (mp_def->m_leftBC > 0.0)
		{
			mp_problem->setBoundaryConditionOnEdge("left", mp_def->m_leftBC);
		}
		if (mp_def->m_rightBC > 0.0)
		{
			mp_problem->setBoundaryConditionOnEdge("right", mp_def->m_rightBC);
		}
		if (mp_def->m_topBC > 0.0)
		{
			mp_problem->setBoundaryConditionOnEdge("top", mp_def->m_topBC);
		}
		if (mp_def->m_bottomBC > 0.0)
		{
			mp_problem->setBoundaryConditionOnEdge("bottom", mp_def->m_bottomBC);
		}
		return true;
	};

	bool RunPoisson2D::simulate() const
	{
		const double lx = mp_def->m_lx, ly = mp_def->m_ly;
		const int nx = mp_def->m_nx, ny = mp_def->m_ny;

		bool res = mp_problem->solve(mp_def->m_rTol, mp_def->m_aTol, mp_def->m_maxIter, mp_def->m_uSol, mp_def->m_logPath);
		if (!res)
		{
			std::cerr << "Not converged within the maximum number of iterations." << std::endl;
			return false;
		}
		else
		{
			Eigen::Map<Eigen::MatrixXd>u_sol_mat(mp_def->m_uSol.data(), nx, ny);
			//std::cout << "Numerical solution:" << std::endl;
			//std::cout << u_sol_mat.transpose().colwise().reverse() << std::endl;

			std::ofstream file(mp_def->m_outputPath);
			if (file.is_open())
			{
				file << lx << " " << ly << " " << nx << " " << ny << "\n";
				file << u_sol_mat.transpose().colwise().reverse().format(Eigen::IOFormat(Eigen::FullPrecision, Eigen::DontAlignCols, " ", "\n"));
				file.close();
				std::cout << "Solution saved." << std::endl;
			}
			else
			{
				std::cerr << "Unable to open file for writing." << std::endl;
			}
			return true;
		}
	};
} // namespace CPPPOISSON

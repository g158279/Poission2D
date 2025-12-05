#include "preprocess.h"
#include <muParser.h>

namespace CPPPOISSON
{
	PreProcess::PreProcess(const std::filesystem::path& jsonPath)
	{
		readFromJson(jsonPath);
	}

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
			m_def.dirichletBC[BoundaryType::Left] = config["boundary_conditions"]["left"].get<double>();
		}
		else
		{
			std::cout << "Left boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("right"))
		{
			m_def.dirichletBC[BoundaryType::Right] = config["boundary_conditions"]["right"].get<double>();
		}
		else
		{
			std::cout << "Right boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("top"))
		{
			m_def.dirichletBC[BoundaryType::Top] = config["boundary_conditions"]["top"].get<double>();
		}
		else
		{
			std::cout << "Top boundary condition not set." << std::endl;
		}
		if (config["boundary_conditions"].contains("bottom"))
		{
			m_def.dirichletBC[BoundaryType::Bottom] = config["boundary_conditions"]["bottom"].get<double>();
		}
		else
		{
			std::cout << "Bottom boundary condition not set." << std::endl;
		}

		m_def.rTol = config["solver"]["rtol"].get<double>();
		m_def.aTol = config["solver"]["atol"].get<double>();
		m_def.maxIter = config["solver"]["max_iter"].get<double>();
		m_def.outputPath = config["output"]["outputPath"].get<std::string>();
		m_def.logPath = config["output"]["logPath"].get<std::string>();
		m_def.u0 = parserFuncXY(config["functions"]["u0"].get<std::string>());
		m_def.f = parserFuncX(config["functions"]["f"].get<std::string>());
		m_def.df_du = parserFuncX(config["functions"]["df_du"].get<std::string>());
	}

	std::function<double(double)> PreProcess::parserFuncX(const std::string& exprStr)
	{
		mu::Parser parser;
		parser.SetExpr(exprStr);
		std::shared_ptr<double> x = std::make_shared<double>(0.0);
		parser.DefineVar("u", x.get());

		return [parser, x, exprStr](double input) mutable {
			*x = input;
			try {
				return parser.Eval();
			}
			catch (const mu::Parser::exception_type& ex) {
				std::string errorMSG{ "Error involving parsing function string " };
				errorMSG += exprStr + ": " + ex.GetMsg();
				throw std::invalid_argument(errorMSG);
			}
			};
	}

	std::function<double(double, double)> PreProcess::parserFuncXY(const std::string& exprStr)
	{
		mu::Parser parser;
		parser.SetExpr(exprStr);
		std::shared_ptr<double> x = std::make_shared<double>(0.0);
		std::shared_ptr<double> y = std::make_shared<double>(0.0);
		parser.DefineVar("x", x.get());
		parser.DefineVar("y", y.get());

		return [parser, x, y, exprStr](double inputX, double inputY) mutable {
			*x = inputX;
			*y = inputY;
			try {
				return parser.Eval();
			}
			catch (const mu::Parser::exception_type& ex) {
				std::string errorMSG{ "Error involving parsing function string " };
				errorMSG += exprStr + ": " + ex.GetMsg();
				throw std::invalid_argument(errorMSG);
			}
			};
	}

	bool Boundary::isOnBoundary(const Point& point, BoundaryType boundaryType) const
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
		else {
			return false;
		}
	}
}
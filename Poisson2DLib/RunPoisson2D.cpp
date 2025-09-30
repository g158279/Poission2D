#include "RunPoisson2D.h"
#include "RunPoisson2D.h"
#include "mesh.h"
#include "preprocess.h"
//#include "utils.h"
//#include "solver.h"

using json = nlohmann::json;

namespace CPPPOISSON
{
	RunPoisson2D::RunPoisson2D(const Mesh& mesh, const PoissonDef& poissonDef) :m_mesh{ mesh }, m_poissonDef{ poissonDef }
	{
		initDirichletBC();
		initSolution();
		//FEMProblem problem(mesh, poissonDef);
	}


	void RunPoisson2D::initDirichletBC()
	{
		for (const auto& point:m_mesh.getAllPoints()) {
			for (const auto& DBC : m_poissonDef.dirichletBC) {
				bool isOnBoundary = m_poissonDef.boundary.isOnBoundary(*point, DBC.first);
				if (isOnBoundary) {
					m_dirichletBC[point->index()]=DBC.second;
				}
			}
		}
	}

	void RunPoisson2D::initSolution()
	{
		//for (size_t i = 0; i < m_mesh.getPointSize(); i++) {
		//	const auto& point = m_mesh.getPoint(i);
		//	size_t index = point.index();
		//	double x = point.x();
		//	double y = point.y();
		//	m_solution(index) = m_config.funcGuess(x, y);
		//}

		//for (const auto& valueIndicesPair : m_dirichletBC) {
		//	const auto& vec = valueIndicesPair.first;
		//	const double val = valueIndicesPair.second;
		//	for (size_t n : vec) {
		//		m_solution(n) = val;
		//	}
		//}

	}

	//bool RunPoisson2D::genProblem() {
	//	if (m_def.m_leftBC > 0.0)
	//	{
	//		m_problem.setBoundaryConditionOnEdge("left", m_def.m_leftBC);
	//	}
	//	if (m_def.m_rightBC > 0.0)
	//	{
	//		m_problem.setBoundaryConditionOnEdge("right", m_def.m_rightBC);
	//	}
	//	if (m_def.m_topBC > 0.0)
	//	{
	//		m_problem.setBoundaryConditionOnEdge("top", m_def.m_topBC);
	//	}
	//	if (m_def.m_bottomBC > 0.0)
	//	{
	//		m_problem.setBoundaryConditionOnEdge("bottom", m_def.m_bottomBC);
	//	}
	//	return true;
	//}


	//void RunPoisson2D::simulate() const
	//{
	//	const double lx = m_def.m_lx, ly = m_def.m_ly;
	//	const int nx = m_def.m_nx, ny = m_def.m_ny;

	//	bool res = mp_problem->solve(m_def.m_rTol, m_def.m_aTol, m_def.m_maxIter, m_def.m_uSol, m_def.m_logPath);
	//	if (!res)
	//	{
	//		std::cerr << "Not converged within the maximum number of iterations." << std::endl;
	//	}
	//	else
	//	{
	//		Eigen::Map<Eigen::MatrixXd>u_sol_mat(m_def.m_uSol.data(), nx, ny);
	//		//std::cout << "Numerical solution:" << std::endl;
	//		//std::cout << u_sol_mat.transpose().colwise().reverse() << std::endl;

	//		std::ofstream file(m_def.m_outputPath);
	//		if (file.is_open())
	//		{
	//			file << lx << " " << ly << " " << nx << " " << ny << "\n";
	//			file << u_sol_mat.transpose().colwise().reverse().format(Eigen::IOFormat(Eigen::FullPrecision, Eigen::DontAlignCols, " ", "\n"));
	//			file.close();
	//			std::cout << "Solution saved." << std::endl;
	//		}
	//		else
	//		{
	//			std::cerr << "Unable to open file for writing." << std::endl;
	//		}
	//	}
	//}
} // namespace CPPPOISSON

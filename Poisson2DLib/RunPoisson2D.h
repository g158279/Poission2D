#pragma once

#ifdef POISSON_EXPORTS
#define RUNPOISSON2D_API __declspec(dllexport)
#else
#define RUNPOISSON2D_API __declspec(dllimport)
#endif


#include "mesh.h"
//#include "utils.h"
//#include "solver.h"

namespace CPPPOISSON
{

    class RUNPOISSON2D_API RunPoisson2D
	{
	public:
		explicit RunPoisson2D(const Mesh& mesh, const PoissonDef& poissonDef);
		~RunPoisson2D() = default;
		RunPoisson2D(const RunPoisson2D&) = delete;
		RunPoisson2D& operator=(const RunPoisson2D&) = delete;
		RunPoisson2D(RunPoisson2D&&) = delete;
		RunPoisson2D& operator=(RunPoisson2D&&) = delete;

		void simulate() const;

    private:
		void initDirichletBC();
		void initSolution();
		const Mesh& m_mesh;
		const PoissonDef& m_poissonDef;
		std::map<int, double> m_dirichletBC;
		//std::vector <std::pair<std::vector<size_t>, double>> m_dirichletBC;
		Eigen::VectorXd m_solution;
	};
}   // namespace CPPPOISSON

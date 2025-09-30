#pragma once

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

#include "mesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <map>
#include <vector>
#include <string>
#include <functional>

namespace CPPPOISSON
{

	class MYLIBRARY_API FEMProblem
	{
	public:
		explicit FEMProblem(const Mesh& mesh, const PoissonDef& poissonDef,
			const std::map<int, double>& boundary_conditions = {});
		~FEMProblem() = default;
		FEMProblem(const FEMProblem&) = delete;
		FEMProblem& operator=(const FEMProblem&) = delete;
		FEMProblem(FEMProblem&&) = delete;
		FEMProblem& operator=(FEMProblem&&) = delete;

		void setBoundaryConditionOnEdge(const std::string& boundary, double value);
#ifdef USE_SPARSE_MATRIX
		std::pair<Eigen::SparseMatrix<double>, Eigen::VectorXd> assembleGlobalMatricesAndVector() const;
		void applyDirichletBoundaryConditions(Eigen::SparseMatrix<double>& K, Eigen::VectorXd& F);
#else
		std::pair<Eigen::MatrixXd, Eigen::VectorXd> assembleGlobalMatricesAndVector() const;
		void applyDirichletBoundaryConditions(Eigen::MatrixXd& K, Eigen::VectorXd& F);
#endif
		bool solve(double rtol, double atol, int max_iter, Eigen::VectorXd& u_sol, const std::string& output);

		std::map<int, double> m_boundary_conditions;
		Eigen::VectorXd m_u;

	private:
		std::vector<Eigen::Vector2d> m_gauss_points;
		std::vector<double> m_weights;
	};
}   // namespace CPPPOISSON


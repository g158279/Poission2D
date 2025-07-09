#ifndef FEMPROBLEM_H
#define FEMPROBLEM_H

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
		FEMProblem(const std::shared_ptr<Mesh>& mesh,
			std::function<double(double)> f,
			std::function<double(double)> df_du,
			std::function<Eigen::VectorXd(const std::vector<Eigen::Vector2d>&)> u0,
			const std::vector<Eigen::Vector2d>& gauss_points,
			const std::vector<double>& weights,
			const std::map<int, double>& boundary_conditions = {});

		virtual ~FEMProblem();

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
		std::shared_ptr<Mesh> mp_mesh;
		std::function<double(double)> m_f;
		std::function<double(double)> m_df_du;
		std::vector<Eigen::Vector2d> m_gauss_points;
		std::vector<double> m_weights;
	};
}   // namespace CPPPOISSON

#endif // FEMPROBLEM_H

#include "solver.h"
#include "utils.h"
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace CPPPOISSON
{
	FEMProblem::FEMProblem(const std::shared_ptr<Mesh>& mesh,
		std::function<double(double)> f,
		std::function<double(double)> df_du,
		std::function<Eigen::VectorXd(const std::vector<Eigen::Vector2d>&)> u0,
		const std::vector<Eigen::Vector2d>& gauss_points,
		const std::vector<double>& weights,
		const std::map<int, double>& boundary_conditions)
		: mp_mesh(mesh), m_f(f), m_df_du(df_du), m_gauss_points(gauss_points), m_weights(weights), m_boundary_conditions(boundary_conditions)
	{
		m_u = u0(mesh->m_points);
	}

	FEMProblem::~FEMProblem() {}

	void FEMProblem::setBoundaryConditionOnEdge(const std::string& boundary, double value)
	{
		std::vector<int> nodes = mp_mesh->getBoundaryNodes(boundary);
		for (int node : nodes)
		{
			m_boundary_conditions[node] = value;
		}
	}

#ifdef USE_SPARSE_MATRIX
	std::pair<Eigen::SparseMatrix<double>, Eigen::VectorXd> FEMProblem::assembleGlobalMatricesAndVector() const
#else
	std::pair<Eigen::MatrixXd, Eigen::VectorXd> FEMProblem::assembleGlobalMatricesAndVector() const
#endif
	{
		const std::vector<Eigen::Vector2d>& points = mp_mesh->m_points;
		const std::vector<std::vector<int>>& elements = mp_mesh->m_elements;

		int total_dofs = points.size();
#ifdef USE_SPARSE_MATRIX
		Eigen::SparseMatrix<double> K(total_dofs, total_dofs);
#else
		Eigen::MatrixXd K = Eigen::MatrixXd::Zero(total_dofs, total_dofs);
#endif
		Eigen::VectorXd F = Eigen::VectorXd::Zero(total_dofs);
		for (const auto& element : elements)
		{
			int num_nodes = element.size();
			Eigen::MatrixXd local_K = Eigen::MatrixXd::Zero(num_nodes, num_nodes);
			Eigen::VectorXd local_F = Eigen::VectorXd::Zero(num_nodes);

			Eigen::MatrixXd node_coords(num_nodes, points[0].size());
			Eigen::VectorXd u_values(num_nodes);
			for (int i = 0; i < num_nodes; ++i)
			{
				node_coords.row(i) = points[element[i]];
				u_values(i) = m_u[element[i]];
			}

			for (size_t i = 0; i < m_gauss_points.size(); ++i)
			{
				double s = m_gauss_points[i][0], t = m_gauss_points[i][1];
				double w = m_weights[i];
				Eigen::VectorXd N;
				Eigen::MatrixXd dN_ds;

				N = mp_mesh->get_N(s, t);
				dN_ds = mp_mesh->get_dN_ds(s, t);

				Eigen::MatrixXd J = dN_ds.transpose() * node_coords;
				double detJ = J.determinant();
				Eigen::MatrixXd invJ = J.inverse();
				Eigen::MatrixXd grad_N = invJ * dN_ds.transpose();

				double u_local = N.dot(u_values);

				Eigen::MatrixXd grad_grad_term = -grad_N.transpose() * grad_N;
				double df_du_u = m_df_du(u_local);
				Eigen::MatrixXd non_linear_term = N * N.transpose() * df_du_u;

				Eigen::VectorXd grad_u = grad_N * u_values;
				Eigen::VectorXd residual_term = grad_N.transpose() * grad_u + N * m_f(u_local);

				local_K += (grad_grad_term - non_linear_term) * detJ * w;
				local_F += residual_term * detJ * w;
			}

			for (int i = 0; i < num_nodes; ++i)
			{
				int global_i = element[i];
				F(global_i) += local_F(i);
				for (int j = 0; j < num_nodes; ++j)
				{
					int global_j = element[j];
#ifdef USE_SPARSE_MATRIX
					K.coeffRef(global_i, global_j) += local_K(i, j);
#else
					K(global_i, global_j) += local_K(i, j);
#endif
				}
			}
		}
		return std::make_pair(K, F);
	}


#ifdef USE_SPARSE_MATRIX
	void FEMProblem::applyDirichletBoundaryConditions(Eigen::SparseMatrix<double>& K, Eigen::VectorXd& F)
#else
	void FEMProblem::applyDirichletBoundaryConditions(Eigen::MatrixXd& K, Eigen::VectorXd& F)
#endif
	{
		for (const auto& [node, value] : m_boundary_conditions)
		{
#ifdef USE_SPARSE_MATRIX
			for (int k = 0; k < K.outerSize(); ++k)
			{
				for (Eigen::SparseMatrix<double>::InnerIterator it(K, k); it; ++it)
				{
					if (it.col() == node && it.row() != node)
					{
						K.coeffRef(it.row(), it.col()) = 0.0;
					}
					if (it.row() == node && it.col() != node)
					{
						K.coeffRef(it.row(), it.col()) = 0.0;
					}
				}
			}
			K.coeffRef(node, node) = 1.0;
#else
			K.row(node).setZero();
			K.col(node).setZero();
			K(node, node) = 1.0;
#endif
			F(node) = value;
			m_u(node) = value;
		}
	}

	bool FEMProblem::solve(double rtol, double atol, int max_iter, Eigen::VectorXd& u_sol, const std::string& output)
	{
		std::ofstream log_file(output);
		log_file << std::scientific << std::setprecision(8);
		log_file << "-----------------------------------------------------------------\n";
		log_file << "Step\t\tAbs. Error\t\tRel. Error\n";
		log_file << "-----------------------------------------------------------------\n";

		std::cout << std::scientific << std::setprecision(8) << std::endl;
		std::cout << "-----------------------------------------------------------------" << std::endl;
		std::cout << "Step\t\tAbs. Error\t\tRel. Error" << std::endl;
		std::cout << "-----------------------------------------------------------------" << std::endl;

		for (int iteration = 0; iteration < max_iter; ++iteration)
		{
			auto [K, F] = assembleGlobalMatricesAndVector();
			applyDirichletBoundaryConditions(K, F);

#ifdef USE_SPARSE_MATRIX
			Eigen::ConjugateGradient<Eigen::SparseMatrix<double>, Eigen::Lower | Eigen::Upper> solver;
			solver.compute(K);
			Eigen::VectorXd du = solver.solve(F);
#else
			Eigen::ConjugateGradient<Eigen::MatrixXd> solver;
			solver.compute(K);
			Eigen::VectorXd du = solver.solve(F);
#endif

			for (const auto& [node, _] : m_boundary_conditions)
			{
				du(node) = 0.0;
				F(node) = 0.0;
			}

			m_u += du;

			double abs_error = du.norm() / m_u.norm();
			double rel_error = F.norm();
			if (iteration == 0)
			{
				log_file << iteration + 1 << "\t\t" << abs_error << "\t\t-\n";
				std::cout << iteration + 1 << "\t\t" << abs_error << "\t\t-" << std::endl;
			}
			else
			{
				log_file << iteration + 1 << "\t\t" << abs_error << "\t\t" << rel_error << "\n";
				std::cout << iteration + 1 << "\t\t" << abs_error << "\t\t" << rel_error << std::endl;
			}

			if (abs_error < rtol && rel_error < atol)
			{
				log_file << "-----------------------------------------------------------------\n";
				std::cout << "-----------------------------------------------------------------" << std::endl;
				u_sol = m_u;
				return true;
			}
			if (iteration == max_iter - 1)
			{
				return false;
			}
		}
	}
} // namespace CPPPOISSON

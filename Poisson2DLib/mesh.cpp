#include "mesh.h"
#include<iostream>

namespace CPPPOISSON
{
	Mesh::Mesh(double lx, double ly, int nx, int ny)
		:m_lx(lx), m_ly(ly), m_nx(nx), m_ny(ny)
	{
		Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(m_nx, 0, m_lx);
		Eigen::VectorXd y = Eigen::VectorXd::LinSpaced(m_ny, 0, m_ly);
		for (int j = 0; j < m_ny; ++j)
		{
			for (int i = 0; i < m_nx; ++i)
			{
				m_points.emplace_back(x[i], y[j]);
			}
		}
	}

	Mesh::~Mesh() {}

	std::vector<int> Mesh::getBoundaryNodes(const std::string& boundary) const
	{
		constexpr double tolerance = 1e-8;
		std::vector<int> nodes;
		if (boundary == "left")
		{
			int ind = 0;
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				if (std::fabs(m_points[i][0] - 0.) < tolerance)
				{
					nodes.push_back(ind);
				}
				++ind;
			}
		}
		else if (boundary == "right")
		{
			int ind = 0;
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				if (std::fabs(m_points[i][0] - m_lx) < tolerance)
				{
					nodes.push_back(ind);
				}
				++ind;
			}
		}
		else if (boundary == "bottom")
		{
			int ind = 0;
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				if (std::fabs(m_points[i][1] - 0.) < tolerance)
				{
					nodes.push_back(ind);
				}
				++ind;
			}
		}
		else if (boundary == "top")
		{
			int ind = 0;
			for (size_t i = 0; i < m_points.size(); ++i)
			{
				if (std::fabs(m_points[i][1] - m_ly) < tolerance)
				{
					nodes.push_back(ind);
				}
				++ind;
			}
		}
		else
		{
			std::cout << "Wrong boundary name <" << boundary << "> !!! (Boundary name should be left/right/bottom/top)" << std::endl;
		}
		return nodes;
	}

	TriangularMesh::TriangularMesh(double lx, double ly, int nx, int ny)
		:Mesh(lx, ly, nx, ny)
	{
		m_elements = generateElement(m_nx, m_ny, m_points);
	}

	std::vector<std::vector<int>> TriangularMesh::generateElement(int nx, int ny, const std::vector<Eigen::Vector2d>& points) const
	{
		std::vector<std::vector<int>> elements;
		for (int i = 0; i < ny - 1; ++i)
		{
			for (int j = 0; j < nx - 1; ++j)
			{
				int n1 = i * nx + j;
				int n2 = i * nx + (j + 1);
				int n3 = (i + 1) * nx + (j + 1);
				int n4 = (i + 1) * nx + j;
				std::vector<int> ele1 = { n1,n2,n4 };
				std::vector<int> ele2 = { n2,n3,n4 };
				elements.push_back(ele1);
				elements.push_back(ele2);
			}
		}
		return elements;
	}

	Eigen::VectorXd TriangularMesh::get_N(double s, double t) const
	{
		Eigen::VectorXd N(3);
		N <<
			1 - s - t,
			s,
			t;
		return N;
	}

	Eigen::MatrixXd TriangularMesh::get_dN_ds(double s, double t) const
	{
		Eigen::MatrixXd dN_ds(3, 2);
		dN_ds <<
			-1, -1,
			1, 0,
			0, 1;
		return dN_ds;
	}

	RectangularMesh::RectangularMesh(double lx, double ly, int nx, int ny)
		:Mesh(lx, ly, nx, ny)
	{
		m_elements = generateElement(m_nx, m_ny, m_points);
	}

	std::vector<std::vector<int>> RectangularMesh::generateElement(int nx, int ny, const std::vector<Eigen::Vector2d>& points) const
	{
		std::vector<std::vector<int>> elements;
		for (int i = 0; i < ny - 1; ++i)
		{
			for (int j = 0; j < nx - 1; ++j)
			{
				int n1 = i * nx + j;
				int n2 = i * nx + (j + 1);
				int n3 = (i + 1) * nx + (j + 1);
				int n4 = (i + 1) * nx + j;
				std::vector<int> ele = { n1,n2,n3,n4 };
				elements.push_back(ele);
			}
		}
		return elements;
	}

	Eigen::VectorXd RectangularMesh::get_N(double s, double t) const
	{
		Eigen::VectorXd N(4);
		N <<
			0.25 * (1 - s) * (1 - t),
			0.25 * (1 + s) * (1 - t),
			0.25 * (1 + s) * (1 + t),
			0.25 * (1 - s) * (1 + t);
		return N;
	}

	Eigen::MatrixXd RectangularMesh::get_dN_ds(double s, double t) const
	{
		Eigen::MatrixXd dN_ds(4, 2);
		dN_ds <<
			-0.25 * (1 - t), -0.25 * (1 - s),
			0.25 * (1 - t), -0.25 * (1 + s),
			0.25 * (1 + t), 0.25 * (1 + s),
			-0.25 * (1 + t), 0.25 * (1 - s);
		return dN_ds;
	}
}   // namespace CPPPOISSON

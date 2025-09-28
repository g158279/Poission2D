#include "mesh.h"
#include <iostream>
#include <algorithm>

namespace CPPPOISSON
{
	Mesh::Mesh(const PoissonDef& poissonDef)
	{
		int nx{ poissonDef.nx }, ny{ poissonDef.ny };
		double lx{ poissonDef.boundary.xRight }, ly{ poissonDef.boundary.yTop };
		double xStep{ lx / (nx - 1) };
		double yStep{ ly / (ny - 1) };
		m_points.resize(nx * ny);
		for (int i = 0; i < ny; ++i)
		{
			double y = i * yStep;
			for (int j = 0; j < nx; ++j)
			{
				double x = j * xStep;
				size_t index = i * nx + j;
				m_points.at(index) = std::make_unique<Point>(x, y, index);
			}
		}
		std::string meshShape = poissonDef.shape;
		std::transform(meshShape.begin(), meshShape.end(), meshShape.begin(), ::tolower);
		if (meshShape == "triangular" || meshShape == "tri" || meshShape == "t")
		{
			std::cout << "Calculating based on triangular mesh" << std::endl;
			m_eleType = EleType::Triangle;
			generateTriangleMesh(nx, ny);
		}
		else if (meshShape == "rectangular" || meshShape == "rect" || meshShape == "r")
		{
			std::cout << "Calculating based on rectangular mesh" << std::endl;
			m_eleType = EleType::Rectangle;
			generateRectangleMesh(nx, ny);
		}
		else
		{
			throw std::invalid_argument("Wrong mesh shape!");
		}
	}

	void Mesh::generateTriangleMesh(int nx, int ny)
	{
		for (int i = 0; i < ny - 1; ++i)
		{
			for (int j = 0; j < nx - 1; ++j)
			{
				int n1 = i * nx + j;
				int n2 = i * nx + (j + 1);
				int n3 = (i + 1) * nx + (j + 1);
				int n4 = (i + 1) * nx + j;
				std::vector<Point*> ele1 = { m_points.at(n1).get(),m_points.at(n2).get(),m_points.at(n4).get() };
				std::vector<Point*> ele2 = { m_points.at(n2).get(),m_points.at(n3).get(),m_points.at(n4).get() };
				m_elements.emplace_back(std::make_unique<TriangularElement>(ele1));
				m_elements.emplace_back(std::make_unique<TriangularElement>(ele2));
			}
		}
	}

	void Mesh::generateRectangleMesh(int nx, int ny)
	{
		for (int i = 0; i < ny - 1; ++i)
		{
			for (int j = 0; j < nx - 1; ++j)
			{
				int n1 = i * nx + j;
				int n2 = i * nx + (j + 1);
				int n3 = (i + 1) * nx + (j + 1);
				int n4 = (i + 1) * nx + j;
				std::vector<Point*> ele = { m_points.at(n1).get(),m_points.at(n2).get(),m_points.at(n3).get(),m_points.at(n4).get() };
				m_elements.emplace_back(std::make_unique<RectangularElement>(ele));
			}
		}
	}

	Element::Element(const std::vector<Point*>& nodes) :mp_nodes{ nodes } {}

	Eigen::VectorXd TriangularElement::get_N(double s, double t) const
	{
		Eigen::VectorXd N(3);
		N <<
			1 - s - t,
			s,
			t;
		return N;
	}

	Eigen::MatrixXd TriangularElement::get_dN_ds(double s, double t) const
	{
		Eigen::MatrixXd dN_ds(3, 2);
		dN_ds <<
			-1, -1,
			1, 0,
			0, 1;
		return dN_ds;
	}

	Eigen::VectorXd RectangularElement::get_N(double s, double t) const
	{
		Eigen::VectorXd N(4);
		N <<
			0.25 * (1 - s) * (1 - t),
			0.25 * (1 + s) * (1 - t),
			0.25 * (1 + s) * (1 + t),
			0.25 * (1 - s) * (1 + t);
		return N;
	}

	Eigen::MatrixXd RectangularElement::get_dN_ds(double s, double t) const
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

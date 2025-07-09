#ifndef MESH_H
#define MESH_H

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

#include <vector>
#include <Eigen/Dense>
#include <string>

namespace CPPPOISSON
{
	class MYLIBRARY_API Mesh
	{
	public:
		Mesh(double lx, double ly, int nx, int ny);
		virtual ~Mesh();

		std::vector<Eigen::Vector2d> m_points;
		std::vector<std::vector<int>> m_elements;
		std::vector<int> getBoundaryNodes(const std::string& boundary) const;

		virtual std::vector<std::vector<int>> generateElement(int nx, int ny, const std::vector<Eigen::Vector2d>& points) const = 0;
		virtual Eigen::VectorXd get_N(double s, double t) const = 0;
		virtual Eigen::MatrixXd get_dN_ds(double s, double t) const = 0;
	protected:
		double m_lx, m_ly;
		int m_nx, m_ny;
	};

	class MYLIBRARY_API TriangularMesh : public Mesh
	{
	public:
		TriangularMesh(double lx, double ly, int nx, int ny);
		std::vector<std::vector<int>> generateElement(int nx, int ny, const std::vector<Eigen::Vector2d>& points) const override;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};

	class MYLIBRARY_API RectangularMesh : public Mesh
	{
	public:
		RectangularMesh(double lx, double ly, int nx, int ny);
		std::vector<std::vector<int>> generateElement(int nx, int ny, const std::vector<Eigen::Vector2d>& points) const override;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};
}   // namespace CPPPOISSON

#endif // MESH_H

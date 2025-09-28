#pragma once

#ifdef POISSON_EXPORTS
#define MESH_API __declspec(dllexport)
#else
#define MESH_API __declspec(dllimport)
#endif

#include <vector>
#include <Eigen/Dense>
#include <string>
#include "preprocess.h"

namespace CPPPOISSON
{
	class Point {
	public:
		explicit Point(double x, double y, size_t index) :m_x(x), m_y(y), m_index(index) {}

		const double x() const { return m_x; }
		const double y() const { return m_y; }
		const size_t index() const { return m_index; }
	private:
		double m_x;
		double m_y;
		size_t m_index;
	};

	enum class EleType
	{
		Triangle,
		Rectangle,
	};

	class Element {
	public:
		Element(const std::vector<Point*>& nodes) {};
		virtual ~Element() = default;
		virtual Eigen::VectorXd get_N(double s, double t) const = 0;
		virtual Eigen::MatrixXd get_dN_ds(double s, double t) const = 0;
	private:
		std::vector<Point*> mp_nodes;
	};

	class TriangularElement : public Element
	{
	public:
		TriangularElement(const std::vector<Point*>& nodes);
		virtual ~TriangularElement() = default;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};

	class RectangularElement : public Element
	{
	public:
		RectangularElement(const std::vector<Point*>& nodes);
		virtual ~RectangularElement() = default;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};

	class MESH_API Mesh
	{
	public:
		Mesh(const PoissonDef& poissonDef);
		~Mesh() = default;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(Mesh&&) = delete;

	private:
		void generateTriangleMesh(int nx, int ny);
		void generateRectangleMesh(int nx, int ny);
		std::vector<std::unique_ptr<Point>> m_points;
		std::vector<std::unique_ptr<Element>> m_elements;
		EleType m_eleType;
	};

}   // namespace CPPPOISSON

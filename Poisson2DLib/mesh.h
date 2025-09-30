#pragma once

#ifdef POISSON_EXPORTS
#define MESH_API __declspec(dllexport)
#else
#define MESH_API __declspec(dllimport)
#endif

#include <vector>
#include <Eigen/Dense>
#include <string>
#include "geometry.h"
#include "preprocess.h"

namespace CPPPOISSON
{

	enum class EleType
	{
		Triangle,
		Rectangle,
	};

	class Element {
	public:
		explicit Element(const std::vector<Point*>& nodes);
		virtual ~Element() = default;
		virtual Eigen::VectorXd get_N(double s, double t) const = 0;
		virtual Eigen::MatrixXd get_dN_ds(double s, double t) const = 0;
	private:
		std::vector<Point*> mp_nodes;
	};

	class TriangularElement : public Element
	{
	public:
		explicit TriangularElement(const std::vector<Point*>& nodes) :Element(nodes) {};
		virtual ~TriangularElement() = default;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};

	class RectangularElement : public Element
	{
	public:
		explicit RectangularElement(const std::vector<Point*>& nodes) :Element(nodes) {};
		virtual ~RectangularElement() = default;
		Eigen::VectorXd get_N(double s, double t) const override;
		Eigen::MatrixXd get_dN_ds(double s, double t) const override;
	};

	class MESH_API Mesh
	{
	public:
		explicit Mesh(const PoissonDef& poissonDef);
		~Mesh() = default;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(Mesh&&) = delete;

		const std::vector<std::unique_ptr<Element>>& getAllEles() const { return m_elements; }
		const std::unique_ptr<Element>& getEle(size_t index) const { return m_elements.at(index); }
		const size_t getEleSize() const { return m_elements.size(); }
		const std::vector<std::unique_ptr<Point>>& getAllPoints() const { return m_points; }
		const Point& getPoint(size_t index) const { return *m_points.at(index); }
		const size_t getPointSize() const { return m_points.size(); }
	private:
		void generateTriangleMesh(int nx, int ny);
		void generateRectangleMesh(int nx, int ny);
		std::vector<std::unique_ptr<Point>> m_points;
		std::vector<std::unique_ptr<Element>> m_elements;
		EleType m_eleType;
	};

}   // namespace CPPPOISSON

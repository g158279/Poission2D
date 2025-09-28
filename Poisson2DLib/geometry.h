#pragma once

namespace CPPPOISSON{
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

	enum class BoundaryType {
		Left,
		Right,
		Top,
		Bottom,
	};

	struct Boundary {
		bool isOnBoundary(const Point& point, BoundaryType boundaryType) const;
		double xLeft{ 0.0 };
		double xRight{ 1.0 };
		double yBottom{ 0.0 };
		double yTop{ 1.0 };
	private:
		double tol{ 1e-8 };
	};

}
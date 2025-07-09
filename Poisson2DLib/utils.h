#ifndef UTILS_H
#define UTILS_H

#ifdef MYLIBRARY_EXPORTS
#define MYLIBRARY_API __declspec(dllexport)
#else
#define MYLIBRARY_API __declspec(dllimport)
#endif

#include <vector>
#include <boost/math/quadrature/gauss.hpp>
#include <boost/math/tools/roots.hpp>
#include <Eigen/Dense>

namespace CPPPOISSON
{
MYLIBRARY_API void gauss_quadrature_2d(size_t n, std::vector<Eigen::Vector2d>& points, std::vector<double>& weights);
MYLIBRARY_API Eigen::MatrixXd analytic_solution(const double lx, const double ly, const int nx, const int ny, size_t N=100);
}   // namespace CPPPOISSON

#endif // UTILS_H

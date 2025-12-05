#include "utils.h"
#include <boost/math/quadrature/gauss.hpp>
#include <boost/math/tools/roots.hpp>
#include <Eigen/Dense>

namespace CPPPOISSON
{
	void generateRectangleInt(size_t n, std::vector<Eigen::Vector3d>& gaussInts) {

		std::vector<double> points_1d;
		std::vector<double> weights_1d;
		constexpr double PI = 3.14159265358979323846;

		for (size_t k = 0; k < n; ++k) {
			double guess = std::cos(PI * (k + 0.75) / (n + 0.5)); // 初始猜测点基于Chebyshev近似
			auto legendre_n = [n](double x) { return boost::math::legendre_p(n, x); };

			boost::uintmax_t max_iter = 50;
			double root = boost::math::tools::newton_raphson_iterate(
				[n](double x) {
					return std::make_pair(
						boost::math::legendre_p(n, x),
						boost::math::legendre_p_prime(n, x)); // legendre_p的导数
				},
				guess, -1.0, 1.0, std::numeric_limits<double>::digits, max_iter
			);
			points_1d.push_back(root);
		}

		for (auto x : points_1d) {
			// w_i = 2 / ((1 - x_i^2) * [P'_n(x_i)]^2)
			double weight = 2.0 / ((1 - x * x) * std::pow(boost::math::legendre_p_prime(n, x), 2));
			weights_1d.push_back(weight);
		}

		for (size_t i = 0; i < n; ++i) {
			double s = points_1d[i];
			double w_s = weights_1d[i];
			for (size_t j = 0; j < n; ++j) {
				double t = points_1d[j];
				double w_t = weights_1d[j];
				gaussInts.emplace_back(Eigen::Vector3d(s, t, w));
			}
		}
	}

	void generateTriangleInt(size_t n, std::vector<Eigen::Vector3d>& gaussInts) {
		std::vector<double> xPoints(n);
		std::vector<double> yPoints(n);
		std::vector<double> weights(n);
		if (n <= 1) {
			xPoints = { 1.0 / 3 };
			yPoints = { 1.0 / 3 };
			weights = { 1.0 / 2 };
		}
		else if (n <= 3) {
			xPoints = { 1.0 / 6, 2.0 / 3, 1.0 / 6 };
			yPoints = { 1.0 / 6, 1.0 / 6, 2.0 / 3 };
			weights = { 1.0 / 6, 1.0 / 6, 1.0 / 6 };
		}
		else if (n <= 4) {
			xPoints = { 1.0 / 3, 3.0 / 5, 1.0 / 5, 1.0 / 5 };
			yPoints = { 1.0 / 3, 1.0 / 5, 3.0 / 5, 1.0 / 5 };
			weights = { -9.0 / 32, 25.0 / 96, 25.0 / 96, 25.0 / 96 };
		}
		else if (n <= 7) {
			xPoints = { 0, 1.0 / 2, 1, 1.0 / 2, 0, 0, 1.0 / 3 };
			yPoints = { 0, 0, 0, 1.0 / 2, 1, 1.0 / 2, 1.0 / 3 };
			weights = { 1.0 / 40, 1.0 / 15, 1.0 / 40, 1.0 / 15, 1.0 / 40, 1.0 / 15, 9.0 / 40 };
		}
		else {
			throw std::invalid_argument("Only n <= 7 are supported for reference triangle element");
		}
		for (size_t i = 0; i < n; ++i) {
			gaussInts.emplace_back(Eigen::Vector3d(xPoints[i], xPoints[i], xPoints[i]));
		}
	}

	Eigen::MatrixXd analytic_solution(const double lx, const double ly, const int nx, const int ny, size_t N)
	{
		constexpr double PI = 3.14159265358979323846;
		const double u_hat = 1.;
		double coeff = 4. * u_hat / PI;

		Eigen::MatrixXd u_anal(nx, ny);
		u_anal.setZero();
		for (size_t n = 0; n < N; ++n) {
			if (n % 2 == 1) {
				size_t ix = 0;
				for (double x : Eigen::VectorXd::LinSpaced(nx, 0., lx)) {
					size_t iy = 0;
					for (double y : Eigen::VectorXd::LinSpaced(ny, 0., ly)) {
						u_anal(ix, iy) += coeff * std::sin(n * PI * x / lx) * std::sinh(n * PI * y / lx) / n / std::sinh(n * PI * ly / lx);
						++iy;
					}
					++ix;
				}
			}
		}
		return u_anal;
	}
} // namespace CPPPOISSON

//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_UNUSED_H
#define IDEA_UNUSED_H


#if 0
class BoxGenerator {
	std::default_random_engine& engine;
	double x, y;
	std::uniform_real_distribution<double> x_dist;
	std::uniform_real_distribution<double> y_dist;

	BoxGenerator(
			std::default_random_engine& engine,
			const double x, const double y,
			const double w, const double h,
			const double r
	)
			: engine{engine}
			, x{x}
			, y{y}
			, x_dist{r, w - r}
			, y_dist{r, h - r}
	{}

	BoxGenerator(
			std::default_random_engine& engine,
			const double w, const double h, const double r
	) : BoxGenerator{engine, 0, 0, w, h, r} {}

	BoxGenerator(
			std::default_random_engine& engine,
			const config::TableDimensions& dims, const double r
	) : BoxGenerator{engine, dims.width, dims.height, r} {}

	geometry::Point gen() {
		return geometry::Point{x + x_dist(), y + y_dist()};
	}
};
#endif

#endif //IDEA_UNUSED_H

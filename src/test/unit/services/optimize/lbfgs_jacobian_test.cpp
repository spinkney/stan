#include <stan/services/optimize/lbfgs.hpp>
#include <gtest/gtest.h>
#include <stan/io/empty_var_context.hpp>
#include <test/test-models/good/optimization/simple_jacobian.hpp>
#include <test/unit/services/instrumented_callbacks.hpp>
#include <stan/callbacks/stream_writer.hpp>
#include <cmath>

TEST_F(ServicesOptimize, with_jacobian) {
  unsigned int seed = 0;
  unsigned int chain = 1;
  double init_radius = 0;

  bool save_iterations = true;
  int refresh = 0;
  stan::test::unit::instrumented_interrupt interrupt;

  int return_code = stan::services::optimize::lbfgs<stan_model, true>(
      model, context, seed, chain, init_radius, 5, 0.001, 1e-12, 10000, 1e-8,
      10000000, 1e-8, 2000, save_iterations, refresh, interrupt, logger, init,
      parameter);

  EXPECT_TRUE(logger.find("Optimization terminated normally: "));
  EXPECT_FLOAT_EQ(return_code, 0);

  ASSERT_EQ(2, parameter.names_.size());
  EXPECT_EQ("lp__", parameter.names_[0]);
  EXPECT_EQ("sigma", parameter.names_[1]);
  EXPECT_NEAR((3 + std::sqrt(13)) / 2, parameter.states_.back()[1], 0.0001);
}

TEST_F(ServicesOptimize, without_jacobian) {
  unsigned int seed = 0;
  unsigned int chain = 1;
  double init_radius = 0;

  bool save_iterations = true;
  int refresh = 0;
  stan::test::unit::instrumented_interrupt interrupt;

  int return_code = stan::services::optimize::lbfgs<stan_model, false>(
      model, context, seed, chain, init_radius, 5, 0.001, 1e-12, 10000, 1e-8,
      10000000, 1e-8, 2000, save_iterations, refresh, interrupt, logger, init,
      parameter);

  EXPECT_TRUE(logger.find("Optimization terminated normally: "));
  EXPECT_FLOAT_EQ(return_code, 0);

  ASSERT_EQ(2, parameter.names_.size());
  EXPECT_EQ("lp__", parameter.names_[0]);
  EXPECT_EQ("sigma", parameter.names_[1]);
  EXPECT_NEAR(3, parameter.states_.back()[1], 0.0001);
}

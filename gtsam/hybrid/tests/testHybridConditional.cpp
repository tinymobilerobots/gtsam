/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/*
 *  @file testHybridConditional.cpp
 *  @date Mar 11, 2022
 *  @author Fan Jiang
 */

#include <gtsam/hybrid/HybridFactor.h>
#include <gtsam/hybrid/HybridConditional.h>
#include <gtsam/hybrid/HybridFactorGraph.h>
#include <gtsam/hybrid/HybridGaussianFactor.h>
#include <gtsam/hybrid/HybridDiscreteFactor.h>
#include <gtsam/hybrid/CGMixtureFactor.h>
#include <gtsam/hybrid/HybridBayesNet.h>
#include <gtsam/hybrid/HybridBayesTree.h>

#include <gtsam/linear/JacobianFactor.h>
#include <gtsam/inference/BayesNet.h>

#include <gtsam/inference/Symbol.h>

#include <CppUnitLite/TestHarness.h>

#include <boost/assign/std/map.hpp>
using namespace boost::assign;

using namespace std;
using namespace gtsam;

using gtsam::symbol_shorthand::X;
using gtsam::symbol_shorthand::C;

/* ************************************************************************* */
TEST_UNSAFE(HybridFactorGraph, creation) {
  HybridConditional test;

  HybridFactorGraph hfg;

  hfg.add(HybridGaussianFactor(JacobianFactor(0, I_3x3, Z_3x1)));
}

TEST_UNSAFE(HybridFactorGraph, eliminate) {
  HybridFactorGraph hfg;

  hfg.add(HybridGaussianFactor(JacobianFactor(0, I_3x3, Z_3x1)));

  auto result = hfg.eliminatePartialSequential({0});

  EXPECT_LONGS_EQUAL(result.first->size(), 1);
}

TEST(HybridFactorGraph, eliminateMultifrontal) {
  HybridFactorGraph hfg;

  DiscreteKey x(X(1), 2);

  hfg.add(JacobianFactor(X(0), I_3x3, Z_3x1));
  hfg.add(HybridDiscreteFactor(DecisionTreeFactor(x, {2, 8})));

  auto result = hfg.eliminatePartialMultifrontal({X(0)});

  EXPECT_LONGS_EQUAL(result.first->size(), 1);
  EXPECT_LONGS_EQUAL(result.second->size(), 1);
}

TEST(HybridFactorGraph, eliminateFullMultifrontal) {

  std::cout << ">>>>>>>>>>>>>>\n";

  HybridFactorGraph hfg;

  DiscreteKey x(C(1), 2);

  hfg.add(JacobianFactor(X(0), I_3x3, Z_3x1));
  hfg.add(JacobianFactor(X(0), I_3x3, X(1), -I_3x3, Z_3x1));

  DecisionTree<Key, GaussianFactor::shared_ptr> dt;

  hfg.add(CGMixtureFactor({X(1)}, { x }, dt));
  hfg.add(HybridDiscreteFactor(DecisionTreeFactor(x, {2, 8})));

  auto result = hfg.eliminateMultifrontal(Ordering::ColamdConstrainedLast(hfg, {C(1)}));

  GTSAM_PRINT(*result);
  GTSAM_PRINT(*result->marginalFactor(C(1)));
}

/* ************************************************************************* */
int main() {
  TestResult tr;
  return TestRegistry::runAllTests(tr);
}
/* ************************************************************************* */


/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation,
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/*
 * @file PCGSolver.h
 * @brief Preconditioned Conjugate Gradient Solver for linear systems
 * @date Jan 31, 2012
 * @author Yong-Dian Jian
 * @author Sungtae An
 */

#pragma once

#include <gtsam/linear/ConjugateGradientSolver.h>
#include <gtsam/linear/GaussianFactorGraph.h>

#include <string>

namespace gtsam {

class KeyInfo;
class Preconditioner;
class VectorValues;
struct PreconditionerParameters;

/**
 * Parameters for Preconditioned Conjugate Gradient solver.
 */
struct GTSAM_EXPORT PCGSolverParameters : public ConjugateGradientParameters {
  typedef ConjugateGradientParameters Base;
  typedef std::shared_ptr<PCGSolverParameters> shared_ptr;

  std::shared_ptr<PreconditionerParameters> preconditioner;

  PCGSolverParameters() {}

  PCGSolverParameters(
      const std::shared_ptr<PreconditionerParameters> &preconditioner)
      : preconditioner(preconditioner) {}

  void print(std::ostream &os) const override;
  void print(const std::string &s) const;
};

/**
 * A virtual base class for the preconditioned conjugate gradient solver
 */
class GTSAM_EXPORT PCGSolver: public IterativeSolver {
public:
  typedef IterativeSolver Base;
  typedef std::shared_ptr<PCGSolver> shared_ptr;

protected:

  PCGSolverParameters parameters_;
  std::shared_ptr<Preconditioner> preconditioner_;

public:
  /* Interface to initialize a solver without a problem */
  PCGSolver(const PCGSolverParameters &p);
  ~PCGSolver() override {
  }

  using IterativeSolver::optimize;

  VectorValues optimize(const GaussianFactorGraph &gfg,
      const KeyInfo &keyInfo, const std::map<Key, Vector> &lambda,
      const VectorValues &initial) override;

};

/**
 * System class needed for calling preconditionedConjugateGradient
 */
class GTSAM_EXPORT GaussianFactorGraphSystem {
  const GaussianFactorGraph &gfg_;
  const Preconditioner &preconditioner_;
  KeyInfo keyInfo_;
  std::map<Key, Vector> lambda_;

 public:
  GaussianFactorGraphSystem(const GaussianFactorGraph &gfg,
                            const Preconditioner &preconditioner,
                            const KeyInfo &info,
                            const std::map<Key, Vector> &lambda);

  Vector residual(const Vector &x) const;
  Vector multiply(const Vector &x) const;
  Vector leftPrecondition(const Vector &x, Vector &y) const;
  Vector rightPrecondition(const Vector &x, Vector &y) const;
  Vector scal(const double alpha, const Vector &x) const;
  double dot(const Vector &x, const Vector &y) const;
  Vector axpy(const double alpha, const Vector &x, const Vector &y) const;

  Vector getb() const;
};

/// @name utility functions
/// @{

/// Create VectorValues from a Vector
VectorValues buildVectorValues(const Vector &v, const Ordering &ordering,
    const std::map<Key, size_t> & dimensions);

/// Create VectorValues from a Vector and a KeyInfo class
VectorValues buildVectorValues(const Vector &v, const KeyInfo &keyInfo);

/// @}

}


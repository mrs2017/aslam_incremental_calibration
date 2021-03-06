/******************************************************************************
 * Copyright (C) 2015 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include "aslam/calibration/egomotion/algo/splinesToFile.h"

#include <algorithm>

#include <sm/kinematics/quaternion_algebra.hpp>
#include <sm/kinematics/EulerAnglesYawPitchRoll.hpp>

#include <aslam/calibration/core/IncrementalEstimator.h>
#include <aslam/calibration/core/IncrementalOptimizationProblem.h>

#include <bsplines/NsecTimePolicy.hpp>

#include "aslam/calibration/egomotion/algo/OptimizationProblemSpline.h"

using namespace sm::timing;
using namespace sm::kinematics;

namespace aslam {
  namespace calibration {

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

    void writeSplines(const IncrementalEstimatorSP& estimator, double dt,
        std::ofstream& stream) {
      auto batches = estimator->getProblem()->getOptimizationProblems();
      for (const auto& batch : batches) {
        auto transSplines = dynamic_cast<const OptimizationProblemSpline*>(
          batch.get())->getTranslationSplines();
        auto rotSplines = dynamic_cast<const OptimizationProblemSpline*>(
          batch.get())->getRotationSplines();
        for (auto itSplines = transSplines.cbegin(); itSplines !=
            transSplines.cend(); ++itSplines) {
          writeSplines(*itSplines, rotSplines.at(std::distance(
            transSplines.cbegin(), itSplines)), dt, stream);
        }
      }
    }

    void writeSplines(const TranslationSplineSP& transSpline, const
        RotationSplineSP& rotSpline, double dt, std::ofstream& stream) {
      auto t = transSpline->getMinTime();
      auto T = transSpline->getMaxTime();
      const EulerAnglesYawPitchRoll ypr;
      while (t < T) {
        auto translationExpressionFactory =
          transSpline->getExpressionFactoryAt<0>(t);
        stream << translationExpressionFactory.getValueExpression().toValue().
            transpose() << " ";
        auto rotationExpressionFactory =
          rotSpline->getExpressionFactoryAt<0>(t);
        stream << ypr.rotationMatrixToParameters(quat2r(
          rotationExpressionFactory.getValueExpression().toValue())).
            transpose() << std::endl;
        t += secToNsec(dt);
      }
    }

    void writeSplines(const TranslationSplineSSP& transSpline, const
        RotationSplineSSP& rotSpline, double dt, std::ofstream& stream) {
      auto t = transSpline->getMinTime();
      auto T = transSpline->getMaxTime();
      const EulerAnglesYawPitchRoll ypr;
      while (t < T) {
        auto translationEvaluator = transSpline->getEvaluatorAt<0>(t);
        stream << translationEvaluator.eval().transpose() << " ";
        auto rotationEvaluator = rotSpline->getEvaluatorAt<0>(t);
        stream << ypr.rotationMatrixToParameters(quat2r(
          rotationEvaluator.eval())).transpose() << std::endl;
        t += secToNsec(dt);
      }
    }

  }
}

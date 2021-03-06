/******************************************************************************
 * Copyright (C) 2014 by Jerome Maye                                          *
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

/** \file PoseMeasurement.h
    \brief This file defines the PoseMeasurement structure which represents a
           measurement of a pose sensor.
  */

#ifndef ASLAM_CALIBRATION_TIME_DELAY_POSE_MEASUREMENT_H
#define ASLAM_CALIBRATION_TIME_DELAY_POSE_MEASUREMENT_H

#include <Eigen/Core>

namespace aslam {
  namespace calibration {

    /** The structure PoseMeasurement represents a measurement returned by a
        pose sensor such as Applanix.
        \brief Pose measurement
      */
    struct PoseMeasurement {
      /// \cond
      // Required by Eigen for fixed-size matrices members
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      /// \endcond

      /** \name Public members
        @{
        */
      /// Relative position of the reference frame w.r. to the world frame
      Eigen::Vector3d w_r_wp;
      /// Covariance matrix for w_r_wp
      Eigen::Matrix3d sigma2_w_r_wp;
      /// Relative orientation of the reference frame w.r. to the world frame
      Eigen::Vector3d w_R_p; // [yaw, pitch, roll]
      /// Covariance matrix for m_R_r
      Eigen::Matrix3d sigma2_w_R_p;
      /** @}
        */

    };

  }
}

#endif // ASLAM_CALIBRATION_TIME_DELAY_POSE_MEASUREMENT_H

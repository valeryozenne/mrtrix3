/*
 * Copyright (c) 2008-2016 the MRtrix3 contributors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/
 *
 * MRtrix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * For more details, see www.mrtrix.org
 *
 */

#ifndef __registration_transform_initialiser_h__
#define __registration_transform_initialiser_h__

#include "image.h"
#include "transform.h"
#include "registration/transform/base.h"

#include "registration/transform/initialiser_helpers.h"

namespace MR
{
  namespace Registration
  {
    namespace Transform
    {
      namespace Init
      {
        enum InitType {mass, geometric, moments, mass_unmasked, moments_use_mask_intensity, moments_unmasked, fod, set_centre_mass, none};

          void set_centre_using_image_mass (Image<default_type>& im1,
                                            Image<default_type>& im2,
                                            Image<default_type>& mask1,
                                            Image<default_type>& mask2,
                                            Registration::Transform::Base& transform) {
            CONSOLE ("initialising centre of rotation using centre of mass");
            Eigen::Vector3 im1_centre_mass, im2_centre_mass;
            Eigen::Vector3 im1_centre_mass_transformed, im2_centre_mass_transformed;

            get_centre_of_mass (im1, mask1, im1_centre_mass);
            get_centre_of_mass (im2, mask2, im2_centre_mass);

            transform.transform_half_inverse (im1_centre_mass_transformed, im1_centre_mass);
            transform.transform_half (im2_centre_mass_transformed, im2_centre_mass);

            Eigen::Vector3 centre = (im1_centre_mass + im2_centre_mass) * 0.5;
            transform.set_centre_without_transform_update (centre);
          }

        template <class Im1ImageType,
                  class Im2ImageType,
                  class TransformType>
          void initialise_using_image_centres (const Im1ImageType& im1,
                                               const Im2ImageType& im2,
                                               TransformType& transform) {
            CONSOLE ("initialising centre of rotation and translation using geometric centre");
            Eigen::Vector3 im1_centre_scanner;
            get_geometric_centre (im1, im1_centre_scanner);

            Eigen::Vector3 im2_centre_scanner;
            get_geometric_centre (im2, im2_centre_scanner);

            Eigen::Vector3 translation = im1_centre_scanner - im2_centre_scanner;
            Eigen::Vector3 centre = (im1_centre_scanner + im2_centre_scanner) / 2.0;
            transform.set_centre (centre);
            transform.set_translation (translation);
          }

        void initialise_using_image_moments (Image<default_type>& im1,
                                             Image<default_type>& im2,
                                             Image<default_type>& mask1,
                                             Image<default_type>& mask2,
                                             Registration::Transform::Base& transform,
                                             bool use_mask_values = false) {
          if (use_mask_values) {
            if (!(mask1.valid() or mask2.valid()))
              throw Exception ("cannot run image moments initialisation using mask values without a valid mask");
            CONSOLE ("initialising using image moments using mask values instead of image values");
          }
          else
            CONSOLE ("initialising using image moments");
          auto init = Transform::Init::MomentsInitialiser (im1, im2, mask1, mask2, transform, use_mask_values);
          init.run();
        }

        void initialise_using_image_moments (Image<default_type>& im1,
                                             Image<default_type>& im2,
                                             Registration::Transform::Base& transform) {
          CONSOLE ("initialising using image moments with unmasked images");
          Image<default_type> mask1;
          Image<default_type> mask2;
          auto init = Transform::Init::MomentsInitialiser (im1, im2, mask1, mask2, transform, false);
          init.run();
        }

        void initialise_using_FOD (Image<default_type>& im1,
                                   Image<default_type>& im2,
                                   Image<default_type>& mask1,
                                   Image<default_type>& mask2,
                                   Registration::Transform::Base& transform,
                                   ssize_t lmax = -1) {
          CONSOLE ("initialising using masked images interpreted as FOD");
          auto init = Transform::Init::FODInitialiser (im1, im2, mask1, mask2, transform, lmax);
          init.run();
        }


        void initialise_using_FOD (Image<default_type>& im1,
                                   Image<default_type>& im2,
                                   Registration::Transform::Base& transform,
                                   ssize_t lmax = -1) {
          CONSOLE ("initialising using image moments with unmasked images");
          Image<default_type> mask1;
          Image<default_type> mask2;
          auto init = Transform::Init::FODInitialiser (im1, im2, mask1, mask2, transform, lmax);
          init.run();
        }


        void initialise_using_image_mass (Image<default_type>& im1,
                                          Image<default_type>& im2,
                                          Image<default_type>& mask1,
                                          Image<default_type>& mask2,
                                          Registration::Transform::Base& transform);

        void initialise_using_image_mass (Image<default_type>& im1,
                                          Image<default_type>& im2,
                                          Registration::Transform::Base& transform) {
          Image<default_type> bogus_mask;
          initialise_using_image_mass (im1, im2, bogus_mask, bogus_mask, transform);
        }
      }
    }
  }
}

#endif

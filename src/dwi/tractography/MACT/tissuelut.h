/*
 * Copyright (c) 2008-2018 the MRtrix3 contributors.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at http://mozilla.org/MPL/2.0/
 *
 * MRtrix3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * For more details, see http://www.mrtrix.org/
 */


#ifndef __dwi_tractography_mact_tissuelut_h__
#define __dwi_tractography_mact_tissuelut_h__


#include "dwi/tractography/MACT/keycomp.h"
#include "dwi/tractography/MACT/tissue.h"
#include <set>


namespace MR
{

namespace DWI
{

namespace Tractography
{

namespace MACT
{


class SceneModeller;


class TissueLut
{

  public:

    TissueLut( const std::shared_ptr< SceneModeller >& sceneModeller );
    virtual ~TissueLut();

    void update( Tissue_ptr Tissue );

    std::set< Tissue_ptr > getTissues( const Eigen::Vector3i& voxel ) const;
    std::set< Tissue_ptr > getTissues( const Eigen::Vector3d& point ) const;

    std::set< Tissue_ptr > getTissues( const std::set< Eigen::Vector3i, Vector3iCompare >& voxels ) const;
    std::set< Tissue_ptr > getTissues( const std::set< Eigen::Vector3d >& points ) const;

  private:

    std::shared_ptr< SceneModeller > _sceneModeller;
    std::map< Eigen::Vector3i, std::set< Tissue_ptr >, Vector3iCompare > _lut;

};


}

}

}

}

#endif

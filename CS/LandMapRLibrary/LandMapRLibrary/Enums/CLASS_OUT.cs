/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
namespace LandMapRLibrary
{
    /// <summary>
    /// the kind of fuzzy model to use to re-scale the raw input value for each grid cell into a
    /// fuzzy likelihood value scaled from 0 to 100. 
    /// </summary>
    public enum CLASS_OUT
    {
        CONVEX_D,
        CONCAVE_D,
        PLANAR_D,
        CONVEX_A,
        CONCAVE_A,
        PLANAR_A,
        HIGH_WI,
        LOW_WI,
        NEAR_LEVEL,
        REL_STEEP,
        NEAR_DIV,
        NEAR_HALF,
        NEAR_CHAN,
        NEAR_PEAK,
        NEAR_MID,
        NEAR_PIT,
        HI_ABOVE
    }

}

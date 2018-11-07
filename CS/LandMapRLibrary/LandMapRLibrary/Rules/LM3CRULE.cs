/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
using System;
using System.Collections.Generic;

namespace LandMapRLibrary
{
    /// <summary>
    /// Fuzzy facet rules.
    /// </summary>
    public class LM3CRULE
    {
        public string F_Name { get; set; }
        public string FuzAttr { get; set; }
        public int AttrWT { get; set; }
        public int Facet_No { get; set; }
        public int F_Code { get; set; }

        /// <summary>
        /// List of default fuzzy facet rules.
        /// </summary>
        public static List<LM3CRULE> Defaults = new List<LM3CRULE>
        {
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.NEAR_PEAK,30,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.NEAR_DIV,20,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.HI_ABOVE,10,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.NEAR_LEVEL,20,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.PLANAR_D,10,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.PLANAR_A,5,11,1),
            new LM3CRULE(F_NAME.LCR,CLASS_OUT.LOW_WI,5,11,1),

            new LM3CRULE(F_NAME.DSH,CLASS_OUT.NEAR_PEAK,30,12,2),
            new LM3CRULE(F_NAME.DSH,CLASS_OUT.NEAR_DIV,20,12,2),
            new LM3CRULE(F_NAME.DSH,CLASS_OUT.HI_ABOVE,10,12,2),
            new LM3CRULE(F_NAME.DSH,CLASS_OUT.CONVEX_D,20,12,2),
            new LM3CRULE(F_NAME.DSH,CLASS_OUT.CONVEX_A,10,12,2),
            new LM3CRULE(F_NAME.DSH,CLASS_OUT.LOW_WI,10,12,2),

            new LM3CRULE(F_NAME.UDE,CLASS_OUT.NEAR_PEAK,30,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.NEAR_DIV,20,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.HI_ABOVE,10,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.NEAR_LEVEL,10,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.PLANAR_D,10,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.PLANAR_A,10,13,3),
            new LM3CRULE(F_NAME.UDE,CLASS_OUT.HIGH_WI,10,13,3),

            new LM3CRULE(F_NAME.BSL,CLASS_OUT.NEAR_HALF,20,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.NEAR_MID,10,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.HI_ABOVE,5,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.REL_STEEP,20,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.PLANAR_D,15,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.PLANAR_A,25,21,4),
            new LM3CRULE(F_NAME.BSL,CLASS_OUT.LOW_WI,5,21,4),

            new LM3CRULE(F_NAME.DBS,CLASS_OUT.NEAR_HALF,20,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.NEAR_MID,10,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.HI_ABOVE,5,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.REL_STEEP,20,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.CONVEX_A,20,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.PLANAR_D,15,22,5),
            new LM3CRULE(F_NAME.DBS,CLASS_OUT.LOW_WI,10,22,5),

            new LM3CRULE(F_NAME.CBS,CLASS_OUT.NEAR_HALF,20,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.NEAR_MID,10,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.HI_ABOVE,5,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.REL_STEEP,20,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.CONCAVE_A,20,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.PLANAR_D,15,23,6),
            new LM3CRULE(F_NAME.CBS,CLASS_OUT.HIGH_WI,10,23,6),

            new LM3CRULE(F_NAME.TER,CLASS_OUT.NEAR_HALF,20,24,7),
            new LM3CRULE(F_NAME.TER,CLASS_OUT.NEAR_MID,10,24,7),
            new LM3CRULE(F_NAME.TER,CLASS_OUT.HI_ABOVE,5,24,7),
            new LM3CRULE(F_NAME.TER,CLASS_OUT.NEAR_LEVEL,30,24,7),
            new LM3CRULE(F_NAME.TER,CLASS_OUT.PLANAR_D,15,24,7),
            new LM3CRULE(F_NAME.TER,CLASS_OUT.PLANAR_A,20,24,7),

            new LM3CRULE(F_NAME.SAD,CLASS_OUT.NEAR_HALF,20,25,8),
            new LM3CRULE(F_NAME.SAD,CLASS_OUT.NEAR_MID,10,25,8),
            new LM3CRULE(F_NAME.SAD,CLASS_OUT.HI_ABOVE,5,25,8),
            new LM3CRULE(F_NAME.SAD,CLASS_OUT.NEAR_LEVEL,20,25,8),
            new LM3CRULE(F_NAME.SAD,CLASS_OUT.CONCAVE_D,20,25,8),
            new LM3CRULE(F_NAME.SAD,CLASS_OUT.CONVEX_A,20,25,8),

            new LM3CRULE(F_NAME.MDE,CLASS_OUT.NEAR_HALF,20,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.NEAR_MID,10,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.HI_ABOVE,5,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.NEAR_LEVEL,25,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.CONCAVE_D,10,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.CONCAVE_A,10,26,9),
            new LM3CRULE(F_NAME.MDE,CLASS_OUT.HIGH_WI,20,26,9),

            new LM3CRULE(F_NAME.FSL,CLASS_OUT.NEAR_CHAN,20,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.NEAR_PIT,10,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.REL_STEEP,10,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.CONCAVE_D,20,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.CONCAVE_A,20,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.PLANAR_A,10,31,10),
            new LM3CRULE(F_NAME.FSL,CLASS_OUT.HIGH_WI,20,31,10),

            new LM3CRULE(F_NAME.TSL,CLASS_OUT.NEAR_CHAN,20,32,11),
            new LM3CRULE(F_NAME.TSL,CLASS_OUT.NEAR_PIT,10,32,11),
            new LM3CRULE(F_NAME.TSL,CLASS_OUT.REL_STEEP,10,32,11),
            new LM3CRULE(F_NAME.TSL,CLASS_OUT.PLANAR_D,25,32,11),
            new LM3CRULE(F_NAME.TSL,CLASS_OUT.PLANAR_A,25,32,11),
            new LM3CRULE(F_NAME.TSL,CLASS_OUT.HIGH_WI,10,32,11),

            new LM3CRULE(F_NAME.FAN,CLASS_OUT.NEAR_CHAN,20,33,12),
            new LM3CRULE(F_NAME.FAN,CLASS_OUT.NEAR_PIT,10,33,12),
            new LM3CRULE(F_NAME.FAN,CLASS_OUT.REL_STEEP,10,33,12),
            new LM3CRULE(F_NAME.FAN,CLASS_OUT.CONVEX_A,25,33,12),
            new LM3CRULE(F_NAME.FAN,CLASS_OUT.PLANAR_D,25,33,12),
            new LM3CRULE(F_NAME.FAN,CLASS_OUT.LOW_WI,10,33,12),

            new LM3CRULE(F_NAME.LSM,CLASS_OUT.NEAR_DIV,10,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.NEAR_CHAN,20,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.NEAR_PIT,10,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.NEAR_PEAK,10,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.REL_STEEP,10,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.CONVEX_D,15,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.CONVEX_A,15,41,13),
            new LM3CRULE(F_NAME.LSM,CLASS_OUT.LOW_WI,10,41,13),

            new LM3CRULE(F_NAME.LLS,CLASS_OUT.NEAR_CHAN,20,42,14),
            new LM3CRULE(F_NAME.LLS,CLASS_OUT.NEAR_PIT,20,42,14),
            new LM3CRULE(F_NAME.LLS,CLASS_OUT.NEAR_LEVEL,40,42,14),
            new LM3CRULE(F_NAME.LLS,CLASS_OUT.PLANAR_D,5,42,14),
            new LM3CRULE(F_NAME.LLS,CLASS_OUT.PLANAR_A,5,42,14),
            new LM3CRULE(F_NAME.LLS,CLASS_OUT.HIGH_WI,10,42,14),

            new LM3CRULE(F_NAME.DEP,CLASS_OUT.NEAR_CHAN,20,43,15),
            new LM3CRULE(F_NAME.DEP,CLASS_OUT.NEAR_PIT,30,43,15),
            new LM3CRULE(F_NAME.DEP,CLASS_OUT.NEAR_LEVEL,20,43,15),
            new LM3CRULE(F_NAME.DEP,CLASS_OUT.CONCAVE_A,10,43,15),
            new LM3CRULE(F_NAME.DEP,CLASS_OUT.CONCAVE_D,10,43,15),
            new LM3CRULE(F_NAME.DEP,CLASS_OUT.HIGH_WI,10,43,15),
        };
    

        /// <summary>
        /// Constructor for new fuzzy facet rules.
        /// </summary>
        /// <param name="f_Name"></param>
        /// <param name="fuzAttr"></param>
        /// <param name="attrWT"></param>
        /// <param name="facet_No"></param>
        /// <param name="f_Code"></param>
        public LM3CRULE(F_NAME f_Name, CLASS_OUT fuzAttr, int attrWT, int facet_No, int f_Code)
        {
            F_Name = Enum.GetName(typeof(F_NAME), f_Name);
            FuzAttr = Enum.GetName(typeof(CLASS_OUT), fuzAttr);
            AttrWT = attrWT;
            Facet_No = facet_No;
            F_Code = f_Code;
        }
    }
}

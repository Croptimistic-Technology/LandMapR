/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
using System;
using System.Collections.Generic;

namespace LandMapRLibrary
{
    /// <summary>
    /// fuzzy attribute rules
    /// </summary>
    public class LM3ARULE
    {
        public int SortOrder { get; set; }
        public string FileIn { get; set; }
        public string AttrIn { get; set; }
        public string ClassOut { get; set; }
        public int ModelNo { get; set; }
        public double B { get; set; }
        public double BLow { get; set; }
        public double BHi { get; set; }
        public double B1 { get; set; }
        public double B2 { get; set; }
        public double D { get; set; }

        /// <summary>
        /// Default classification rules.
        /// </summary>
        public static List<LM3ARULE> Defaults = new List<LM3ARULE>
        {
            new LM3ARULE(1, FILE_IN.formfile, ATTR_IN.PROF, CLASS_OUT.CONVEX_D, 4, 5, 0, 0, 2.5, 0, 2.5),
            new LM3ARULE(2, FILE_IN.formfile, ATTR_IN.PROF, CLASS_OUT.CONCAVE_D, 5, -5, 0, 0, 0, -2.5, 2.5),
            new LM3ARULE(3, FILE_IN.formfile, ATTR_IN.PROF, CLASS_OUT.PLANAR_D, 1, 0, 0, 0, -2.5, 2.5, 2.5),
            new LM3ARULE(4, FILE_IN.formfile, ATTR_IN.PLAN, CLASS_OUT.CONVEX_A, 4, 5, 0, 0, 2.5, 0, 2.5),
            new LM3ARULE(5, FILE_IN.formfile, ATTR_IN.PLAN, CLASS_OUT.CONCAVE_A, 5, -5, 0, 0, 0, -2.5, 2.5),
            new LM3ARULE(6, FILE_IN.formfile, ATTR_IN.PLAN, CLASS_OUT.PLANAR_A, 1, 0, 0, 0, -2.5, 2.5, 2.5),
            new LM3ARULE(7, FILE_IN.formfile, ATTR_IN.QWETI, CLASS_OUT.HIGH_WI, 4, 7, 0, 0, 3.5, 0,3),
            new LM3ARULE(8, FILE_IN.formfile, ATTR_IN.QWETI, CLASS_OUT.LOW_WI, 5, 0.5, 0, 0, 0, 3.5, 3),
            new LM3ARULE(9, FILE_IN.formfile, ATTR_IN.SLOPE, CLASS_OUT.NEAR_LEVEL, 5, 0.5, 0, 0, 0, 1, 0.5),
            new LM3ARULE(10, FILE_IN.formfile, ATTR_IN.SLOPE, CLASS_OUT.REL_STEEP, 4, 2, 0, 0, 1, 0, 1),
            new LM3ARULE(11, FILE_IN.relzfile, ATTR_IN.PCTZ2ST, CLASS_OUT.NEAR_DIV, 4, 90, 0, 0, 75, 0, 15),
            new LM3ARULE(12, FILE_IN.relzfile, ATTR_IN.PCTZ2ST, CLASS_OUT.NEAR_HALF, 1, 50, 50, 50, 25, 75, 25),
            new LM3ARULE(13, FILE_IN.relzfile, ATTR_IN.PCTZ2ST, CLASS_OUT.NEAR_CHAN, 5, 10, 0, 0, 0, 25, 15),
            new LM3ARULE(14, FILE_IN.relzfile, ATTR_IN.PCTZ2PIT, CLASS_OUT.NEAR_PEAK, 4, 90, 0, 0, 75, 0, 15),
            new LM3ARULE(15, FILE_IN.relzfile, ATTR_IN.PCTZ2PIT, CLASS_OUT.NEAR_MID, 1, 50, 50, 50, 25, 75, 25),
            new LM3ARULE(16, FILE_IN.relzfile, ATTR_IN.PCTZ2PIT, CLASS_OUT.NEAR_PIT, 5, 5, 0, 0, 0, 10, 5),
            new LM3ARULE(17, FILE_IN.relzfile, ATTR_IN.Z2PIT, CLASS_OUT.HI_ABOVE, 4, 2, 0, 0, 1, 0 ,1),
        };

        /// <summary>
        /// Constructor for creating a new fuzzy rule record. A list of custom rules can be passed into the LandMapROptions but there is no guarantee that the new rules you set will crash the program.
        /// </summary>
        /// <param name="sortOrder"></param>
        /// <param name="fileIn"></param>
        /// <param name="attrIn"></param>
        /// <param name="classOut"></param>
        /// <param name="modelNo"></param>
        /// <param name="b"></param>
        /// <param name="bLow"></param>
        /// <param name="bHi"></param>
        /// <param name="b1"></param>
        /// <param name="b2"></param>
        /// <param name="d"></param>
        public LM3ARULE(int sortOrder, FILE_IN fileIn, ATTR_IN attrIn, CLASS_OUT classOut, int modelNo, double b, double bLow, double bHi, double b1, double b2, double d)
        {
            SortOrder = sortOrder;
            FileIn = Enum.GetName(typeof(FILE_IN), fileIn);
            AttrIn = Enum.GetName(typeof(ATTR_IN), attrIn);
            ClassOut = Enum.GetName(typeof(CLASS_OUT), classOut);
            ModelNo = modelNo;
            B = b;
            BLow = bLow;
            BHi = bHi;
            B1 = b1;
            B2 = b2;
            D = d;
        }
    }
}

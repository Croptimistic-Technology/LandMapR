/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{
    /// <summary>
    /// A class to represent a row in the '_Condensed_LSM_fuzzy_classes.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class Clsm
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public bool Missing { get; set; }
        public string MaxClass { get; set; }
        public int MaxCode { get; set; }
        public int MaxValue { get; set; }
        public int IDNo { get; set; }
        public string LandForms4Code { get; set; }
    }
}

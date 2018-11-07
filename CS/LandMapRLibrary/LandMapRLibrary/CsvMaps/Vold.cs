/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{
    /// <summary>
    /// A class to represent a row in the 'vold.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class Vold
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public int Ddir { get; set; }
        public int Drec { get; set; }
        public int Upslope { get; set; }
        public int ShedNow { get; set; }
        public int Stage { get; set; }
        public int Urec { get; set; }
        public int DsArea { get; set; }
        public double Varatio { get; set; }
    }
}

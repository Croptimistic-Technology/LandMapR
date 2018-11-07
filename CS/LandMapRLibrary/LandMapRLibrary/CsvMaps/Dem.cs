/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{
    /// <summary>
    /// A class to represent a row in the 'dem.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class Dem
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public int Row { get; set; }
        public int Column { get; set; }
        public double Elev { get; set; }
        public int Ddir { get; set; }
        public int Drec { get; set; }
        public int Upslope { get; set; }
        public int ShedNo { get; set; }
        public int ShedNow { get; set; }
        public bool Missing { get; set; }
        public bool Edge { get; set; }
        public double Vol2Fl { get; set; }
        public double Mm2fl { get; set; }
        public int PArea { get; set; }
    }
}

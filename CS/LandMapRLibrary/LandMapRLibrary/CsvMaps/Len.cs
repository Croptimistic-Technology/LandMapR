/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{
    /// <summary>
    /// A class to represent a row in the 'Len.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class Len
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public double L2PIT { get; set; }
        public double L2PEAK { get; set; }
        public double LPIT2PEAK { get; set; }
        public int PPIT2PEAKL { get; set; }
        public double L2STR { get; set; }
        public double L2DIV { get; set; }
        public double LSTR2DIV { get; set; }
        public int PSTR2DIVL { get; set; }
    }
}

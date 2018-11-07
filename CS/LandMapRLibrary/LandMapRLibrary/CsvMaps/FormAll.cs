/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{
    /// <summary>
    /// A class to represent a row in the 'form_all.csv' file. Properties top to bottom are aligned  with each row left to right.
    /// </summary>
    public class FormAll
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public double Slope { get; set; }
        public int Aspect { get; set; }
        public double Prof { get; set; }
        public double Plan { get; set; }
        public double QArea { get; set; }
        public double QWETI { get; set; }
        public double LNQArea { get; set; }
        public int NewAsp { get; set; }
    }
}

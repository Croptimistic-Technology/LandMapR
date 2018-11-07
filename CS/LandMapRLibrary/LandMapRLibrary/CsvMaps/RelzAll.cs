/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{

    /// <summary>
    /// A class to represent a row in the 'Relz_all.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class RelzAll
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int SeqNo { get; set; }
        public double Z2ST { get; set; }
        public double Z2CR { get; set; }
        public double Z2PIT { get; set; }
        public double Z2PEAK { get; set; }
        public double Z2TOP { get; set; }
        public double ZCR2ST { get; set; }
        public double ZPIT2PEAK { get; set; }
        public double ZTOP2PIT { get; set; }
        public double PCTZ2ST { get; set; }
        public double PCTZ2PIT { get; set; }
        public int PCTZ2TOP { get; set; }
        public int PMIN2MAX { get; set; }
        public int N2ST { get; set; }
        public int N2CR { get; set; }
        public int N2PEAK { get; set; }
    }
}

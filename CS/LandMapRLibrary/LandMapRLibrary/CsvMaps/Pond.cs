/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary.CsvMaps
{

    /// <summary>
    /// A class to represent a row in the 'pond.csv' file. Properties top to bottom are aligned with each row left to right.
    /// </summary>
    public class Pond
    {
        public double Easting { get; set; }
        public double Northing { get; set; }
        public int ShedNo { get; set; }
        public bool Edge { get; set; }
        public bool Final { get; set; }
        public int EndPit { get; set; }
        public int ShedArea { get; set; }
        public int PitRow { get; set; }
        public int PitCol { get; set; }
        public int PitRec { get; set; }
        public double PitElev { get; set; }
        public double PourElev { get; set; }
        public double PreVol { get; set; }
        public double PitVol { get; set; }
        public double Varatio { get; set; }
        public int PitArea { get; set; }
        public int DrainsTo { get; set; }
        public int NextPit { get; set; }
        public int Becomes { get; set; }
        public bool Removed { get; set; }
        public int InRow { get; set; }
        public int InColumn { get; set; }
        public int InRec { get; set; }
        public double InElev { get; set; }
        public int OutRow { get; set; }
        public int OutCol { get; set; }
        public int OutRec { get; set; }
        public double OutElev { get; set; }
        public int Stage { get; set; }
        public bool Visited { get; set; }
    }
}

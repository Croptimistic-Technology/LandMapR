/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace LandMapRLibrary
{
    /// <summary>
    /// A result classes that contains all the file paths for the files generated.
    /// </summary>
    public class LandMapRResult
    {
        public bool Success { get; set; }
        public string ErrorMessage { get; set; }
        public string CondensedLSMFuzzyClassesPath { get; set; }
        public string DemPath { get; set; }
        public string FillPath { get; set; }
        public string FormAllPath { get; set; }
        public string IDemPath { get; set; }
        public string IPitPath { get; set; }
        public string LenPath { get; set; }
        public string MoldPath { get; set; }
        public string PitPath { get; set; }
        public string PondPath { get; set; }
        public string RelzAllPath { get; set; }
        public string VoldPath { get; set; }
    }
}

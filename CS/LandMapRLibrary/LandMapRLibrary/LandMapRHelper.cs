/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

using LandMapRLibrary.CsvMaps;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LandMapRLibrary
{
    /// <summary>
    /// Helper class to read points from the generated csv files into list's of C# classes. No buffered file streams are used so if the file is too large it may error out.
    /// </summary>
    public static class LandMapRHelper
    {
        /// <summary>
        /// Reads the '_Condensed_LSM_fuzzy_classes.csv' file from the LandMapRResult and returns a list of Clsm points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Clsm> GetCondensedLSMFuzzyClassesPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.CondensedLSMFuzzyClassesPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Clsm
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Missing = Convert.ToBoolean(x[3]),
                     MaxClass = x[4],
                     MaxCode = Convert.ToInt32(x[5]),
                     MaxValue = Convert.ToInt32(x[6]),
                     IDNo = Convert.ToInt32(x[7]),
                     LandForms4Code = x[8]
                 });
        }

        /// <summary>
        /// Reads the 'dem.csv' file from the LandMapRResult and returns a list of Dem points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Dem> GetDemPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.DemPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Dem
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Row = Convert.ToInt32(x[3]),
                     Column = Convert.ToInt32(x[4]),
                     Elev = Convert.ToDouble(x[5]),
                     Ddir = Convert.ToInt32(x[6]),
                     Drec = Convert.ToInt32(x[7]),
                     Upslope = Convert.ToInt32(x[8]),
                     ShedNo = Convert.ToInt32(x[9]),
                     ShedNow = Convert.ToInt32(x[10]),
                     Missing = Convert.ToBoolean(x[11]),
                     Edge = Convert.ToBoolean(x[12]),
                     Vol2Fl = Convert.ToDouble(x[13]),
                     Mm2fl = Convert.ToDouble(x[14]),
                     PArea = Convert.ToInt32(x[15]),
                 });
        }

        /// <summary>
        /// Reads the 'fill.csv' file from the LandMapRResult and returns a list of Fill points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Fill> GetFillPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.FillPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Fill
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     ShedNo = Convert.ToInt32(x[2]),
                     Edge = Convert.ToBoolean(x[3]),
                     Final = Convert.ToBoolean(x[4]),
                     EndPit = Convert.ToInt32(x[5]),
                     ShedArea = Convert.ToInt32(x[6]),
                     PitRow = Convert.ToInt32(x[7]),
                     PitCol = Convert.ToInt32(x[8]),
                     PitRec = Convert.ToInt32(x[9]),
                     PitElev = Convert.ToDouble(x[10]),
                     PourElev = Convert.ToDouble(x[11]),
                     PreVol = Convert.ToDouble(x[12]),
                     PitVol = Convert.ToDouble(x[13]),
                     Varatio = Convert.ToDouble(x[14]),
                     PitArea = Convert.ToInt32(x[15]),
                     DrainsTo = Convert.ToInt32(x[16]),
                     NextPit = Convert.ToInt32(x[17]),
                     Becomes = Convert.ToInt32(x[18]),
                     Removed = Convert.ToBoolean(x[19]),
                     InRow = Convert.ToInt32(x[20]),
                     InColumn = Convert.ToInt32(x[21]),
                     InRec = Convert.ToInt32(x[22]),
                     InElev = Convert.ToDouble(x[23]),
                     OutRow = Convert.ToInt32(x[24]),
                     OutCol = Convert.ToInt32(x[25]),
                     OutRec = Convert.ToInt32(x[26]),
                     OutElev = Convert.ToDouble(x[27]),
                     Stage = Convert.ToInt32(x[28]),
                     Visited = Convert.ToBoolean(x[29]),
                 });
        }

        /// <summary>
        /// Reads the 'form_all.csv' file from the LandMapRResult and returns a list of FormAll points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<FormAll> GetFormAllPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.FormAllPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new FormAll
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Slope = Convert.ToDouble(x[3]),
                     Aspect = Convert.ToInt32(x[4]),
                     Prof = Convert.ToDouble(x[5]),
                     Plan = Convert.ToDouble(x[6]),
                     QArea = Convert.ToDouble(x[7]),
                     QWETI = Convert.ToDouble(x[8]),
                     LNQArea = Convert.ToDouble(x[9]),
                     NewAsp = Convert.ToInt32(x[10]),
                 });
        }

        /// <summary>
        /// Reads the 'idem.csv' file from the LandMapRResult and returns a list of IDem points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<IDem> GetIDemPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.IDemPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new IDem
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Row = Convert.ToInt32(x[3]),
                     Column = Convert.ToInt32(x[4]),
                     Elev = Convert.ToDouble(x[5]),
                     Ddir = Convert.ToInt32(x[6]),
                     Drec = Convert.ToInt32(x[7]),
                     Upslope = Convert.ToInt32(x[8]),
                     ShedNo = Convert.ToInt32(x[9]),
                     ShedNow = Convert.ToInt32(x[10]),
                     Missing = Convert.ToBoolean(x[11]),
                     Edge = Convert.ToBoolean(x[12]),
                     Vol2Fl = Convert.ToDouble(x[13]),
                     Mm2fl = Convert.ToDouble(x[14]),
                     PArea = Convert.ToInt32(x[15]),
                 });
        }


        /// <summary>
        /// Reads the 'ipit.csv' file from the LandMapRResult and returns a list of IPit points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<IPit> GetIPitPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.IPitPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new IPit
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     ShedNo = Convert.ToInt32(x[2]),
                     Edge = Convert.ToBoolean(x[3]),
                     Final = Convert.ToBoolean(x[4]),
                     EndPit = Convert.ToInt32(x[5]),
                     ShedArea = Convert.ToInt32(x[6]),
                     PitRow = Convert.ToInt32(x[7]),
                     PitCol = Convert.ToInt32(x[8]),
                     PitRec = Convert.ToInt32(x[9]),
                     PitElev = Convert.ToDouble(x[10]),
                     PourElev = Convert.ToDouble(x[11]),
                     PreVol = Convert.ToDouble(x[12]),
                     PitVol = Convert.ToDouble(x[13]),
                     Varatio = Convert.ToDouble(x[14]),
                     PitArea = Convert.ToInt32(x[15]),
                     DrainsTo = Convert.ToInt32(x[16]),
                     NextPit = Convert.ToInt32(x[17]),
                     Becomes = Convert.ToInt32(x[18]),
                     Removed = Convert.ToBoolean(x[19]),
                     InRow = Convert.ToInt32(x[20]),
                     InColumn = Convert.ToInt32(x[21]),
                     InRec = Convert.ToInt32(x[22]),
                     InElev = Convert.ToDouble(x[23]),
                     OutRow = Convert.ToInt32(x[24]),
                     OutCol = Convert.ToInt32(x[25]),
                     OutRec = Convert.ToInt32(x[26]),
                     OutElev = Convert.ToDouble(x[27]),
                     Stage = Convert.ToInt32(x[28]),
                     Visited = Convert.ToBoolean(x[29]),
                 });
        }

        /// <summary>
        /// Reads the 'Len.csv' file from the LandMapRResult and returns a list of Len points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Len> GetLenPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.LenPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Len
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     L2PIT = Convert.ToDouble(x[3]),
                     L2PEAK = Convert.ToDouble(x[4]),
                     LPIT2PEAK = Convert.ToDouble(x[5]),
                     PPIT2PEAKL = Convert.ToInt32(x[6]),
                     L2STR = Convert.ToDouble(x[7]),
                     L2DIV = Convert.ToDouble(x[8]),
                     LSTR2DIV = Convert.ToDouble(x[9]),
                     PSTR2DIVL = Convert.ToInt32(x[10]),
                 });
        }

        /// <summary>
        /// Reads the 'mold.csv' file from the LandMapRResult and returns a list of Mold points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Mold> GetMoldPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.MoldPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Mold
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Ddir = Convert.ToInt32(x[3]),
                     Drec = Convert.ToInt32(x[4]),
                     Upslope = Convert.ToInt32(x[5]),
                     ShedNow = Convert.ToInt32(x[6]),
                     Stage = Convert.ToInt32(x[7]),
                     Urec = Convert.ToInt32(x[8]),
                     DsArea = Convert.ToInt32(x[9]),
                     Varatio = Convert.ToDouble(x[10]),
                 });
        }

        /// <summary>
        /// Reads the 'pit.csv' file from the LandMapRResult and returns a list of Pit points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Pit> GetPitPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.PitPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Pit
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     ShedNo = Convert.ToInt32(x[2]),
                     Edge = Convert.ToBoolean(x[3]),
                     Final = Convert.ToBoolean(x[4]),
                     EndPit = Convert.ToInt32(x[5]),
                     ShedArea = Convert.ToInt32(x[6]),
                     PitRow = Convert.ToInt32(x[7]),
                     PitCol = Convert.ToInt32(x[8]),
                     PitRec = Convert.ToInt32(x[9]),
                     PitElev = Convert.ToDouble(x[10]),
                     PourElev = Convert.ToDouble(x[11]),
                     PreVol = Convert.ToDouble(x[12]),
                     PitVol = Convert.ToDouble(x[13]),
                     Varatio = Convert.ToDouble(x[14]),
                     PitArea = Convert.ToInt32(x[15]),
                     DrainsTo = Convert.ToInt32(x[16]),
                     NextPit = Convert.ToInt32(x[17]),
                     Becomes = Convert.ToInt32(x[18]),
                     Removed = Convert.ToBoolean(x[19]),
                     InRow = Convert.ToInt32(x[20]),
                     InColumn = Convert.ToInt32(x[21]),
                     InRec = Convert.ToInt32(x[22]),
                     InElev = Convert.ToDouble(x[23]),
                     OutRow = Convert.ToInt32(x[24]),
                     OutCol = Convert.ToInt32(x[25]),
                     OutRec = Convert.ToInt32(x[26]),
                     OutElev = Convert.ToDouble(x[27]),
                     Stage = Convert.ToInt32(x[28]),
                     Visited = Convert.ToBoolean(x[29]),
                 });
        }

        /// <summary>
        /// Reads the 'pong.csv' file from the LandMapRResult and returns a list of Pond points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Pond> GetPondPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.PondPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Pond
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     ShedNo = Convert.ToInt32(x[2]),
                     Edge = Convert.ToBoolean(x[3]),
                     Final = Convert.ToBoolean(x[4]),
                     EndPit = Convert.ToInt32(x[5]),
                     ShedArea = Convert.ToInt32(x[6]),
                     PitRow = Convert.ToInt32(x[7]),
                     PitCol = Convert.ToInt32(x[8]),
                     PitRec = Convert.ToInt32(x[9]),
                     PitElev = Convert.ToDouble(x[10]),
                     PourElev = Convert.ToDouble(x[11]),
                     PreVol = Convert.ToDouble(x[12]),
                     PitVol = Convert.ToDouble(x[13]),
                     Varatio = Convert.ToDouble(x[14]),
                     PitArea = Convert.ToInt32(x[15]),
                     DrainsTo = Convert.ToInt32(x[16]),
                     NextPit = Convert.ToInt32(x[17]),
                     Becomes = Convert.ToInt32(x[18]),
                     Removed = Convert.ToBoolean(x[19]),
                     InRow = Convert.ToInt32(x[20]),
                     InColumn = Convert.ToInt32(x[21]),
                     InRec = Convert.ToInt32(x[22]),
                     InElev = Convert.ToDouble(x[23]),
                     OutRow = Convert.ToInt32(x[24]),
                     OutCol = Convert.ToInt32(x[25]),
                     OutRec = Convert.ToInt32(x[26]),
                     OutElev = Convert.ToDouble(x[27]),
                     Stage = Convert.ToInt32(x[28]),
                     Visited = Convert.ToBoolean(x[29]),
                 });
        }

        /// <summary>
        /// Reads the 'Relz_all.csv' file from the LandMapRResult and returns a list of RelzAll points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<RelzAll> GetRelzAllPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.RelzAllPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new RelzAll
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Z2ST = Convert.ToDouble(x[3]),
                     Z2CR = Convert.ToDouble(x[4]),
                     Z2PIT = Convert.ToDouble(x[5]),
                     Z2PEAK = Convert.ToDouble(x[6]),
                     Z2TOP = Convert.ToDouble(x[7]),
                     ZCR2ST = Convert.ToDouble(x[8]),
                     ZPIT2PEAK = Convert.ToDouble(x[9]),
                     ZTOP2PIT = Convert.ToDouble(x[10]),
                     PCTZ2ST = Convert.ToDouble(x[11]),
                     PCTZ2PIT = Convert.ToDouble(x[12]),
                     PCTZ2TOP = Convert.ToInt32(x[13]),
                     PMIN2MAX = Convert.ToInt32(x[14]),
                     N2ST = Convert.ToInt32(x[15]),
                     N2CR = Convert.ToInt32(x[16]),
                     N2PEAK = Convert.ToInt32(x[17]),
                 });
        }


        /// <summary>
        /// Reads the 'vold.csv' file from the LandMapRResult and returns a list of Vold points.
        /// </summary>
        /// <param name="landMapRResult"></param>
        /// <returns></returns>
        public static IEnumerable<Vold> GetVoldPoints(LandMapRResult landMapRResult)
        {
            if (!landMapRResult.Success)
                throw new Exception("LandMapR result hasn't completed successfully. Check the Error message in the LandMapRResult object.");

            return File.ReadAllLines(landMapRResult.VoldPath)
                 .Skip(1)
                 .Select(x => x.Split(','))
                 .Select(x => new Vold
                 {
                     Easting = Convert.ToDouble(x[0]),
                     Northing = Convert.ToDouble(x[1]),
                     SeqNo = Convert.ToInt32(x[2]),
                     Ddir = Convert.ToInt32(x[3]),
                     Drec = Convert.ToInt32(x[4]),
                     Upslope = Convert.ToInt32(x[5]),
                     ShedNow = Convert.ToInt32(x[6]),
                     Stage = Convert.ToInt32(x[7]),
                     Urec = Convert.ToInt32(x[8]),
                     DsArea = Convert.ToInt32(x[9]),
                     Varatio = Convert.ToDouble(x[10]),
                 });
        }
    }
}

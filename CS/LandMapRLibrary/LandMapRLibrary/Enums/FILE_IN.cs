/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
 
namespace LandMapRLibrary
{
    /// <summary>
    /// The field File_in tells the program to obtain data for the first 10 attributes 
    /// from data stored in the formfile (ID#Form) DBF table and to obtain data to compute
    /// the next 7 fuzzy attributes from the relzfile (ID#Relz)
    /// </summary>
    public enum FILE_IN
    {
        formfile,
        relzfile
    }
}

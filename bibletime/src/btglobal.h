/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTGLOBAL_H
#define BTGLOBAL_H

/**
  Filter options to control the text display of modules. Uses int and not bool
  because not all options have just two toggle values.
*/
struct FilterOptions {
    int footnotes; /**< 0 for disabled, 1 for enabled */
    int strongNumbers; /**< 0 for disabled, 1 for enabled */
    int headings; /**< 0 for disabled, 1 for enabled */
    int morphTags; /**< 0 for disabled, 1 for enabled */
    int lemmas; /**< 0 for disabled, 1 for enabled */
    int hebrewPoints; /**< 0 for disabled, 1 for enabled */
    int hebrewCantillation; /**< 0 for disabled, 1 for enabled */
    int greekAccents; /**< 0 for disabled, 1 for enabled */
    int textualVariants; /**< Number n to enabled the n-th variant */
    int redLetterWords; /**< 0 for disabled, 1 for enabled */
    int scriptureReferences; /**< 0 for disabled, 1 for enabled */
    int morphSegmentation; /**< 0 for disabled, 1 for enabled */
};
Q_DECLARE_METATYPE(FilterOptions)

/**
  Controls the display of a text.
*/
struct DisplayOptions {
    int lineBreaks;
    int verseNumbers;
};
Q_DECLARE_METATYPE(DisplayOptions)

#endif // BTGLOBAL_H
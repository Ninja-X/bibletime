/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2016 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "cswordkey.h"

#include <QRegExp>
#include <QString>
#include <QTextCodec>
#include <swordxx/keys/treekey.h>
#include <swordxx/keys/treekeyidx.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/swkey.h>
#include <swordxx/swmodule.h>
#include <swordxx/utilstr.h>
#include "../../util/btassert.h"
#include "../drivers/cswordmoduleinfo.h"
#include "cswordldkey.h"
#include "cswordtreekey.h"
#include "cswordversekey.h"


const QTextCodec * CSwordKey::m_cp1252Codec = QTextCodec::codecForName("Windows-1252");

QString CSwordKey::rawText() {
    if (!m_module)
        return QString::null;

    auto & m = m_module->module();
    if (dynamic_cast<swordxx::SWKey *>(this))
        m.getKey()->setText( rawKey() );

    if (key().isNull())
        return QString::null;

    return QString::fromUtf8(m.getRawEntry().c_str());
}

QString CSwordKey::renderedText(const CSwordKey::TextRenderType mode) {
    BT_ASSERT(m_module);

    swordxx::SWKey * const k = dynamic_cast<swordxx::SWKey *>(this);

    auto & m = m_module->module();
    if (k) {
        swordxx::VerseKey * vk_mod = dynamic_cast<swordxx::VerseKey *>(m.getKey());
        if (vk_mod)
            vk_mod->setIntros(true);

        m.getKey()->setText(rawKey());

        if (m_module->type() == CSwordModuleInfo::Lexicon) {
            m_module->snap();
            /* In lexicons make sure that our key (e.g. 123) was successfully set to the module,
            i.e. the module key contains this key (e.g. 0123 contains 123) */

            if (swordxx::stricmp(m.getKey()->getText(), rawKey())
                && !strstr(m.getKey()->getText(), rawKey()))
            {
                qDebug("return an empty key for %s", m.getKey()->getText());
                return QString::null;
            }
        }
    }

    if (key().isNull())
        return QString::null;

    bool DoRender = mode != ProcessEntryAttributesOnly;
    QString text = QString::fromUtf8(m.renderText(nullptr, -1, DoRender).c_str());
    if (!DoRender)
        return QString::null;

    // This is yucky, but if we want strong lexicon refs we have to do it here.
    if (m_module->type() == CSwordModuleInfo::Lexicon) {
        const QString t(text);
        const QRegExp rx("(GREEK|HEBREW) for 0*([1-9]\\d*)");    // ignore 0's before number
        int pos = 0;
        while ((pos = rx.indexIn(t, pos)) != -1) {
            const QString language = rx.cap(1);
            const QString langcode = QString(language.at(0));    // "G" or "H"
            const QString number = rx.cap(2);
            const QString paddednumber = number.rightJustified(5, '0');    // Form 00123

            text.replace(
                QRegExp(QString(
                            "(>[^<>]+)"            // Avoid replacing inside tags
                            "\\b(0*%1)\\b").arg(number)),    // And span around 0's
                QString("\\1<span lemma=\"%1%2\"><a href=\"strongs://%3/%4\">\\2</a></span>")
                    .arg(langcode, paddednumber, language, paddednumber)
            );
            pos += rx.matchedLength();
        }
    }

    if (mode == HTMLEscaped) {
        /*
          Here we encode all non-latin1 characters as HTML unicode entities
          in the form &#<decimal unicode value here>;
        */
        QString ret;

        // Reserve characters to reduce number of memory allocations:
        ret.reserve(text.size());

        for (const QChar * c = text.constBegin(); c != text.constEnd(); c++) {
            if (c->toLatin1()) {
                ret.append(*c);
            } else {
                ret.append("&#").append(c->unicode()).append(";");
            }
        }

        return ret;
    }
    else {
        return text;
    }
}

QString CSwordKey::strippedText() {
    if (!m_module)
        return QString::null;

    auto & m = m_module->module();
    if (dynamic_cast<swordxx::SWKey*>(this)) {
        char * buffer = new char[strlen(rawKey()) + 1];
        strcpy(buffer, rawKey());
        m.getKey()->setText(buffer);
        delete [] buffer;
    }

    return QString::fromUtf8(m.stripText().c_str());
}

void CSwordKey::emitBeforeChanged() {
    if (!m_beforeChangedSignaller.isNull())
        m_beforeChangedSignaller->emitSignal();
}

void CSwordKey::emitAfterChanged() {
    if (!m_afterChangedSignaller.isNull())
        m_afterChangedSignaller->emitSignal();
}

CSwordKey * CSwordKey::createInstance(const CSwordModuleInfo * module) {
    if (!module)
        return nullptr;

    swordxx::SWKey * const key = module->module().getKey();

    switch (module->type()) {

        case CSwordModuleInfo::Bible: // Fall through
        case CSwordModuleInfo::Commentary:

            BT_ASSERT(dynamic_cast<swordxx::VerseKey *>(key));
            return new CSwordVerseKey(static_cast<swordxx::VerseKey *>(key),
                                      module);

        case CSwordModuleInfo::Lexicon:

            return new CSwordLDKey(key, module);

        case CSwordModuleInfo::GenericBook:

            BT_ASSERT(dynamic_cast<swordxx::TreeKeyIdx *>(key));
            return new CSwordTreeKey(dynamic_cast<swordxx::TreeKeyIdx *>(key),
                                     module );

        default:

            return nullptr;

    }
}

const BtSignal * CSwordKey::beforeChangedSignaller() {
    if (m_beforeChangedSignaller.isNull())
        m_beforeChangedSignaller = new BtSignal();

    return m_beforeChangedSignaller;
}

const BtSignal * CSwordKey::afterChangedSignaller() {
    if (m_afterChangedSignaller.isNull())
        m_afterChangedSignaller = new BtSignal();

    return m_afterChangedSignaller;
}

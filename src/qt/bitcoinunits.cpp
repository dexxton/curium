<<<<<<< HEAD
// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The CRU developers
// Distributed under the MIT/X11 software license, see the accompanying
=======
// Copyright (c) 2011-2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
>>>>>>> dev-1.12.1.0
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bitcoinunits.h"
#include "chainparams.h"
#include "primitives/transaction.h"

#include <QSettings>
#include <QStringList>

BitcoinUnits::BitcoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<BitcoinUnits::Unit> BitcoinUnits::availableUnits()
{
    QList<BitcoinUnits::Unit> unitlist;
    unitlist.append(CURIUM);
    unitlist.append(mCURIUM);
    unitlist.append(uCURIUM);
    unitlist.append(crus);
    return unitlist;
}

bool BitcoinUnits::valid(int unit)
{
    switch(unit)
    {
    case CURIUM:
    case mCURIUM:
    case uCURIUM:
    case crus:
        return true;
    default:
        return false;
    }
}

<<<<<<< HEAD
QString BitcoinUnits::id(int unit)
{
    switch(unit)
    {
        case CRU: return QString("cru");
        case mCRU: return QString("mcru");
        case uCRU: return QString::fromUtf8("ucru");
        case crus: return QString("crus");
        default: return QString("???");
    }
}

=======
>>>>>>> dev-1.12.1.0
QString BitcoinUnits::name(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
            case CURIUM: return QString("CURIUM");
            case mCURIUM: return QString("mCURIUM");
            case uCURIUM: return QString::fromUtf8("μCURIUM");
            case crus: return QString("crus");
            default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
            case CURIUM: return QString("tCURIUM");
            case mCURIUM: return QString("mtCURIUM");
            case uCURIUM: return QString::fromUtf8("μtCURIUM");
            case crus: return QString("tcrus");
            default: return QString("???");
        }
    }
}

QString BitcoinUnits::description(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
<<<<<<< HEAD
            case CRU: return QString("CRU");
            case mCRU: return QString("Milli-CRU (1 / 1" THIN_SP_UTF8 "000)");
            case uCRU: return QString("Micro-CRU (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case crus: return QString("Ten Nano-CRU (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
=======
            case CURIUM: return QString("Curium");
            case mCURIUM: return QString("Milli-Curium (1 / 1" THIN_SP_UTF8 "000)");
            case uCURIUM: return QString("Micro-Curium (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case crus: return QString("Ten Nano-Curium (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
>>>>>>> dev-1.12.1.0
            default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
<<<<<<< HEAD
            case CRU: return QString("TestCRUs");
            case mCRU: return QString("Milli-TestCRU (1 / 1" THIN_SP_UTF8 "000)");
            case uCRU: return QString("Micro-TestCRU (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case crus: return QString("Ten Nano-TestCRU (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
=======
            case CURIUM: return QString("TestCuriums");
            case mCURIUM: return QString("Milli-TestCurium (1 / 1" THIN_SP_UTF8 "000)");
            case uCURIUM: return QString("Micro-TestCurium (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            case crus: return QString("Ten Nano-TestCurium (1 / 100" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
>>>>>>> dev-1.12.1.0
            default: return QString("???");
        }
    }
}

qint64 BitcoinUnits::factor(int unit)
{
    switch(unit)
    {
    case CURIUM:  return 100000000;
    case mCURIUM: return 100000;
    case uCURIUM: return 100;
    case crus: return 1;
    default:   return 100000000;
    }
}

int BitcoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case CURIUM: return 8;
    case mCURIUM: return 5;
    case uCURIUM: return 2;
    case crus: return 0;
    default: return 0;
    }
}

QString BitcoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals <= 0)
        return quotient_str;

    return quotient_str + QString(".") + remainder_str;
}


// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString BitcoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString BitcoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString BitcoinUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

    QString result = format(unit, amount, plussign, separators);
    if(decimals(unit) > digits) result.chop(decimals(unit) - digits);

    return result + QString(" ") + name(unit);
}

QString BitcoinUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

bool BitcoinUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString BitcoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (BitcoinUnits::valid(unit))
    {
        amountTitle += " ("+BitcoinUnits::name(unit) + ")";
    }
    return amountTitle;
}

int BitcoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant BitcoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount BitcoinUnits::maxMoney()
{
    return MAX_MONEY;
}

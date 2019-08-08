#include "MegaAlertDelegate.h"
#include <QPainter>
#include "megaapi.h"
#include <QDebug>
#include <QSortFilterProxyModel>
#include "assert.h"

using namespace mega;

MegaAlertDelegate::MegaAlertDelegate(QAlertsModel *model, bool useProxyModel, QObject *parent)
    : QStyledItemDelegate(parent)
{
    this->model = model;
    useProxy = useProxyModel;
}

void MegaAlertDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {       

        //Map index when we are using QSortFilterProxyModel
        // if we are using QAbstractItemModel just access internalPointer casting to MegaAlert
        MegaUserAlert *alert = NULL;
        if (useProxy)
        {
            QModelIndex actualId = ((QSortFilterProxyModel*)index.model())->mapToSource(index);
            if (!(actualId.isValid()))
            {
                QStyledItemDelegate::paint(painter, option, index);
                return;
            }

            alert = (MegaUserAlert *)actualId.internalPointer();
        }
        else
        {
            alert = (MegaUserAlert *)index.internalPointer();
        }

        if (!alert)
        {
            assert(false || "No alert found");
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        AlertItem *ti = model->alertItems[alert->getId()];
        if (!ti)
        {
            ti = new AlertItem();
            ti->setAlertData(alert);
            model->alertItems.insert(alert->getId(), ti);
        }
        else
        {
            ti->setAlertData(alert);
        }

        painter->save();
        painter->translate(option.rect.topLeft());

        ti->resize(option.rect.width(), option.rect.height());

        ti->render(painter, QPoint(0, 0), QRegion(0, 0, option.rect.width(), option.rect.height()));
        painter->restore();
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize MegaAlertDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        return QSize(400, 122);
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}
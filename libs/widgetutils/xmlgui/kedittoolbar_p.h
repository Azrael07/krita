/* This file is part of the KDE libraries
   SPDX-FileCopyrightText: 2000 Kurt Granroth <granroth@kde.org>
   SPDX-FileCopyrightText: 2006 Hamish Rodda <rodda@kde.org>

   SPDX-License-Identifier: LGPL-2.0-only
*/
#ifndef KEDITTOOLBARP_H
#define KEDITTOOLBARP_H

#include "kxmlguiclient.h"
#include <QDialog>
#include <QListWidget>

class QDialogButtonBox;
class QLineEdit;
class QCheckBox;

namespace KDEPrivate
{

class ToolBarItem;
class KisKEditToolBarWidgetPrivate;

class ToolBarListWidget : public QListWidget
{
    Q_OBJECT
public:
    ToolBarListWidget(QWidget *parent = 0);

    void makeVisible(QListWidgetItem *item)
    {
        scrollTo(indexFromItem(item));
    }

    ToolBarItem *currentItem() const;

    void setActiveList(bool isActiveList)
    {
        m_activeList = isActiveList;
    }

Q_SIGNALS:
    void dropped(ToolBarListWidget *list, int index, ToolBarItem *item, bool sourceIsActiveList);

protected:
    Qt::DropActions supportedDropActions() const override
    {
        return Qt::MoveAction;
    }
    QStringList mimeTypes() const override
    {
        return QStringList() << QStringLiteral("application/x-kde-action-list");
    }

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QMimeData *mimeData(const QList<QListWidgetItem *> items) const override;
#else
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;
#endif
    bool dropMimeData(int index, const QMimeData *data, Qt::DropAction action) override;

    // Skip internal dnd handling in QListWidget ---- how is one supposed to figure this out
    // without reading the QListWidget code !?
    void dropEvent(QDropEvent *ev) override
    {
        QAbstractItemView::dropEvent(ev);
    }

private:
    bool m_activeList;
};

class IconTextEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IconTextEditDialog(QWidget *parent = 0);

public:
    void setIconText(const QString &text);
    QString iconText() const;

    void setTextAlongsideIconHidden(bool hidden);
    bool textAlongsideIconHidden() const;

private Q_SLOTS:
    void slotTextChanged(const QString &text);

private:
    QLineEdit *m_lineEdit;
    QCheckBox *m_cbHidden;
    QDialogButtonBox *m_buttonBox;
};

/**
 * @short A widget used to customize or configure toolbars
 *
 * This is the widget that does all of the work for the
 * KisKEditToolBar dialog.  In most cases, you will want to use the
 * dialog instead of this widget directly.
 *
 * Typically, you would use this widget only if you wanted to embed
 * the toolbar editing directly into your existing configure or
 * preferences dialog.
 *
 * This widget only works if your application uses the XML UI
 * framework for creating menus and toolbars.  It depends on the XML
 * files to describe the toolbar layouts and it requires the actions
 * to determine which buttons are active.
 *
 * @author Kurt Granroth <granroth@kde.org>
 * @internal
 */
class KisKEditToolBarWidget : public QWidget, virtual public KisKXMLGUIClient
{
    Q_OBJECT
public:
    /**
     * Old constructor for apps that do not use components.
     * This constructor is somewhat deprecated, since it doesn't work
     * with any KXMLGuiClient being added to the mainwindow.
     * You really want to use the other constructor.
     *
     * You @em must pass along your collection of actions (some of which appear in your toolbars).
     * Then call old-style load.
     *
     * @param collection The collection of actions to work on
     * @param parent This widget's parent
     */
    explicit KisKEditToolBarWidget(KisKActionCollection *collection,
                                QWidget *parent = 0L);

    /**
     * Main constructor.
     *
     * Use this like so:
     * \code
     * KisKEditToolBarWidget widget(this);
     * widget.load(factory());
     * ...
     * \endcode
     *
     * @param parent This widget's parent
     */
    explicit KisKEditToolBarWidget(QWidget *parent = 0L);

    /**
     * Destructor.  Note that any changes done in this widget will
     * @p NOT be saved in the destructor.  You @p must call save()
     * to do that.
     */
    ~KisKEditToolBarWidget() override;

    /**
     * Old-style load.
     *
     * Loads the toolbar configuration into the widget. Should be called before being shown.
     *
     * @param resourceFile the name (absolute or relative) of your application's UI
     * resource file.  If it is left blank, then the resource file: share/apps/appname/appnameui.xmlgui
     * is used.  This is the same resource file that is used by the
     * default createGUI function in KisKMainWindow so you're usually
     * pretty safe in leaving it blank.
     *
     * @param global controls whether or not the
     * global resource file is used.  If this is true, then you may
     * edit all of the actions in your toolbars -- global ones and
     * local one.  If it is false, then you may edit only your
     * application's entries.  The only time you should set this to
     * false is if your application does not use the global resource
     * file at all (very rare)
     *
     * @param defaultToolBar the default toolbar that will be selected when the dialog is shown.
     * If not set, or QString() is passed in, the global default tool bar name
     * will be used.
     *
     * @see KisKEditToolBar
     */
    void load(const QString &resourceFile,
              bool global = true,
              const QString &defaultToolBar = QString());

    /**
     * Loads the toolbar configuration into the widget. Should be called before being shown.
     *
     * @param factory pointer to the XML GUI factory object for your application.
     * It contains a list of all of the GUI clients (along with the action
     * collections and xml files) and the toolbar editor uses that.
     *
     * @param defaultToolBar the default toolbar that will be selected when the dialog is shown.
     * If not set, or QString() is passed in, the global default tool bar name
     * will be used.
     *
     * @see KisKEditToolBar
     */
    void load(KisKXMLGUIFactory *factory,
              const QString &defaultToolBar = QString());

    /**
     * @internal Reimplemented for internal purposes.
     */
    KisKActionCollection *actionCollection() const override;

    /**
     * Save any changes the user made.  The file will be in the user's
     * local directory (usually $HOME/.kde/share/apps/\<appname\>).  The
     * filename will be the one specified in the constructor.. or the
     * made up one if the filename was NULL.
     *
     */
    void save();

    /**
     * Remove and readd all KMXLGUIClients to update the GUI
     */
    void rebuildKisKXMLGUIClients();

Q_SIGNALS:
    /**
     * Emitted whenever any modifications are made by the user.
     */
    void enableOk(bool);

private:
    Q_PRIVATE_SLOT(d, void slotToolBarSelected(int index))

    Q_PRIVATE_SLOT(d, void slotInactiveSelectionChanged())
    Q_PRIVATE_SLOT(d, void slotActiveSelectionChanged())

    Q_PRIVATE_SLOT(d, void slotInsertButton())
    Q_PRIVATE_SLOT(d, void slotRemoveButton())
    Q_PRIVATE_SLOT(d, void slotUpButton())
    Q_PRIVATE_SLOT(d, void slotDownButton())

    Q_PRIVATE_SLOT(d, void slotDropped(ToolBarListWidget *, int, ToolBarItem *, bool))

private:
    friend class KisKEditToolBarWidgetPrivate;
    KisKEditToolBarWidgetPrivate *const d;

    Q_DISABLE_COPY(KisKEditToolBarWidget)
};

}

#endif

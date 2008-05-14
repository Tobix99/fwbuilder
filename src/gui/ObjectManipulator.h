/* 

                          Firewall Builder

                 Copyright (C) 2003 NetCitadel, LLC

  Author:  Vadim Kurland     vadim@fwbuilder.org

  $Id: ObjectManipulator.h,v 1.58 2007/01/08 02:11:48 vkurland Exp $

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


#ifndef  __OBJECTMANIPULATOR_H_
#define  __OBJECTMANIPULATOR_H_


#include "config.h"
#include "global.h"

#include <qdialog.h>
#include <qtreewidget.h>
#include <qtooltip.h>
#include <QMenu>

#include <ui_objectmanipulator_q.h>

#include "fwbuilder/FWObject.h"
#include "fwbuilder/FWObjectDatabase.h"
#include "fwbuilder/ObjectGroup.h"

#include <stack>
#include <set>

class ObjectTreeView;
class ObjectTreeViewItem;
class QComboBox;
class QMenu;
class ProjectPanel;

namespace libfwbuilder 
{
    class Firewall;
    class Library;
}

class HistoryItem {
    ObjectTreeViewItem *itm;
    QString         objId;

 public:
    HistoryItem(ObjectTreeViewItem *oi,const QString &id) { itm=oi; objId=id; }
    ~HistoryItem();
    ObjectTreeViewItem* item() { return itm;   }
    QString         id()   { return objId; }
};

/*class ObjToolTip : public QToolTip {

    ObjectTreeView *otv;
 public:
    ObjToolTip(ObjectTreeView *widget);
    virtual ~ObjToolTip() {}
    virtual void maybeTip(const QPoint &p);
};*/

class ObjectManipulator : public QWidget/*ObjectManipulator_q*/ {

    Q_OBJECT

    std::vector<libfwbuilder::FWObject*>          idxToLibs;
    std::vector<QTreeWidget*>                       idxToTrees;
    QSet <QString> ids ;     
    std::stack<HistoryItem>                       history;
    int cacheHits;

    libfwbuilder::FWObject                        *currentObj;
    ObjectTreeView                                *current_tree_view;
    
    int                                            treeWidth;
    int                                            treeHeight;

    bool                                           active;
    
/* this is a reverse idex of all objects in all trees. We use it to
 * quickly locate given object in the tree and open it
 */
    std::map<libfwbuilder::FWObject*, ObjectTreeViewItem*> allItems;

    
    ObjectTreeViewItem* insertObject( ObjectTreeViewItem *itm,libfwbuilder::FWObject *obj );
    void insertSubtree( ObjectTreeViewItem *itm,libfwbuilder::FWObject *obj );

    void removeObjectFromTreeView(libfwbuilder::FWObject *obj );

    QString getTreeLabel( libfwbuilder::FWObject *obj );

    void addTreePage(libfwbuilder::FWObject *lib);
    void showObjectInTree(ObjectTreeViewItem *otvi);

    int  getIdxForLib(libfwbuilder::FWObject*);
    void addLib( libfwbuilder::FWObject *lib, QTreeWidget* otv=NULL);
    void removeLib(libfwbuilder::FWObject*);
    void removeLib(int idx);
    void updateCreateObjectMenu(libfwbuilder::FWObject* lib);
    void makeNameUnique(libfwbuilder::FWObject* p,libfwbuilder::FWObject* obj);

    libfwbuilder::FWObject* actuallyCreateObject(libfwbuilder::FWObject *parent,
                                                 const QString &objType,
                                                 const QString &objName,
                                                 libfwbuilder::FWObject *copyFrom=NULL);
    void autorename(libfwbuilder::FWObject *obj,bool ask=true);
    void extractFirewallsFromGroup(
            libfwbuilder::ObjectGroup *gr,
            std::set<libfwbuilder::Firewall*> &fo);

    ProjectPanel *m_project;
public slots:
     virtual void libChanged(int l);
     virtual void switchingTrees(QWidget* w);
     virtual void currentTreePageChanged(int i);

     void selectionChanged(QTreeWidgetItem *cur);

     void info();

     /**
      * open object obj in the editor. Does not open editor panel
      * if it is closed. Asks FWWindow permission to own editor.
      */
     bool switchObjectInEditor(libfwbuilder::FWObject *obj);

     /**
      * same as above but opens editor panel if it is closed. This is
      * an entry point for menu items 'edit', all 'new object' as well
      * as doubleclick
      */
     bool editObject(libfwbuilder::FWObject *obj);

     void editSelectedObject();
     
     void contextMenuRequested(const QPoint &pos);

     
     libfwbuilder::FWObject* createObject(const QString &objType,
                                          const QString &objName,
                                          libfwbuilder::FWObject *copyFrom=NULL);
     
     libfwbuilder::FWObject* createObject(libfwbuilder::FWObject *parent,
                                          const QString &objType,
                                          const QString &objName,
                                          libfwbuilder::FWObject *copyFrom=NULL);
     libfwbuilder::FWObject * copyObj2Tree(const QString &objType, const QString &objName,
         libfwbuilder::FWObject *copyFrom, libfwbuilder::FWObject *parent=NULL, bool ask4Lib=true);

     libfwbuilder::FWObject * copyObjWithDeep(libfwbuilder::FWObject *copyFrom);
     

     void newLibrary();
     void newObject();
     void newFirewall();
     void newHost();
     void newInterface();
     void newNetwork();
     void newNetworkIPv6(); 
     void newAddress();
     void newAddressIPv6();
     void newInterfaceAddress();
     void newPhysicalAddress();
     void newAddressRange();
     void newObjectGroup();
     void newDNSName();
     void newAddressTable();
     
     void newCustom();
     void newIP();
     void newICMP();
     void newTCP();
     void newUDP();
     void newTagService();
     void newServiceGroup();
     
     void newInterval();

     void duplicateObj(QAction*);
     void duplicateObjUnderSameParent();
     void moveObj(QAction*);
     void copyObj();
     void cutObj();
     void pasteObj();
     void deleteObj();
     void dumpObj();
     void compile();
     void install();

     void groupObjects();
     
     void openObject(QTreeWidgetItem *otvi);
     void openObject(libfwbuilder::FWObject *obj);
     void restoreSelection(bool same_widget);

     void find();
     void findObject();
     
     virtual void back();
     virtual void lockObject();
     virtual void unlockObject();
     virtual void simulateInstall();
     virtual void findWhereUsedSlot();

     
 public:
     Ui::ObjectManipulator_q *m_objectManipulator;
    void filterFirewallsFromSelection(
            std::vector<libfwbuilder::FWObject*> &so,
            std::set<libfwbuilder::Firewall*> &fo);
    void autorename(libfwbuilder::FWObject *obj,
                    const std::string &objtype,
                    const std::string &namesuffix);
    
    ObjectManipulator( QWidget *parent);
    ~ObjectManipulator();
    void loadObjects();
    void loadObjects(libfwbuilder::FWObjectDatabase *db);
    void clearObjects();
    
    bool validateDialog();
    void invalidateDialog();

    void reopenCurrentItemParent();

    void openObject(libfwbuilder::FWObject *obj, bool register_in_history);
    void openObject(ObjectTreeViewItem *otvi,    bool register_in_history);

    libfwbuilder::FWObject* duplicateObject(libfwbuilder::FWObject *target,
                                            libfwbuilder::FWObject *obj,
                                            const QString &name = QString::null,
                                            bool  askForAutorename=true);
    void moveObject(libfwbuilder::FWObject *target,
                    libfwbuilder::FWObject *obj);

    void moveObject(const QString &targetLibName,
                    libfwbuilder::FWObject *obj);
    
    libfwbuilder::FWObject* getOpened() { return currentObj; }

    void updateLibColor(libfwbuilder::FWObject *lib);
    void updateLibName(libfwbuilder::FWObject *lib);

    void updateObjName(libfwbuilder::FWObject *obj,
                       const QString &oldName,
                       bool  askForAutorename=true);
    void updateObjName(libfwbuilder::FWObject *obj,
                       const QString &oldName,
                       const QString &oldLabel,
                       bool  askForAutorename=true);
    
    ObjectTreeView* getCurrentObjectTree();
    libfwbuilder::FWObject* getSelectedObject();

    /**
     *  this method opens given library in the tree
     */
    void openLib(libfwbuilder::FWObject *lib);

    /**
     * returns pointer at a library that is currently opened in the tree
     */
    libfwbuilder::FWObject*  getCurrentLib();

    /**
     *  this method makes sure the system library is NOT opened in the
     *  tree. If it is, it switches to the 'User' library. If one of
     *  the user's libraries is already opened, it does nothing.
     */
    void closeSystemLib();

    libfwbuilder::FWObject* pasteTo(libfwbuilder::FWObject *target,
                                    libfwbuilder::FWObject *obj,
                                    bool openobj=true,
                                    bool validateOnly=false, bool renew_id=true);


    void delObj(libfwbuilder::FWObject *obj,bool openobj=true);

    /**
     * select whatever object is current in the tree (used to restore
     * selected state of the tree item after it was unselected)
     */
    void select();

    /**
     * unselect whatever object is currently selected
     */
    void unselect();

    /**
     * returns true if anything is selected in the tree
     */
    bool isSelected();

    /**
     * controls whether "Deleted Objects" library is shown
     */
    void showDeletedObjects(bool f);


    /**
     * get boolean flags that describe state of the menu items.
     * Can be used for both pop-up context menu and the main menu.
     */ 
    void getMenuState(bool haveMoveTargets,
                      bool &dupMenuItem,
                      bool &moveMenuItem,
                      bool &copyMenuItem,
                      bool &pasteMenuItem,
                      bool &delMenuItem,
                      bool &newMenuItem,
                      bool &inDeletedObjects);
    
    void updateLastModifiedTimestampForOneFirewall(libfwbuilder::FWObject *o);
    void updateLastModifiedTimestampForAllFirewalls(libfwbuilder::FWObject *o);   
    void updateLastInstalledTimestamp(libfwbuilder::FWObject *o);
    void updateLastCompiledTimestamp(libfwbuilder::FWObject *o);
    
    std::list<libfwbuilder::Firewall * > findFirewallsForObject(libfwbuilder::FWObject *o);
    void findAllFirewalls (std::list<libfwbuilder::Firewall *> &fws);

    
 signals:
/**
 * the dialog class should have a slot that can load object's data
 * into dialog elements when ObjectManipulator emits this signal
 */
    void loadObject_sign(libfwbuilder::FWObject *);

/**
 * the dialog class should have a slot that can verify data entered by
 * user in the dialog elements when ObjectManipulator emits this
 * signal. The validation result is returned in variable "bool *res"
 */
    void validate_sign(bool *res);

/**
 * the dialog class should have a slot that can verify if the data in
 * dialog has been edited.
 */
    void isChanged_sign(bool *res);

/**
 * the dialog class should have a slot that applies changes made by
 * the user and saves data in the object.
 */
    void applyChanges_sign();

};

#endif

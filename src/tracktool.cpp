#include "tracktool.h"
#include "track.h"
#include "dialogmarker.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QSlider>
#include <QAction>
#include <QMessageBox>

TrackTool::TrackTool(QWidget *parent)
    : QWidget(parent)
{
    vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vLayout);

    toolbarActions = new QToolBar("Track action", this);

    // Actions remove
    toolBtnTrash = new QToolButton(this);
    toolBtnTrash->setText(tr("Remove"));
    toolBtnTrash->setIcon(QIcon("://icon/trash.svg"));
    toolBtnTrash->setPopupMode(QToolButton::InstantPopup);
    actTrashBegin = new QAction(tr("Remove before"), toolBtnTrash);
    actTrashArea = new QAction(tr("Remove area"), toolBtnTrash);
    actTrashEnd = new QAction(tr("Remove after"), toolBtnTrash);
    toolBtnTrash->addAction(actTrashBegin);
    toolBtnTrash->addAction(actTrashArea);
    toolBtnTrash->addAction(actTrashEnd);
    toolbarActions->addWidget(toolBtnTrash);
    // Others actions
    actMute = toolbarActions->addAction(QIcon("://icon/volume-off.svg"), tr("Mute"));
    actCut = toolbarActions->addAction(QIcon("://icon/cut.svg"), tr("Split"));
    actTrim = toolbarActions->addAction(QIcon("://icon/expand.svg"), tr("Trim"));
    // Filters
    toolbarActions->addSeparator(); 
    toolBtnNoise = new QToolButton(this);
    toolBtnNoise->setText(tr("Reduice Noise"));
    toolBtnNoise->setIcon(QIcon("://icon/creative-commons-sampling.svg"));
    toolBtnNoise->setPopupMode(QToolButton::InstantPopup);
    actNoiseLocal = new QAction(tr("Remove noise localy"), toolBtnNoise);
    actNoiseGlobal= new QAction(tr("Remove noise globaly"), toolBtnNoise);
    toolBtnNoise->addAction(actNoiseLocal);
    toolBtnNoise->addAction(actNoiseGlobal);
    toolbarActions->addWidget(toolBtnNoise);
    // Zoom
    toolbarActions->addSeparator();
    actDefaultTrack = toolbarActions->addAction(QIcon("://icon/default_track.svg"), tr("Default track zoom"));
    toolbarActions->addSeparator();
    actSetMarkers = toolbarActions->addAction(QIcon("://icon/map-marker-alt.svg"), tr("Add markers"));

    soundTrack = new Track(this);

    vLayout->addWidget(toolbarActions);
    vLayout->addWidget(soundTrack);

    connect(actDefaultTrack, SIGNAL(triggered(bool)), soundTrack, SLOT(defaultScale()));
    connect(actSetMarkers, SIGNAL(triggered(bool)), this, SLOT(showDialogMarker()));
    connect(toolBtnTrash, SIGNAL(triggered(QAction*)), this, SLOT(emitActionClick(QAction*)));
    connect(toolBtnNoise, SIGNAL(triggered(QAction*)), this, SLOT(emitActionClick(QAction*)));
    connect(toolbarActions, SIGNAL(actionTriggered(QAction*)), this, SLOT(emitActionClick(QAction*)));
    connect(soundTrack, SIGNAL(selectionChanged(Track::SelectionType)), this, SLOT(activePossibleAction(Track::SelectionType)));
}

TrackTool::~TrackTool()
{
    delete actTrashBegin;
    delete actTrashArea;
    delete actTrashEnd;
    delete toolBtnTrash;
    delete actNoiseLocal;
    delete actNoiseGlobal;
    delete toolBtnNoise;
    delete actMute;
    delete actCut;
    delete actTrim;
    delete actDefaultTrack;
    delete actSetMarkers;
    delete toolbarActions;

    delete soundTrack;

    delete vLayout;
}

QToolBar *TrackTool::getToolbar()
{
    return toolbarActions;
}

Track *TrackTool::getTrack()
{
    return soundTrack;
}

void TrackTool::setMedia(int i, Media &media)
{
    Q_UNUSED(i);
    soundTrack->setSource(media);
}

void TrackTool::emitActionClick(QAction *button)
{
    // Vérification de la présence de ffmpeg dans les paramètres
    if (MainWindow::settings->value("General/prog_ffmpeg").toString().isEmpty()) {
        QMessageBox::critical(NULL, tr("Error"), tr("You must have ffmpeg and configure it in setting"), QMessageBox::Ok);
    } else {
        // Récupération de la zone ou marqueur de selection
        QVector<QTime> selected;
        selected.append(soundTrack->getSelectionTime(Track::SelectionX::X1));
        selected.append(soundTrack->getSelectionTime(Track::SelectionX::X2));

        QString sox = MainWindow::settings->value("General/prog_sox").toString();
        // Différentiation des actions globales/locales
        if (button == actNoiseGlobal) {
            if (sox.isEmpty()) {
                QMessageBox::critical(NULL, tr("Error"), tr("You must have SoX and configure it in setting"), QMessageBox::Ok);
            } else {
                emit actionNoiseGlobal(selected);
            }
        } else {
            // Récupération de l'enum correspondant à l'action locale
            Actions::enumActions action = Actions::enumActions::NONE;
            if (button == actTrashArea) {
                action = Actions::enumActions::DELETE_ZONE;
            } else if (button == actTrashBegin) {
                action = Actions::enumActions::DELETE_BEGIN;
            } else if (button == actTrashEnd) {
                action = Actions::enumActions::DELETE_END;
            } else if (button == actMute) {
                action = Actions::enumActions::MUT;
            } else if (button == actCut) {
                action = Actions::enumActions::SPLIT;
            } else if (button == actTrim) {
                action = Actions::enumActions::TRIM;
            } else if (button == actNoiseLocal) {
                if (sox.isEmpty()) {
                    QMessageBox::critical(NULL, tr("Error"), tr("You must have SoX and configure it in setting"), QMessageBox::Ok);
                } else {
                    action = Actions::enumActions::NOISE;
                }
            }
            if (action != Actions::enumActions::NONE) {
                emit actionClick(action,selected);
            }
        }
    }
}

void TrackTool::activePossibleAction(Track::SelectionType stype)
{
    switch (stype) {
        case Track::NONE:
            toolBtnTrash->setEnabled(false);
            actTrashBegin->setEnabled(false);
            actTrashArea->setEnabled(false);
            actTrashEnd->setEnabled(false);
            actMute->setEnabled(false);
            actCut->setEnabled(false);
            actTrim->setEnabled(false);
            break;
        case Track::LINE:
            toolBtnTrash->setEnabled(true);
            actTrashBegin->setEnabled(true);
            actTrashArea->setEnabled(false);
            actTrashEnd->setEnabled(true);
            actMute->setEnabled(false);
            actCut->setEnabled(true);
            actTrim->setEnabled(false);
            break;
        case Track::AREA:
            toolBtnTrash->setEnabled(true);
            actTrashBegin->setEnabled(true);
            actTrashArea->setEnabled(true);
            actTrashEnd->setEnabled(true);
            actMute->setEnabled(true);
            actCut->setEnabled(false);
            actTrim->setEnabled(true);
            break;
        default:
            break;
    }
}

void TrackTool::showDialogMarker()
{
    DialogMarker *dial_marker = new DialogMarker(soundTrack->getMarkers(), this);
    if (dial_marker->exec() == QMessageBox::Accepted){
        auto markers = dial_marker->getMarkers();
        soundTrack->setMarkers(markers);
        emit markerChanged(markers);
    }
    delete dial_marker;
}


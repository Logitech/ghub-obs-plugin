#include "window-dock.hpp"
#include "obs-app.hpp"
#include "window-basic-main.hpp"

#include <QMessageBox>
#include <QCheckBox>

void OBSDock::closeEvent(QCloseEvent *event)
{
	auto msgBox = []() {
		QMessageBox msgbox(App()->GetMainWindow());
		msgbox.setWindowTitle(QTStr("DockCloseWarning.Title"));
		msgbox.setText(QTStr("DockCloseWarning.Text"));
		msgbox.setIcon(QMessageBox::Icon::Information);
		msgbox.addButton(QMessageBox::Ok);

		QCheckBox *cb = new QCheckBox(QTStr("DoNotShowAgain"));
		msgbox.setCheckBox(cb);

		msgbox.exec();

		if (cb->isChecked()) {
			config_set_bool(App()->GlobalConfig(), "General",
					"WarnedAboutClosingDocks", true);
			config_save_safe(App()->GlobalConfig(), "tmp", nullptr);
		}
	};

	bool warned = config_get_bool(App()->GlobalConfig(), "General",
				      "WarnedAboutClosingDocks");
	if (!OBSBasic::Get()->Closing() && !warned) {
		QMetaObject::invokeMethod(App(), "Exec", Qt::QueuedConnection,
					  Q_ARG(VoidFunc, msgBox));
	}

	QDockWidget::closeEvent(event);

	if (widget() && event->isAccepted()) {
		QEvent widgetEvent(QEvent::Type(QEvent::User + QEvent::Close));
		qApp->sendEvent(widget(), &widgetEvent);
	}
}

void OBSDock::showEvent(QShowEvent *event)
{
	QDockWidget::showEvent(event);
}

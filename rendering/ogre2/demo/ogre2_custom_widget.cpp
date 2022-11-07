#include "ogre2_custom_widget.h"

namespace tesseract_gui
{
Ogre2CustomWidget::Ogre2CustomWidget(QWidget* parent) : QWidget(parent)
{
  setWindowTitle("Some Window");
  setMinimumSize(100, 100);
  resize(1024, 768);

  mainLayout = new QVBoxLayout();

  w = new Ogre2Viewport;
  Ogre2Manager::instance()->createNewScene();
  w2 = new Ogre2Viewport(1);

  mainLayout->addWidget(w, 0);
  mainLayout->addWidget(w2, 0);

  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setMargin(0);
  //    mainLayout->setSpacing(0);

  setLayout(mainLayout);
}
}  // namespace tesseract_gui

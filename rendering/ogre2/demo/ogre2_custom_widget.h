#ifndef TESSERACT_QT_RENDERING_OGRE2_OGRE2_CUSTOM_WIDGET_H
#define TESSERACT_QT_RENDERING_OGRE2_OGRE2_CUSTOM_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <tesseract_qt/rendering/ogre2/ogre2_viewport.h>

namespace tesseract_gui
{
class Ogre2CustomWidget : public QWidget
{
  Q_OBJECT
public:
  explicit Ogre2CustomWidget(QWidget* parent = 0);

  Ogre2Viewport *w, *w2;

private:
  QVBoxLayout* mainLayout;
};

}  // namespace tesseract_gui
#endif  // TESSERACT_QT_RENDERING_OGRE2_OGRE2_CUSTOM_WIDGET_H

#include "progressbardelegate.h"
#include <QApplication>

ProgressBarDelegate::ProgressBarDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 QWidget *ProgressBarDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QProgressBar *editor = new QProgressBar(parent);
     editor->setValue (0);
     return editor;
 }

 void ProgressBarDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     int value = index.model()->data(index, Qt::EditRole).toInt();

     QProgressBar *progress = static_cast<QProgressBar*>(editor);

     progress->setValue(value);
 }

 void ProgressBarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QProgressBar *progress = static_cast<QProgressBar*>(editor);
     int value = progress->value();

     model->setData(index, value, Qt::EditRole);
 }

 void ProgressBarDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

 void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
         const QModelIndex & index ) const
     {
      int value = index.data().toInt();
      int nLeft = 4;
      int nTop = 4;
      int nWidth = option.rect.width() - 2 * nLeft;
      int nHeight = option.rect.height() - 2 * nTop;

      // 设置进度条的风格
      QStyleOptionProgressBar progressBarOption;
      progressBarOption.initFrom(option.widget);
      // 设置进度条显示的区域
      progressBarOption.rect = QRect(option.rect.left() + nLeft, option.rect.top() + nTop,  nWidth, nHeight);
      // 设置最小值
      progressBarOption.minimum = 0;
      // 设置最大值
      progressBarOption.maximum = 100;
      // 设置对齐方式
      progressBarOption.textAlignment = Qt::AlignCenter;
      // 设置进度
      progressBarOption.progress = value;
      if(value==0)
      {
       progressBarOption.text = QString(tr("就绪"));
      }else if(value == -1)
      {
        progressBarOption.text = QString("升级失败");
      }else if(value == -2)
      {
        progressBarOption.text = QString("开始升级");
      }else if(value == -3)
      {
        progressBarOption.text = QString("等待升级");
      }else if(value == 100)
      {
        progressBarOption.text = QString("升级成功");
      }else
      {
      // 设置文本（百分比）
      progressBarOption.text = QString("%1%").arg(value);
      }
      // 设置文本可见
      progressBarOption.textVisible = true;

      QProgressBar progressBar;

      //绘制进度条
      QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter, &progressBar);
     }

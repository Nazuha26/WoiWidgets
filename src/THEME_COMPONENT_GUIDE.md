Инструкция: как добавить новый компонент, поддерживающий темы
==============================================================

### Создать класс-компонент:

```cpp 
class WoiFancyWidget : public WoiThemeWidget {
    Q_OBJECT
    
    public:
        /* --- ALL GETTERS --- */ для всех параметров
        ...
        ...
        
        /* --- SETTERS --- */   только для тех которые не связанны с темой
        ...
        ...
        
        explicit WoiFancyWidget(QWidget* parent = nullptr);
    
    protected:
        void applyTheme(const WoiTheme& theme) override;   // ОБЯЗАТЕЛЬНО
        
    private:
        /* --- STYLE PARAMS --- */
        ...
        ...
        
        void applyStyle(const WoiTheme::FancyStyle& style);     // ЖЕЛАТЕЛЬНО
        void syncStyle();                             // ЖЕЛАТЕЛЬНО
};
```

### Определить, какие параметры считаем «графическими»
   - **Цвета, QBrush, градиенты** → войдут в новый FancyStyle **(только геттеры)**
   - **Геометрия, размеры, числовые настройки** → останутся полями класса **(геттеры + сеттеры)**.

### Расширить структуру ```WoiThemeEngine::WoiTheme```:

```cpp
struct FancyStyle {
    QColor bgColor;             // e.g.
    QColor borderColor;         // e.g.
    QBrush highlightBrush;      // e.g.
} fancy;   // добавить в WoiTheme
```

### Реализовать методы в файле *.cpp компонента:
```cpp
/* ----- THEME MANAGEMENT ----- */
void WoiFancyWidget::applyTheme(const WoiTheme& theme)
{ applyStyle(theme.fancy); }

void WoiFancyWidget::applyStyle(const WoiTheme::FancyStyle& style)
{
    /* --- colors --- */
    bgColor_      = style.bgColor;
    borderColor_  = style.borderColor;
    highlight_    = style.highlightBrush;
    
    update();
}

void WoiFancyWidget::syncStyle()
{ applyStyle(ThemeEngine->currentTheme().fancy); }
/* ----- THEME MANAGEMENT ----- */
```

### В конструкторе компонента вызвать ```syncStyle()```:
- это сразу подтянет активную тему при создании виджета.

### Геттеры/сеттеры:
- **Геттеры** для **ВСЕХ** параметров стиля (чтение текущих значений).
- **Сеттеры** только для геометрии или параметров, не связанных с цветами.

### Наследование от WoiThemeWidget:
- компонент автоматически подписан на сигнал themeChanged.
- при смене темы applyTheme() вызовется сам.

### DONE

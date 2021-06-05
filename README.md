# Game-of-Life

При запуске програмы пользователю предоставляется возможноть выбрать настройки импорта игрового поля, экспорта игры, а также
настроить правила, по которым идет игра.

Импорт осуществляется из одного из трех источников:
1. Игровое поле генерируется прямо в программе. На поле указанного размера каждая клетка имеет определенный шанс оказаться
живой или мертвой.
2. Игровое поле считывается из файла input.txt с закодированным полем. Первая строка файла содержит размерность поля N, а последующие
N строк длины N содержат само поле, где пробел - это мертвая клетка, а любой другой символ - живая.
3. Игровое поле считывается из квадратной картинки input.bmp с 24-разрядным цветом без сжатия. Белые пиксели на картинке 
соответствуют живым клеткам, а черные - мертвым.

Экспорт может осуществляться тремя способами, из которых можно выбрать один, два или все. Способы экспорта:
1. Вывод игрового поля на консоль. Символ 'X' соответствует живой клетке, а пробел - мертвой. В случае,
если размерность поля слишком высока, вывод поля на консоль может осуществляться некорректно.
  Рекомендуемый размер поля: до 25, ограничен размером консоли.
2. Вывод в текстовый файл. В файл поочередно записываются все состояния игрового поля, разделенные ограничителями.
Символ 'X' соответствует живой клетке, а пробел - мертвой. Перед каждым состоянием указано, на каком шагу оно было получено.
  Рекомендуемый размер поля: до 100, ограничен размером экрана.
3. Вывод в серию квадратных картинок формата BMP, например, созданных при помощи программы Paint. 
На картинках белые пиксели соответствуют живым клеткам, а черные - мертвым.
В названии каждой картинки указывается шаг, на котором было получено данное состояние.
  Рекомендуемый размер поля: до 2000, ограничен разрешением экрана. При размере поля более 5000 вывод заметно замедляется.

Настройки игры. После выбора способов импорта и экспорта игрового поля начинается основной цикл игры. На одном шаге цикла 
живые клетки могут продолжить жить или умереть, а мертвые клетки остаться мертвыми или ожить. Решение это принимается на основании
количества соседей клетки. Игра продолжается до тех пор, пока игровое поле не станет статичным, либо до определенного количества шагов. 
Пользователь может изменить следующие параметры:
1. В случае генерации поля внутри программы можно выбрать размер поля и его заполненность в начале симуляции. В иных случаях размер
поля выставляется автоматически при импорте.
2. Диапазон количества соседей клетки, при котором мертвая клетка оживет. Вне этого диапазона клетка останется мертвой.
3. Диапазон количества соседей клетки, при котором живая клетка продолжит жить. Вне этого диапазона клетка умрет.
4. Является ли поле бесшовным. На бесшовном поле крайние клетки с разных сторон поля считаются соседями. 
В ином случае, все клетки за пределами поля считаются мертвыми.
5. Количество шагов симуляции. В случае, если игра не окончится ранее, симуляция будет продолжаться до указанного количества шагов.

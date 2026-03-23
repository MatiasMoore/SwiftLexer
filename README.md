# SwiftLexer — Компилятор Swift для JVM

Компилятор с поддержкой основных возможностей языка **Swift**, написанный на **C++** с использованием [Flex](https://github.com/westes/flex) (лексер) и [Bison](https://www.gnu.org/software/bison/) (парсер). Принимает на вход Swift-файл и компилирует его в **байткод JVM** (`.class` файлы), которые затем запускаются стандартной виртуальной машиной Java.

---

## Содержание

- [Обзор](#обзор)
- [Реализованные возможности](#реализованные-возможности)
- [Архитектура](#архитектура)
- [Требования](#требования)
- [Сборка](#сборка)
- [Использование](#использование)
- [Примеры программ](#примеры-программ)
- [Структура проекта](#структура-проекта)

---

## Обзор

SwiftLexer — учебный компилятор, транслирующий Swift-подобный исходный код в файлы `.class`, совместимые с JVM. Результат помещается в папку `out/` и запускается через `java`. Компилятор поставляется с небольшой **RTL-библиотекой** (Runtime Library), реализованной на Java, которая предоставляет базовые типы и операции.

### Конвейер компиляции

```
Исходный файл Swift (.txt / .swift)
        │
        ▼
  ┌───────────┐
  │   Лексер  │  win_flex  →  swift.l
  └───────────┘
        │  поток токенов
        ▼
  ┌───────────┐
  │  Парсер   │  win_bison  →  swift.y
  └───────────┘
        │  AST (дерево разбора)
        ▼
  ┌──────────────────┐
  │ Трансформация    │  Оборачивает свободные выражения в $MainClass
  └──────────────────┘
        │
        ▼
  ┌──────────────────┐
  │ Семантический    │  Таблицы символов, проверка типов, области видимости
  │ анализ           │
  └──────────────────┘
        │
        ▼
  ┌──────────────────┐
  │ Генерация кода   │  Запись байткода JVM в .class файлы
  └──────────────────┘
        │
        ▼
   out/*.class  →  java -noverify -cp out $MainClass
```

---

## Реализованные возможности

### Локальные переменные и встроенные типы данных

Поддерживаются `var` и `let`-объявления с выводом типа. Доступны все числовые типы Swift:

```swift
var a: Int = 42
var b: Float = 3.14
var c: String = "Привет"
var ch: Character = "A"
```

Числовые и строковые типы: `Int`, `Int8`, `Int16`, `Int32`, `Int64`, `UInt`, `UInt8`, `UInt16`, `UInt32`, `UInt64`, `Float`, `Float80`, `Double`, `Bool`, `String`, `Character`.

---

### Числовые, символьные и строковые литералы

Полная поддержка всех форм литералов, включая служебные последовательности и Unicode:

```swift
var dec  = 1_000_000          // десятичный, с разделителем
var bin  = 0b1010_1010        // двоичный
var oct  = 0o755              // восьмеричный
var hex  = 0xFF_A0            // шестнадцатеричный
var flt  = 1.5e-3             // вещественный (десятичный)
var hflt = 0x1.8p1            // вещественный (шестнадцатеричный)
var s    = "Hello\nWorld"     // строка с escape-последовательностью
var u    = "\u{1F600}"        // Unicode-символ
var ml   = """
    Многострочная
    строка
    """
```

---

### Арифметические, сравнительные и составные операции

```swift
var x = 10 + 3 * 2 - 8 / 4    // арифметика
var eq = x == 8                 // сравнение
x += 5
x -= 2
x *= 3
x /= 6
x %= 4
```

---

### Операции с числами с плавающей точкой

```swift
var a: Float = 5.0
var b = a + 0.155
var c: Double = 9.157 + b
```

---

### Логические операции

```swift
var conditionA = true
var conditionB = false
var result = conditionA && conditionB   // И
result = conditionA || conditionB       // ИЛИ
result = !conditionA                    // НЕ
```

---

### Операции над строками

```swift
var str1 = "Hello"
var str2 = " world!"
if str1.hasPrefix("He") && str2.hasSuffix("rld!") && !str1.isEmpty {
    print(str1 + str2)
}
```

---

### Одномерные массивы и диапазоны

```swift
var arr = [1, 2, 3, 4, 5]         // массив-литерал
var r1 = 1...5                     // закрытый диапазон
var r2 = 1..<6                     // полуоткрытый диапазон

arr[0] = 10                        // доступ по индексу
print(arr.count)                   // свойство count
```

---

### Многомерные массивы

```swift
var matrix = [[1, 2, 3], [4, 5, 6]]
var elem = matrix[0][1]             // элемент матрицы
```

---

### Управляющие структуры: ветвления

```swift
if condition {
    // ...
} else if otherCondition {
    // ...
} else {
    // ...
}
```

---

### Управляющие структуры: циклы

```swift
for elem in array {               // for-in по массиву/диапазону
    print(elem)
}

while condition {                 // while
    // ...
}

repeat {                          // repeat-while
    // ...
} while condition
```

Поддерживаются `break`, `continue`, `fallthrough`.

---

### Функции

Функции с именованными и безымянными параметрами, возвращаемыми типами и рекурсией:

```swift
func fibonacci(_ n: Int) -> Int {
    if n <= 1 { return n }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

func greet(name: String) -> String {
    return "Hello, " + name
}
```

---

### Перегрузка функций по типу параметров

```swift
func myFunc(a: Int) {
    print("Int: " + a)
}

func myFunc(a: Float) {
    print("Float: " + a)
}
```

---

### Ввод и вывод

```swift
print("Введите число:")
var n = readLine().toInt()
print("Вы ввели: " + n)
```

---

### Классы, свойства, наследование и динамическое связывание

```swift
class Animal {
    public var name: String

    public init(name: String) {
        self.name = name
    }

    public func speak() {
        print(name + " говорит")
    }
}

class Dog: Animal {
    override public func speak() {
        print(self.name + " лает!")
    }
}

var d = Dog(name: "Рекс")
d.speak()   // вызов через динамическое связывание
```

---

### Статические свойства и методы

```swift
class Counter {
    static var _count = 0

    static func increment() {
        Counter._count = Counter._count + 1
    }
}

Counter.increment()
print(Counter._count)
```

---

### Контроль доступа

```swift
class Secure {
    public var openField: Int
    private var secret: Int
    internal var packageVisible: Int
}
```

Поддерживаются модификаторы: `public`, `private`, `internal`, `fileprivate`, `open`.

---

### Конструкторы и деструкторы

```swift
class Point {
    public var x: Int
    public var y: Int

    public init(x: Int, y: Int) {
        self.x = x
        self.y = y
    }

    deinit {
        // освобождение ресурсов
    }
}

var p = Point(x: 1, y: 2)
```

---

### Перегрузка операций

```swift
class Vector {
    public var x: Float
    public var y: Float

    public init(_ x: Float, _ y: Float) {
        self.x = x
        self.y = y
    }

    public static func +(_ vec1: Vector, _ vec2: Vector) -> Vector {
        return Vector(vec1.x + vec2.x, vec1.y + vec2.y)
    }
}

var v1 = Vector(1.0, 2.0)
var v2 = Vector(3.0, 4.0)
var v3 = v1 + v2
```

---

## Архитектура

### Компоненты

| Компонент | Расположение | Описание |
|---|---|---|
| Лексер | `swift.l` | Flex-лексер: 5 состояний (`INITIAL`, `SUBSCRIPT`, `STRING`, `STRING_MULTILINE`, `COMMENT_MULTILINE`) |
| Парсер | `swift.y` | Bison LALR(1) грамматика, строит AST |
| Узлы AST | `nodes/` | Пара `.h`/`.cpp` на каждую языковую конструкцию |
| Семантика | `nodes/*.cpp` (`fillTable`, `semanticsTransform`) | Интегрирована в узлы |
| Генерация кода | `generation/` | Запись сырого байткода JVM |
| RTL | `rtl/*.java` | Java-рантайм: `Integer`, `Float`, `Boolean`, `String`, `Array`, `Range`, `InputOutput`, `Operators` |
| Таблица символов | `tables/` | Классы, методы, переменные |
| Трансформация дерева | `NodeTreeTransform.cpp` | Оборачивает глобальные выражения в `$MainClass.main()` |
| Визуализация AST | `nodes/dotHelpers.cpp` | Генерирует Graphviz `.dot` |
| Парсер .class | `ClassfileParser.cpp` | Читает скомпилированные RTL-файлы и регистрирует их в таблице символов |

### RTL-библиотека

| Класс | Назначение |
|---|---|
| `rtl/Integer` | Обёртка над целыми числами |
| `rtl/Float` | Обёртка над числами с плавающей точкой |
| `rtl/Boolean` | Булев тип |
| `rtl/String` | Строки и конкатенация |
| `rtl/Array` | Динамический массив со свойствами `count` и `isEmpty` |
| `rtl/Range` | Диапазоны `0..<n` и `0...n` |
| `rtl/InputOutput` | `print()` и `readLine()` |
| `rtl/Operators` | Арифметика, сравнение, логические операции |

---

## Требования

| Зависимость | Версия | Примечание |
|---|---|---|
| **MSVC** (Visual Studio) | 2019 или 2022 | Файл проекта включён (`SwiftLex.vcxproj`) |
| **Java JDK** | 8+ | `javac` и `java` должны быть в `PATH` |
| **win_flex** | встроен | `SwiftLex/win_flex.exe` |
| **win_bison** | встроен | `SwiftLex/win_bison.exe` |
| **Graphviz** | опционально | Поместите `dot.exe` в `SwiftLex/Graphviz/bin/dot.exe` |


---

## Сборка

1. Откройте `SwiftLex.sln` в **Visual Studio**.
2. Система сборки автоматически вызовет `win_flex` и `win_bison` как pre-build шаги, сгенерировав `swift.tab.cpp`/`.h` и `lex.yy.c`.
3. Соберите решение (**Release** или **Debug**).

> В режиме **Debug** компилятор автоматически читает `SwiftLex/input.txt`.

---

## Использование

```
SwiftLex.exe <путь_к_файлу>
```

### Результаты

- **`out/`** — скомпилированные `.class` файлы (пересоздаются при каждом запуске)
- **`swift.png`** — визуализация AST до семантики (при включённом `_DRAW_DOT`)
- **`swiftSem.png`** — визуализация AST после семантики

### Флаги компилятора (`GlobalSettings.cpp`)

| Флаг | Описание |
|---|---|
| `_DRAW_DOT` | Генерировать и открывать Graphviz-диаграммы AST |
| `_EXEC_MAINCLASS` | Запустить скомпилированную программу сразу после генерации |
| `_IMPLICIT_CONSTRUCTORS` | Включить генерацию неявных конструкторов |

---

## Примеры программ

### Числа Фибоначчи

```swift
func fibonacci(_ n: Int) -> Int {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

let number = 10
print("Fibonacci for")
print(number)
print(fibonacci(number))
```

### Сортировка пузырьком

```swift
print("How many elements will be in your array?")
var n = readLine().toInt()
var arr = 0..<n

for index in 0..<arr.count {
    arr[index] = readLine().toInt()
}

for i in 0..<n {
    for j in 0..<n - i - 1 {
        if arr[j] > arr[j + 1] {
            let temp = arr[j]
            arr[j] = arr[j + 1]
            arr[j + 1] = temp
        }
    }
}

var arrStr = ""
for num in arr {
    arrStr = arrStr + " " + num
}
print("Sorted array:")
print(arrStr)
```

Дополнительные примеры (корректные и некорректные) находятся в `SwiftLex/tests/`.

---

## Структура проекта

```
SwiftLexer/
├── SwiftLex.sln                   # Visual Studio решение
└── SwiftLex/
    ├── swift.l                    # Flex-лексер
    ├── swift.y                    # Bison-грамматика
    ├── main.cpp                   # Точка входа компилятора
    ├── GlobalSettings.{h,cpp}     # Глобальные флаги компилятора
    ├── NodeTreeTransform.{h,cpp}  # Проход трансформации AST
    ├── RTLHelper.h                # Константы имён RTL-классов и методов
    ├── ClassfileParser.{h,cpp}    # Читатель .class файлов для RTL
    ├── allNodes.h                 # Агрегированные включения узлов
    ├── input.txt                  # Отладочный вход (сортировка пузырьком)
    ├── win_flex.exe               # Flex для Windows (встроен)
    ├── win_bison.exe              # Bison для Windows (встроен)
    ├── nodes/                     # Реализации узлов AST
    │   ├── ExprNode.{h,cpp}       # Выражения
    │   ├── StmtNode.{h,cpp}       # Операторы
    │   ├── FuncDeclNode.{h,cpp}   # Объявления функций
    │   ├── FuncCallNode.{h,cpp}   # Вызовы функций
    │   ├── VarDeclarationNode.{h,cpp}
    │   ├── ClassDeclNode.{h,cpp}
    │   ├── IfElseNode.{h,cpp}
    │   ├── LoopNode.{h,cpp}
    │   └── ...                    # Все остальные узлы
    ├── generation/
    │   ├── classFileGeneration.{h,cpp}   # Запись .class файлов
    │   └── generationHelpers.{h,cpp}     # Вспомогательные JVM-функции
    ├── tables/                    # Таблицы символов
    ├── rtl/                       # Java RTL-библиотека
    │   ├── InputOutput.java
    │   ├── Operators.java
    │   ├── Integer.java
    │   ├── Float.java
    │   ├── Boolean.java
    │   ├── String.java
    │   ├── Array.java
    │   └── Range.java
    ├── tests/
    │   ├── correct/               # Корректные Swift-программы (тесты)
    │   └── incorrect/             # Программы с ожидаемыми ошибками
    └── Graphviz/                  # (опционально) Бинарники Graphviz
```


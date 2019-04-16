/**
 * @ru
 * @mainpage Программный интерфейс процессора с набором команд операций дискретной математики
 * @endru
 */

/**
 * @ru
 * @file rus-doxy.hpp
 * @author Dubrovin Egor <dubrovin.en@ya.ru>
 * @brief Описание документации на русском языке
 * @endru
 */

/**
 * @ru
 * @file key.hpp
 * @author Dubrovin Egor <dubrovin.en@ya.ru>
 * @brief Все классы и структуры для создания битового ключа
 * @endru
 */

/**
 * @ru
 * @file declaration.hpp
 * @author Dubrovin Egor <dubrovin.en@ya.ru>
 * @brief Описание макросами типов и команд операций СП
 * @endru
 */

/**
* @ru
* @def SPU_INT_TYPE
* 
* Макрос, определяющий во время компиляции тип, к которому приводится \ref uint
* 
* Например
* @code{.sh}
* g++ -DSPU_INT_TYPE=long
* @endcode
* 
* @endru
*/

/* From here all of the declorations must be with all namespaces */
/**
 * @ru
 * @namespace SPU
 * 
 * Общее пространство имён для описания классов программного интерфейса процессора с набором команд операций дискретной математики
 * 
 * Включает в себя: <br>
 * \ref SPU_INT_TYPE <br>
 * \ref uint <br>
 * \ref Key <br>
 * \ref AnyType <br>
 * 
 * @endru
 */

/**
 * @ru
 * @typedef SPU::uint
 * 
 * Беззнаковый целый тип, представляющий битовую последовательность данных.
 * 
 * Используется для конструирования \ref Key - ключа как опорный тип битовой последовательности.
 * 
 * @endru
 */

/**
 * @ru
 * @class SPU::Key key.hpp
 * 
 * @brief Этот класс предназначен для собирания ключа по полям, определяемыми записями вида Название -> Значение.
 * 
 * Конструктор класса требует определения длинн полей в битах в ключе.
 * 
 * Для начала работы требуется определить тип переменной название FieldNameType.
 * Например, вот создание и получение ключа для двух полей со строковым определением FieldNameType.
 * @code{.cpp}
 * // Конструирование ключа c Названием типа std::string
 * Key<std::string> key_provider({
 *   {"a", 16}, // Поле "a" имеет длину 16 бит
 *   {"b", 5}   // Поле "b" имеет длину 5 бит
 * });
 * 
 * uint key = key_provider.constructKey({
 *   {"a", 2}, // В шестнадцатеричном представлении 0x2
 *   {"b", -1} // В шестнадцатеричном представлении 0xFFFFFFFF
 * });
 * 
 * std::cout << key; // Ответ в битовом представлении 00000000000111110000000000000010
 * @endcode
 * 
 * В качестве аргумента конструктора класса Key используется объект типа FiledsLengthVector.
 * Это переопределение std::vector<FiledLengthStruct>, где в свою очередь SPU::Key::FiledLengthStruct такая структура.
 * @code{.cpp}
 * struct FieldLengthStruct {
 *   FieldNameType name;
 *   uint length;
 * };
 * @endcode
 * 
 * В качестве аргумента метода constructKey используется объект SPU::Key::FieldsDataVector.
 * Переопределение std::vector<FiledLengthStruct>, где
 * @code{.cpp}
 * struct FieldLengthStruct {
 *   FieldNameType name;
 *   AnyType data;
 * };
 * @endcode
 * 
 * @ref AnyType предназначен для конвертации любого базового типа C++ в битовую последовательность беззнакового числа uint.
 * 
 * В примере выше @ref AnyType принимает поле data как тип int и конвертирует их в тип unsigned int.
 * 
 * @endru
 */

/**
 * @ru
 * @class SPU::Key::AnyType key.hpp
 * 
 * Этот класс создан для того, чтобы преобразовывать любой <a href="https://ru.cppreference.com/w/cpp/types">стандартный тип C++</a> в тип \ref uint.
 * 
 * Пример использования
 * @code{.cpp}
 * AnyType(0);
 * AnyType(1);
 * AnyType(-1);
 * AnyType(100);
 * AnyType atp =  AnyType((long) -1);
 * uint stpsu = atp;
 * @endcode
 * 
 * @endru
 */

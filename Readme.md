# RU-ru

## Драйвер процессора обработки структур (СП) ОС Linux

Драйвер предоставляет интерфес для выполнения команд СП на основе передачи и чтения данных символьного файла `/dev/spu`.

## Little-endian формат расположения байт данных СП

Настоящий драйвер (файл `source/spu.h`) не решает целиком проблему совместимости Little-endian Big-endian форматов данных! По-умолчанию предполагается исполнения кода на Little-endian ЭВМ. Формат значения и ключа СП следующий:

* разрядность СП определятся флагом компиляции; минимально поддерживаемая 32, максимальная - 256
* данные ключа и значения задаются как массив 32 разрядных Little-endian беззнаковых чисел
* дравер распределят и записывает значения по 32 разрядным регистрам СП

## Все возможные поля формата передачи данных драйверу СП (файл `source/spu.h`)

* gsid - GSID - глобальный идентификатор структуры, см `struct gsid_container`
* cmd - передаваемая команда и флаги выполнения, см. `enum cmd` и `enum cmd_flags`
* key, val - ключ и значение записи в структуре СП, см. `struct gsid_container`
* rslt - результат выполнения команды, см. `enum rslt`
* power - 32 число - мощность структуры

## Сбор и использование драйвера

По умолчанию сбор производится для МП Baikal для проведения удалённой отладки. См. `Makefile` для подробностей. Сценарии `cp_images_to_srv.sh` и `help_srv.sh` используются в цели *srv-cp*. После сборки цели *default* файл `spudrv.ko` будет находится в директории `source`.

## Пример программы управления СП

```C
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define SPU64 // Лучше задавать при компиляции флаг -DSPU64

#include <spu.h>

/* Код ниже не целиком соответствует C99 */
int main()
{
  int descriptor = open("/dev/" SPU_CDEV_NAME, O_RDWR);
  void *buf;

  /* Создадим структуру */
  adds_cmd_t adds =
  {
    .cmd = ADDS | P_FLAG // Флаг P означает ожидание окончания операции и получение результата
  };

  buf = &adds;
  write(descriptor, buf, sizeof(adds));

  adds_rslt_t adds_rslt = *buf; // В буфер будет записан результат операции

  gsid_t gsid = {0};
  if (adds_rslt.rslt == OK)
  {
    gsid = adds_rslt.gsid;
  }

  printf("Structure GSID is" GSID_FORMAT "\n", gsid);

  /* Запишем значение 1 по ключу 1 */
  /* Помните, что порядок байт и 32 разрядных слов Little-endian */
  spu_key_t key = { 1, 0 };
  val_t     val = { 1, 0 };

  ins_cmd_t ins =
  {
    .cmd  = INS,
    .gsid = gsid,
    .key  = key,
    .val  = val
  };

  buf = &ins;
  write(descriptor, buf, sizeof(ins)); // Флаг P не указан, результат не будет получен

  /* Получим минимальное значение структуры */
  min_cmd_t min =
  {
    .cmd  = MIN | P_FLAG,
    .gsid = gsid
  };

  buf = &min;
  write(descriptor, buf, sizeof(min));

  min_rslt_t min_rslt = *buf;

  printf("Min is %08x-%08x : %08x-%08x", min_rslt.key.cont[1], min_rslt.key.cont[0], min_rslt.val.cont[1], min_rslt.val.cont[0]);

  close(descriptor);
}
```
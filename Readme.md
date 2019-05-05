# RU-ru
## Материалы по установке SDK Baikal находятся [тут](https://www.baikalelectronics.ru/products/T1/)
## Инструкция по подключению:
1. Собрать [образы ядра драйвера leonhard](https://www.baikalelectronics.ru/upload/Stuff/Baikal_Electronics-SDK_seminar-221118%20%281%29.pdf).
2. Запустить скрипт $BAIKAL_SDK_PATH/usr/scripts/build-boot-img.sh bfkx -a
3. Скачать [API Level0](https://bmstu.codes/twobrowin/spu-api).
4. Изменить в скрипте cp_images_to_srv.sh путь к Baikal SDK и имя пользователя на сервере. Запустить скрипт cp_images_to_srv.sh
5. Подключиться к Baikal с сервера 195.19.32.95 (доступ по ssh доступен всем).
6. На сервере понадобятся две консоли: /dev/ttyACM0 (управляющая) и /dev/ttyUSB0 (linux shell).
7. Подключиться к управляющей консоли: minicom -D /dev/ttyACM0
8. Запустить Baikal систему: power on
9. В консоли /dev/ttyUSB0 выбрать пункт 7 меню (загрузка по tftp). В итоге ваш кастомный образ из папки /tftp/ будет загружен в систему.
10. После загрузки Linux копировать выданные скриптом cp_images_to_srv.sh в консоль Baikal shell (minicom -D /dev/ttyUSB0). Запустить команды. В итогу в вашей ~/ директории будут записаны драйвер leonhard, и текстовый код. Далее можно начать тестирование вашего приложения.
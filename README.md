# Лабораторная работа по "инженерному проекту"
### Вариант 11
-------------
> **Важно:** Код целиком ни разу не тестировался )))
> 
> При обнаружении багов или полной неработоспособности кода, просьба обратиться к автору ). Ошибки будут исправлены.



#### Описание:
 Вся суть 1 лабораторной работы заключается в создании обычного http сервера на Arduino ® в связке с Ethernet - шилдом. 
 > **Note:** Никакого отношения эта лаба к ThingWorx ®  не имеет. 

За основу был взят код из стандартных примеров к библиотеке Ethernet. Код был дополнен html страничкой с кнопкой, чтением значения аналогового входа, включением светодиода и выводом информации на дисплей 16x2. 

##### Результат:
Система сама подключается к сети и генерирует свой собственный IP. При входе на него, появляется HTML страничка с кнопкой "START". При нажатии на неё, сервер перенаправляет на адрес IP/start и проиходит вывод значений датчика каждые 5 секунд (время обновления страницы, в заголовке HTML)

# overchat
Кроссплатформенный мессенджер. Backend - Firebase + Google OAuth 2.0.

## Особенности:
- Взаимодействие с Firebase, основанное на REST API, реализовано на C++.
- Аутенитификация реализована на JS, где обрабатываются результаты запросов в веб-браузере.
- Загрузка изображений доступна только на декстопе.

## Каталоги:
- `/components` - компоненты интерфейса + обёртки классов для взаимодействия с Firebase
  - `/db` - работа с локальным хранилищем
  - `/elements` - универсальные элементы
  - `/essentials` - вспомогательные функции общего назначения
  - `/forms` - формы ввода информации
  - `/models` - модели (обёртки классов C++)
  - `/singletons` - модели-одиночки, по одной на инстанс приложения
- `/firebase` - классы для взаимодействия с Firebase
- `/ios` - обёртки компонентов Objective-C под iOS
- `/listobjects` - классы для интеграции списковых элементов с компонентами QML
- `/specials` - файлы, содержащие конфигурации компонентов Firebase: Remote Database и Storage.

## TO-DO:
- Прикрепление файлов с iOS

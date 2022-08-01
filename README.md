## Учебный проект в "Школе 21" - вебсервер на C++

_В процессе, осталось прикрутить запуск CGI скриптов_

Работает с несколькими соединениями через poll, умеет отвечать на GET и POST запросы, загружать статические страницы и файлы.

Настраивается конфиг-файлом, обрабатываются такие настройки:
- Хост и порт
- Главная страница
- Корневая директория
- Принимаемый метод
- Папка для загрузки файлов
- Страницы ошибок
- Максимальный размер файла от клиента
- Редиректы
- Автоиндекс (вместо страницы показывать список файлов в директории)

Для поддиректорий настройки можно переопределять

Например такой конфиг файл запустит сервер на три порта, в первом настроены директории с редиректом, с листингом файлов и тд

```
 server {
	listen 127.0.0.1:7070
	index index.html
	root res/
	upload_path /upload/
	client_max_body_size 80
	methods GET POST
	
	location /tools/
	{
		root tools/
		index tools.html
		autoindex off
		client_max_body_size 120
		error_page 404 /errors/404.html
		error_page 405 /errors/405.html
		methods GET
	}

	location /redirect/
	{
		redirection https://yandex.ru 301
	}

	location /list/
	{
		root list/
		autoindex on
	}
}

server {
	listen 127.0.0.1:8080;
	upload_path /upload/
	root res/
	index index.html
	methods GET
}

server {
	listen 127.0.0.1:9090
	root res/
	index index.html
}
```

Скомпилировать программу: `make`. Запустить: `./webserver <конфиг файл>`


## Part 1. Установка ОС
![Cat-etc-issue](screens/Part 1 - cat-etc-issue.png)


## Part 2. Создание пользователя
- root-права: sudo -i
- Добавление юзера: adduser %name%
- Выдача прав: usermod -aG adm %name%

![adduser](screens/Part 2 - adduser.png)

- Сменить юзера: su %name%

![cat-etc-passwd](screens/Part 2 - cat-etc-passwd.png)


## Part 3. Настройка сети ОС
- Имя сервера: hostname
- Сменить имя: nano/vi /etc/hostname

##### Задать название машины:

![new-hostname](screens/Part 3 - hostname.png)

##### Задать временной пояс:
- ls /usr/share/zoneinfo/
- ln -sf /usr/share/zoneinfo/Asia/Novosibirsk /etc/localtime
- посмотреть дату: date

![date](screens/Part 3 - date.png)

##### Названия сетевых интерфейсов:
- ifconfig -a

![ifconfig](screens/Part 3 - ifconfig.png)

- lo — это локальная петля, которая имеет IP-адрес 127.0.0.1 и предназначена для сетевого доступа к своему же компьютеру.
- удалить ранее присвоеный ip: dhclient -r
- выделить новый: dhclient
- DHCP выполняет всю работу по подбору сетевых настроек автоматически, без необходимости присваивать вручную каждому устройству свой IP-адрес.

##### Определить и вывести
- Инфа о шлюзе: route -n

![route-n](screens/Part 3 - route-n.png)

##### Задать статичные ip gw dns
- открываем файл с параметрами: vi /etc/netplan/*.yaml
- добавляем статик ip и dns

![netparms](screens/Part 3 - netparms.png)

- netplan apply
- reboot
- ip -r

![new-ip](screens/Part 3 - static-ip.png)

##### Пропинговать

![ping](screens/Part 3 - ping-ips.png)


## Part 4. Обновление ОС
- apt-get update
- apt-get dist-upgrade

![update](screens/Part 4 - update.png)


## Part 5. Использование команды sudo
- Выдача прав: usermod -aG sudo %name%

![sudo](screens/Part 5 - sudo.png)

- sudo - команда которая позволяет вам запускать программы от имени суперпользователя.
- переключение на другого юзера: su two
- Сменить имя: sudo vi /etc/hostname

![newhost](screens/Part 5 - newhost.png)

![newhost2](screens/Part 5 - newhost2.png)


## Part 6. Установка и настройка службы времени
![date](screens/Part 6 - date.png)


## Part 7. Установка и использование текстовых редакторов
- apt install %program%

##### VIM
![vim-newfile](screens/Part 7 - vim-new-file.png)
- для выхода с сохр. команда: ":wq"

##### NANO
![nano-newfile](screens/Part 7 - nano-new-file.png)
- для выхода комбинация клавиш: ctr + X и подтверждение изменений

##### JOE
![joe-newfile](screens/Part 7 - nano-new-file.png)
- для выхода комбинация клавиш: ctr + K и ввод Q, затем подтверждение изменений

##### VIM
![vim-edit](screens/Part 7 - vim-edit.png)
- для выхода без сохр. команда: ":q!"

##### NANO
![nano-edit](screens/Part 7 - nano-edit.png)
- для выхода комбинация клавиш: ctr + X и НЕ подтверждение изменений

##### JOE
![joe-edit](screens/Part 7 - joe-edit.png)
- для выхода комбинация клавиш: ctr + K и ввод Q, затем НЕ подтверждение изменений

##### VIM
- для поиска слова: "/слово"

![vim-search](screens/Part 7 - vim-search.png)

- для замены слова: ":s/слово/замена"

![vim-replace](screens/Part 7 - vim-replace.png)

##### NANO
- для поиска комбинация клавиш: ctr + W

![nano-search](screens/Part 7 - nano-search.png)

- для замены комбинация клавиш: ctr + W и затем ctr + R

![nano-replace](screens/Part 7 - nano-replace.png)

##### JOE
- для поиска комбинация клавиш: ctr + K и ввод K

![joe-search](screens/Part 7 - joe-search.png)

- для замены комбинация клавиш: ctr + K и ввод K и затем ввод R

![joe-replace](screens/Part 7 - joe-replace.png)


## Part 8. Установка и базовая настройка сервиса SSHD

##### Установить службу SSHd
- apt update
- apt install openssh-server

##### Проверить статус работы SSHd
- systemctl status ssh - статус работы службы
- systemctl enable ssh - добавление в автозагрузку

##### Перенастроить службу SSHd на порт
- vim /etc/ssh/sshd_config
- меняем "#port 22" на "port 2022"
- перезапуск службы service ssh restart

##### Показать наличие процесса sshd
- ps ключ "-e" для отображения всех процессов | greb ssh - для вывода только процесса ssh
- ps команда для отслеживания активных процессов в системе

![ps](screens/Part 8 - ps.png)

##### Вывод команды netstat -tan
![netstat](screens/Part 8 - netstat-2022.png)

- "-t" - Отображение текущего подключения в состоянии переноса нагрузки с процессора на сетевой адаптер при передаче данных ( "offload" )
- "-a" - Отображение всех подключений и ожидающих портов
- "-n" - Отображение адресов и номеров портов в числовом формате
- proto - сетевой протокол
- Recv-Q - Количество байтов, не скопированных программой пользователя, подключенной к этому сокету.
- Send-Q - Количество байтов, не подтвержденных удаленным хостом
- Local Adress - Локальный адрес
- Foreign Adress - Адрес и номер порта удалённого конца сокета
- LISTEN - Ожидает входящих соединений
- Значение 0.0.0.0 - это означает - "любой адрес", т. е в соединении могут использоваться все IP-адреса существующие на данном компьютере


## Part 9. Установка и использование утилит top, htop
- apt install htop

##### TOP
- uptime
- количество авторизованных пользователей
- общую загрузку системы
- общее количество процессов
- загрузку cpu
- загрузку памяти

![uptime](screens/Part 9 - uptime.png)

- pid процесса занимающего больше всего памяти: 1
- pid процесса, занимающего больше всего процессорного времени: 1202

![pids](screens/Part 9 - pids.png)

##### HTOP
- PID SORT

![pid](screens/Part 9 - pid.png)

- PERCENT_CPU SORT

![percent-cpu](screens/Part 9 - percent_cpu.png)

- PERCENT_MEM SORT

![percent-mem](screens/Part 9 - percent_mem.png)

- TIME SORT

![time](screens/Part 9 - time.png)

- отфильтрованному для процесса sshd

![sshd](screens/Part 9 - sshd.png)

- с процессом syslog, найденным, используя поиск

![syslog](screens/Part 9 - syslog.png)

- с добавленным выводом hostname, clock и uptime

![settings](screens/Part 9 - settings.png)


## Part 10. Использование утилиты fdisk
- Название VBOX HARDDISK
- Кол-во секторов: 20971520
- судя по инфе fdisk -l /dev/sda swap раздел отсутствует


## Part 11. Использование утилиты df

##### Запустить команду df
- Размер корневого раздела: 8731376
- Размер занятого пространства: 4946332
- Размер свободного пространства: 3785044
- Процент использования: 57%

![df](screens/Part 10 - df.png)

- Единица измерения: Килобайт

##### Запустить команду df -Th
- Размер корневого раздела: 8.5G
- Размер занятого пространства: 4.8G
- Размер свободного пространства: 3.7G
- Процент использования: 57%
- Тип файловой системы: журналируемая файловая система ext4

![df-th](screens/Part 10 - df-th.png)


## Part 12. Использование утилиты du
- du -sB KB /home /var /var/log

![du](screens/Part 12 - du.png)

- du -B KB /var/log

![du2](screens/Part 12 - du2.png)


## Part 13. Установка и использование утилиты ncdu
-apt install ncdu

- ncdu /home

![ncdu-home](screens/Part 13 - ncdu-home.png)

- ncdu /var

![ncdu-var](screens/Part 13 - ncdu-var.png)

- ncdu /var/log

![ncdu-var-log](screens/Part 13 - ncdu-var-log.png)


## Part 14. Работа с системными журналами

![login](screens/Part 14 - login.png)

- systemctl restart sshd

![restart](screens/Part 14 - restart.png)


## Part 15. Использование планировщика заданий CRON
- Cоздание файла расписания: crontab -e

![cronlog](screens/Part 15 - cronlog.png)

- Вывод содержимого расписания: crontab -l

![tasks](screens/Part 15 - tasks.png)

- Отработка планировщика

![complete](screens/Part 15 - complete.png)

- Удаление содержимого расписания: crontab -r

![emptylist](screens/Part 15 - emptylist.png)

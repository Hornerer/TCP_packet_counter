Программа была написана на VS Code с расширением WSL Remote на Windows.
Собрана с помощью make под linux. 
Программа подсчитывает TCP-пакеты  протокола  IPv4. 
Фильтры можно комбинировать
Дамп сетевого трафика был сделан с помощью WireShark и сохранен в pcap-файл TCP_dump.pcap

Тестирование работоспособности программы при разнных вводных

Без фильтров
~/projects/tcp_counter$  ./tcp_counter TCP_dump.pcap

Total packet count: 1099

TCP packet count: 1083

Filtered TCP count: 1083


Фильтр по IP источнка
~/projects/tcp_counter$ ./tcp_counter TCP_dump.pcap --srcaddr 192.168.0.116

Total packet count: 1099

TCP packet count: 1083

Filtered TCP count: 451


Фильтр по IP получателя
~/projects/tcp_counter$  ./tcp_counter TCP_dump.pcap --dstaddr 151.115.98.198

Total packet count: 1099

TCP packet count: 1083

Filtered TCP count: 402


Фильтр по порту источнка
~/projects/tcp_counter$ ./tcp_counter TCP_dump.pcap --srcport 443

Total packet count: 274

TCP packet count: 268

Filtered TCP count: 165


Фильтр по порту получателя
~/projects/tcp_counter$ ./tcp_counter TCP_dump.pcap --dstport 443

Total packet count: 274

TCP packet count: 268

Filtered TCP count: 103


Комбинированный фильтр
~/projects/tcp_counter$ ./tcp_counter TCP_dump.pcap --dstaddr 94.124.200.1  --srcaddr 192.168.0.116

Total packet count: 1099

TCP packet count: 1083

Filtered TCP count: 2


~/projects/tcp_counter$ ./tcp_counter TCP_dump.pcap --srcaddr 192.168.0.116 --dstaddr 151.115.98.198 --srcport 12345 --dstport 80

Total packet count: 1099

TCP packet count: 1083

Filtered TCP count: 0



# Задачи конфигурации

## Domain
Должны быть сгенерированы два файла astra.domain.agent.xml, astra.net.agent.xml.\
Для проверки корректности можно использовать расширение для IDE.

### Шаблон astra.domain.agent.xml:

```
<?xml version="1.0" encoding="utf-8"?>
    <Astra.Domain.Agent>
        <EntryPointNetAgent Name="<NAME>" Address="<IP>" Port="<PORT>"/>
            <InstalledComponents>
                <Astra.Server Name="InstalledServer" ServiceName="<SERVICE_NAME>"/>
                // И/или
                <Astra.Server Name="InstalledAccessPoint" ServiceName="Astra.AccessPoint" DefaultActivation="1" />
            </InstalledComponents>
        // Для сервера
        <Server> 
            <Components StoragePath="c:\DomainStorage\cache\server">  
            <Component InstalledName="InstalledServer" Name="AstraServer" StorageLimitSize="0" StorageLimitNum="0"/>
            // И/или
            <Component InstalledName="InstalledAccessPoint" Name="AccessPoint" StorageLimitSize="0" StorageLimitNum="0" />
            </Components>
        </Server>
        // Для АРМ 
        <Workstation Dynamic="false">
            <WorkstationRoles>
                <WorkstationRole Name="THIN_CLIENT">
                    <Components StoragePath="c:\DomainStorage\cache\server">
                        <Component InstalledName="InstalledAccessPoint" Name="AccessPoint" />
                    </Components>
                </WorkstationRole>
            </WorkstationRoles>
        </Workstation>
        // Для главного узла
        <Domain>
            <WorkstationRoles>
            <WorkstationRole Name="THIN_CLIENT">
                <KnownWorkstations>
                    <KnownWorkstation AstraNetPath="<Имя АРМ>" Description="<Описание АРМ>" />
                </KnownWorkstations>
            </WorkstationRole>
            </WorkstationRoles>
            <ConfigurationCache Path="c:\DomainStorage\cache" />
        </Domain>
    <Options LoggerLevel="2" />
</Astra.Domain.Agent>
```

### Historian
```
<?xml version="1.0" encoding="utf-8"?>
<Astra.Historian.Server StatPort="3388" DefaultPrimaryDir="C:\AstraRegul\Astra.Historian\Databases" MaxPoolMemorySize="512" EnableDCOM="0">
  <tcp-server default-port="4949" idle-sessions-count="1" idle-sessions-timeout="15">
    <server-endpoint host="0.0.0.0" />
  </tcp-server>
  <Bases>
    <Base Alias="DB_EVENTS" PreferredCommonCacheLimit="128" ActiveStorageDepth="3" StorageDepth="365" VolumeLimit="2000" ArchiveCompression="none" BlocksPerCompressedPack="10" Recompress="none" />
    <Base Alias="DB_HISTORY" PreferredCommonCacheLimit="128" ActiveStorageDepth="3" StorageDepth="365" VolumeLimit="2000" ArchiveCompression="none" BlocksPerCompressedPack="10" Recompress="none" />
    <Base Alias="DB_IMIT" PreferredCommonCacheLimit="128" ActiveStorageDepth="3" StorageDepth="365" VolumeLimit="2000" ArchiveCompression="none" BlocksPerCompressedPack="10" Recompress="none" />
  </Bases>
</Astra.Historian.Server>
```

### Security
https://support.automiq.ru/otrs/customer.pl?Action=CustomerTicketZoom;TicketNumber=0813283


### RMap
Дописать в файл: \
Windows - `C:\Program Files\AstraRegul\Astra.RMap\Initialize\init_rmap_history.sql` \
Astra Linux - `/usr/share/postgresql/14/extension/init_rmap_history.sql` \
РЕД ОС - `/usr/pgsql-14/share/extension/init_rmap_history.sql`
```
CREATE USER MAPPING FOR "postgres"
SERVER history_server;
```

#### Windows
Запустить файл \
`C:\Program Files\AstraRegul\Astra.RMap\Initialize\init_rmap_history.sql`

#### Astra Linux
	
В файл `/etc/odbcinst.ini` записать:
```
[PostgreSQL ANSI]
Description=PostgreSQL ODBC driver (ANSI version)
Driver=/usr/lib/x86_64-linux-gnu/odbc/psqlodbca.so
Setup=libodbcpsqlS.so
Debug=0
CommLog=1
UsageCount=1
 
[PostgreSQL Unicode]
Description=PostgreSQL ODBC driver (Unicode version)
Driver=/usr/lib/x86_64-linux-gnu/odbc/psqlodbcw.so
Setup=libodbcpsqlS.so
Debug=0
CommLog=1
UsageCount=1
```

В файл `/etc/odbc.ini` записать:
```
[PostgreSQL-connector]
Description      = PostgreSQL connection
Driver           = PostgreSQL Unicode
Database         = postgres
Servername       = localhost
Username         = postgres
Password         = postgres
Port             = 5432
ReadOnly         = No
RowVersioning = No
ShowSystemTables = No
ShowOidColumn = No
FakeOidIndex       = No
ConnSettings     =
```

В конце файла `/etc/postgresql/14/main/pg_hba.conf` добавить:
```
host      all             all             ::1/128           md5
```


Выполнить команды: 
```
sudo -u postgres psql	
\i /usr/share/postgresql/14/extension/init_rmap_history.sql
```

#### РЕД ОС

В файл `/etc/odbcinst.ini` записать:
```
[PostgreSQL]
Description     = ODBC for PostgreSQL
Driver               = /usr/lib/psqlodbcw.so
Setup                = /usr/lib/libodbcpsqlS.so
Driver64           = /usr/lib64/psqlodbcw.so
Setup64            = /usr/lib64/libodbcpsqlS.so
FileUsage          = 1
```

В файл `/etc/odbc.ini` записать:
```
[PostgreSQL-connector]
Description = PostgreSQL connection
Driver = PostgreSQL
Database = postgres
Servername = localhost
Username = postgres
Password = postgres
Port = 5432
ReadOnly = No
RowVersioning = No
ShowSystemTables = No
ShowOidColumn = No
FakeOidIndex = No
ConnSettings =
```

Выполнить команды:
```
sudo postgresql-14-setup initdb
sudo systemctl enable postgresql-14.service --now
sudo systemctl restart postgresql-14.service
```

В конце файла /var/lib/pgsql/14/data/pg_hba.conf добавить:
```
host      all             all             ::1/128           md5
```

#### WebViewer
```
<?xml version="1.0" encoding="utf-8"?>
<Astra.HMI.WebViewer PythonPath="<Путь до питона>"> 
  <WebSocket Address="localhost" Port="8080"/>
  <Application ProjectPath="<Путь до проекта>" WwwRoot="<Путь до папки где будет компиляция>"/>
</Astra.HMI.WebViewer>
```
\
\
`После всех настроек перезапустить PostgreSQL и RMap`


### PostgreSQL
```
odbcconf.exe /a {CONFIGSYSDSN "PostgreSQL Unicode" "DSN=PostgreSQL35W|Description=Подключение к PostgreSQL|Database=postgres|Server=localhost|Port=5432|UID=postgres|Password=postgres"}
```
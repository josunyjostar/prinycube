erase /q .\Reference\Librarys.Debug\*.*
erase /q .\Reference\Librarys.Release\*.*
erase /q .\Reference\Headers\*.*


erase /q .\Client\Bin.Debug
erase /q .\Client\Bin.Release
erase /q .\Client\Default\*.user

erase /q .\Tool\Bin.Debug
erase /q .\Tool\Bin.Release
erase /q .\Tool\Default\*.user

erase /q .\Engine\Resource\Bin.Debug
erase /q .\Engine\Resource\Bin.Release
erase /q .\Engine\Resource\Default\*.user

erase /q .\Engine\System\Bin.Debug
erase /q .\Engine\System\Bin.Release
erase /q .\Engine\System\Default\*.user

erase /q .\Engine\Utility\Bin.Debug
erase /q .\Engine\Utility\Bin.Release
erase /q .\Engine\Utility\Default\*.user

erase /q .\*.ncb
erase /q .\*.suo

rmdir /s /q .\Client\Default\Debug
rmdir /s /q .\Client\Default\Release
rmdir /s /q .\Tool\Default\Debug
rmdir /s /q .\Tool\Default\Release
rmdir /s /q .\Engine\Resource\Default\Debug
rmdir /s /q .\Engine\Resource\Default\Release
rmdir /s /q .\Engine\System\Default\Debug
rmdir /s /q .\Engine\System\Default\Release
rmdir /s /q .\Engine\Utility\Default\Debug
rmdir /s /q .\Engine\Utility\Default\Release

cd..
cd..

xcopy .\GameEditor\Include\Client\*.* .\GameClient\Include\Client\ /s /d /y
xcopy .\GameEditor\Include\*.h .\GameClient\Include\ /s /d /y
xcopy .\GameEditor\Include\*.cpp .\GameClient\Include\ /s /d /y

xcopy .\GameEditor\Bin\Resource\*.* .\GameClient\Bin\Resource\ /s /d /y
xcopy .\GameEditor\Bin\Data\*.* .\GameClient\Bin\Data\ /s /d /y
xcopy .\GameEditor\Bin\Sound\*.* .\GameClient\Bin\Sound\ /s /d /y
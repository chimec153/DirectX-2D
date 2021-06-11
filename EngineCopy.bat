
cd..
cd..

xcopy .\GameEngine\Include\*.h .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.inl .\Engine\Include\ /s /d /y
xcopy .\GameEngine\Include\*.hpp .\Engine\Include\ /s /d /y

xcopy .\GameEngine\Bin\*.lib .\Engine\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\GameClient\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\GameClient\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\GameEditor\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\Editor\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.dll .\Server\Bin\ /s /d /y
xcopy .\GameEngine\Bin\*.lib .\Server\Bin\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\Engine\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\GameClient\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\GameEditor\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\Client\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\Editor\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Resource\*.* .\Server\Bin\Resource\ /s /d /y
xcopy .\GameEngine\Bin\Sound\*.* .\Engine\Bin\Sound\ /s /d /y
xcopy .\GameEngine\Bin\Sound\*.* .\GameClient\Bin\Sound\ /s /d /y
xcopy .\GameEngine\Bin\Sound\*.* .\GameEditor\Bin\Sound\ /s /d /y
xcopy .\GameEngine\Bin\Sound\*.* .\Server\Bin\Sound\ /s /d /y

xcopy .\GameEditor\Include\Client\*.* .\GameClient\Include\Client\ /s /d /y

xcopy .\GameEditor\Bin\Resource\*.* .\GameClient\Bin\Resource\ /s /d /y
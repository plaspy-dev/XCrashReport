# Local mail test · Prueba de correo local
[English](#english) · [Español](#español)

## English

This example uses a synthetic report. Build from the repository root in Developer PowerShell with the included XHyperLink sources. The imported test target only changes the mail client in the test build of `SendEmail.cpp`.

```powershell
$test = New-Item -ItemType Directory (Join-Path $env:TEMP "xcrashreport-$([guid]::NewGuid())")
$sample = New-Item -ItemType Directory (Join-Path $test.FullName 'artifacts')
$targets = (Resolve-Path 'examples/demo-mail/demo-mail-client.targets').Path
MSBuild examples/demo-mail/fixture.vcxproj /t:Build /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64
MSBuild XCrashReport.sln /m:2 /nr:false /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64 "/p:ForceImportBeforeCppTargets=$targets"
Copy-Item examples/demo-mail/bin/XCrashReportDemo.dll $test.FullName
[IO.File]::WriteAllBytes((Join-Path $sample.FullName 'CRASH.DMP'), [byte[]](77,68,77,80,147,167,0,0))
[IO.File]::WriteAllText((Join-Path $sample.FullName 'ERRORLOG.TXT'), "DEMO-104 | DemoApp | Synthetic report`r`n", [Text.Encoding]::ASCII)
$savedTemp = $env:TEMP
$savedTmp = $env:TMP
$savedDemoDir = $env:XCRASHREPORT_DEMO_DIR
try {
    $env:TEMP = $test.FullName
    $env:TMP = $test.FullName
    $env:XCRASHREPORT_DEMO_DIR = $test.FullName
    .\bin\Release\XCrashReport.exe DemoApp.exe (Join-Path $sample.FullName 'DemoApp.exe')
} finally {
    $env:TEMP = $savedTemp
    $env:TMP = $savedTmp
    $env:XCRASHREPORT_DEMO_DIR = $savedDemoDir
}
```

In the report window, type `DEMO-104: synthetic crash report from the test desktop.` Check that both files are selected under **Haga click aquí**, then click **Enviar reporte de error**. The demo mail provider accepts only this fictional report and writes `report.zip` and `message.txt` to `$test`. The normal build uses the Windows Simple MAPI client.

Start a disposable local Mailpit container and send the test provider's output. This uses Python 3's standard library.

```powershell
$inbox = docker run --rm --detach --name prueba-xcrashreport-mailpit -p 127.0.0.1::1025 -p 127.0.0.1::8025 -e MP_DISABLE_VERSION_CHECK=true axllent/mailpit:v1.27.4
if ($LASTEXITCODE) { throw 'Mailpit did not start' }
try {
    $smtp = (docker port $inbox 1025/tcp).Split(':')[-1]
    $web = (docker port $inbox 8025/tcp).Split(':')[-1]
    python examples/demo-mail/deliver.py $test.FullName $smtp
    Write-Host "Open http://127.0.0.1:$web to inspect the fictional message and DemoApp.zip."
    Read-Host 'Press Enter to stop the test inbox'
} finally {
    docker stop $inbox
}
```

Rebuild the normal executable by repeating the solution command with `ForceImportBeforeCppTargets` omitted. After inspection, delete the temporary directory printed in `$test.FullName`.

## Español

Este ejemplo usa un informe ficticio. Ejecuta el primer bloque de PowerShell desde la raíz del repositorio, en la consola de desarrollador de Visual Studio. La solución compila las copias archivadas de XHyperLink incluidas en el repositorio. El archivo de proyecto importado selecciona el cliente de correo de prueba solo al compilar `SendEmail.cpp` para este ensayo.

En la ventana escribe `DEMO-104: synthetic crash report from the test desktop.` Abre **Haga click aquí** para revisar los adjuntos y deja los dos marcados. Pulsa **Enviar reporte de error**. El cliente de prueba acepta únicamente el informe ficticio y guarda `report.zip` y `message.txt` en `$test`. La compilación normal usa Simple MAPI de Windows.

Ejecuta el segundo bloque para levantar Mailpit en Docker y enviar la salida del cliente de prueba con Python 3. La consola muestra la dirección del buzón local. Ábrela para inspeccionar el mensaje y su adjunto `DemoApp.zip`. Pulsa Enter en la consola al terminar para retirar el contenedor.

Para recuperar el ejecutable normal, vuelve a compilar la solución omitiendo `ForceImportBeforeCppTargets`. Luego elimina la carpeta temporal indicada en `$test.FullName`.

# XCrashReport
[English](#english) · [Español](#español)

## English

Legacy Windows utility for reviewing and emailing application crash reports with selected ZIP attachments

> [!WARNING]
> **Status: Legacy.** Reconstructed from archived Win32 code. Tested on Windows 11 with synthetic reports.

### Overview

XCrashReport shows a Windows program’s crash report before it is sent for support. The program that failed supplies its name and the location of the files it already generated. A user can inspect the report, choose its attachments, and tell support what they were doing when the problem happened.

### Features

- Shows a dialog with the supplied application name and an optional comment.
- Lists the error log and crash dump, and shows the contents of text attachments.
- Lets the user exclude individual items and packages the selected files as a ZIP.
- Hands the comment and ZIP to the Windows Simple MAPI client. The built-in example points to `reports@example.invalid`.
- Reads additional file and registry entries when those are explicitly declared in a local INI file.

### Requirements

- Windows with desktop support and Visual Studio 2026 18.11.4, MSVC v145, MFC x86 and Windows SDK 10.0.26100.0. Build `Win32`.
- The two XHyperLink 1.0 sources are included next to the app. See THIRD-PARTY-NOTICES.md for authors, notices and hashes.

- A compatible 32-bit Simple MAPI client for the ordinary Send button. For local checks use a disposable profile and a test inbox.

### Getting started

In a Developer PowerShell for Visual Studio, run:

```powershell
MSBuild XCrashReport.sln /m:2 /nr:false /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64
```

The executable is `bin/Release/XCrashReport.exe`. Create a synthetic log and launch the dialog from the repository root:

```powershell
$demo = Join-Path $env:TEMP "xcrashreport-demo-$([guid]::NewGuid())"
New-Item -ItemType Directory $demo | Out-Null
"DEMO-104 | DemoApp | Synthetic report" | Set-Content (Join-Path $demo "ERRORLOG.TXT")
.\bin\Release\XCrashReport.exe DemoApp.exe (Join-Path $demo "DemoApp.exe")
```

The executable named in the second argument may be fictitious. Use **Haga click aquí** for the attachment list and **No enviar** to close the sample. If you use the optional INI file, only list files and registry entries you want to include.

### Usage

When integrating it into another Windows program, pass the executable name as the first argument and a full path ending in that executable name as the second. The viewer looks for `ERRORLOG.TXT`, `CRASH.DMP`, and the optional `XCrashReport.ini` beside that path. In the dialog, clear any item you want to leave out, add a comment, and review the recipient before handing the report to mail. The sample recipient is reserved for examples. Set the real recipient through your own deployment copy of `EmailDefines.h`.

### Project structure

`Dlg.cpp` displays the report and chooses attachments. `Details.cpp` implements the attachment list and preview. `XZip.cpp` creates the archive and `SendEmail.cpp` hands it to Simple MAPI. The converted `XCrashReport.sln` builds the Win32 executable.

### Support

When reporting a problem, include the version or commit, Visual Studio version, Win32 configuration, and steps using a synthetic report. Share build errors or redacted logs through the repository’s issue tracker.

### Contributing

Build the current solution before proposing a focused change. Use synthetic report files, keep upstream notices intact, and describe the effect on the displayed attachments or the mail handoff.

### History

The 14 September 2004 date comes from the newest original source in the archived RAR. The 2026 reconstruction converted the original project and retained its `.vcproj` and `.sln` alongside the v145 solution. It provided a fictional mail recipient and an empty default registry export.

### License

The [MIT license](LICENSE) applies to the project’s own integration. See [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md) for the original code and build dependencies.

---

## Español

Utilidad legada de Windows para revisar informes de fallos y enviar por correo los adjuntos elegidos en un ZIP

> [!WARNING]
> **Estado: Legado.** Reconstrucción de código Win32 archivado. Se probó en Windows 11 con informes ficticios.

### Descripción general

XCrashReport muestra el informe de fallo de un programa de Windows antes de enviarlo a soporte. El programa que falló suministra su nombre y la ubicación de los archivos que ya generó. La persona puede revisar el informe, escoger qué adjuntar y contar qué estaba haciendo cuando ocurrió el problema.

### Funciones

- Muestra una ventana con el nombre de la aplicación y un campo para comentarios.
- Lista el registro de error y el volcado, y permite leer los adjuntos de texto.
- Permite excluir archivos y reúne los elegidos en un ZIP.
- Entrega el comentario y el ZIP al cliente Simple MAPI de Windows. El ejemplo apunta a `reports@example.invalid`.
- Lee archivos o claves de registro adicionales cuando se declaran expresamente en un INI local.

### Requisitos

- Windows de escritorio y Visual Studio 2026 18.11.4 con MSVC v145, MFC x86 y el SDK de Windows 10.0.26100.0. Compila para `Win32`.
- Los dos fuentes de XHyperLink 1.0 se incluyen junto a la aplicación. Los avisos registran autores, condiciones y hashes.

- Un cliente de 32 bits compatible con Simple MAPI para el botón normal de envío. Para ensayar usa un perfil desechable y un buzón de prueba.

### Cómo empezar

En Developer PowerShell para Visual Studio, ejecuta:

```powershell
MSBuild XCrashReport.sln /m:2 /nr:false /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64
```

El ejecutable queda en `bin/Release/XCrashReport.exe`. Crea un registro ficticio y abre el informe desde la raíz del repositorio:

```powershell
$demo = Join-Path $env:TEMP "xcrashreport-demo-$([guid]::NewGuid())"
New-Item -ItemType Directory $demo | Out-Null
"DEMO-104 | DemoApp | Synthetic report" | Set-Content (Join-Path $demo "ERRORLOG.TXT")
.\bin\Release\XCrashReport.exe DemoApp.exe (Join-Path $demo "DemoApp.exe")
```

El ejecutable nombrado en el segundo argumento puede ser ficticio. Pulsa **Haga click aquí** para revisar los adjuntos y **No enviar** para cerrar la muestra. Si usas el INI opcional, lista únicamente los archivos y claves de registro que quieras incluir.

### Uso

Al integrarlo en otro programa de Windows, pasa el nombre de su ejecutable como primer argumento y una ruta completa que termine con ese nombre como segundo. El visor busca `ERRORLOG.TXT`, `CRASH.DMP` y el archivo opcional `XCrashReport.ini` junto a esa ruta. En la ventana desmarca los archivos que quieras excluir, añade el comentario y revisa el destinatario antes de enviar. La dirección de ejemplo está reservada para pruebas. Configura el destinatario de tu instalación en tu copia de `EmailDefines.h`.

### Estructura del proyecto

`Dlg.cpp` muestra el informe y selecciona los adjuntos. `Details.cpp` contiene la lista y la vista previa. `XZip.cpp` crea el archivo comprimido y `SendEmail.cpp` se lo entrega a Simple MAPI. La solución `XCrashReport.sln` convertida compila el ejecutable Win32.

### Soporte

Al reportar un problema, indica la versión o commit, la versión de Visual Studio, la configuración Win32 y los pasos con un informe ficticio. Comparte errores de compilación o registros redactados en el gestor de incidencias del repositorio.

### Contribuir

Compila la solución actual antes de proponer un cambio puntual. Usa informes ficticios, conserva los avisos originales y explica el efecto sobre los adjuntos o la entrega al cliente de correo.

### Historial

La fecha del 14 de septiembre de 2004 corresponde al archivo fuente más reciente del RAR original. La reconstrucción de 2026 convirtió el proyecto y conserva el `.vcproj` y la `.sln` originales junto a la solución v145. Incluyó un destinatario ficticio y dejó vacío el ámbito predeterminado de exportación del registro.

### Licencia

La [licencia MIT](LICENSE) cubre la integración propia. Consulta [THIRD-PARTY-NOTICES.md](THIRD-PARTY-NOTICES.md) para ver los avisos del código original y las dependencias.

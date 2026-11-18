"""Deliver the fictional DemoApp report to the running local Mailpit instance."""

from email.message import EmailMessage
import io
from pathlib import Path
from smtplib import SMTP
import sys
from zipfile import ZipFile


def deliver(folder, port):
    subject, description = (folder / 'message.txt').read_text(encoding='ascii').split('\n', 1)
    if subject != 'Reporte de Error de DemoApp.exe' or 'DEMO-104' not in description:
        raise ValueError('Expected the fictional DemoApp message')
    contents = (folder / 'report.zip').read_bytes()
    with ZipFile(io.BytesIO(contents)) as archive:
        expected = {'errorlog.txt', 'crash.dmp'}
        if {name.casefold() for name in archive.namelist()} != expected:
            raise ValueError('Expected the two fictional attachments')
        files = {name.casefold(): archive.read(name) for name in archive.namelist()}
        if files['errorlog.txt'] != b'DEMO-104 | DemoApp | Synthetic report\r\n':
            raise ValueError('Expected the synthetic error log')
        if files['crash.dmp'] != b'MDMP\x93\xa7\0\0':
            raise ValueError('Expected the dummy eight-byte dump')
    letter = EmailMessage()
    letter['From'] = 'demo@example.invalid'
    letter['To'] = 'reports@example.invalid'
    letter['Subject'] = subject
    letter.set_content(description)
    letter.add_attachment(contents, maintype='application', subtype='zip', filename='DemoApp.zip')
    with SMTP('127.0.0.1', port, timeout=8) as server:
        if server.send_message(letter):
            raise RuntimeError('The local test server rejected the report')
    print('Delivered the fictional report and DemoApp.zip to the local mailbox.')


if __name__ == '__main__':
    if len(sys.argv) != 3:
        raise SystemExit('Usage: python deliver.py TEST_DIRECTORY LOCAL_SMTP_PORT')
    deliver(Path(sys.argv[1]), int(sys.argv[2]))

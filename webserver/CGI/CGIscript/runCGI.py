import cgi, cgitb
import os, sys

print('HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 700\n\n')
print('<!DOCTYPE html>\n<html lang="en">\n<head>\n\r<meta charset="UTF-8">\n\r<meta name="viewport" content="width=device-width, initial-scale=1.0">\n\r<title>Vibrant Website</title>\n\n</head>\n<body>\n\r<h1>you file has been uploaded successfully!</h1>\n</body>\n</html>')

UPLOAD_DIR = '/Users/hait-hsa/Desktop/second_v/webserver/CGI/CGIscript'

form = cgi.FieldStorage()
for key in form.keys():
    file_item = form[key]
    if file_item.filename:
        filename = os.path.basename(file_item.filename)
        filepath = os.path.join(UPLOAD_DIR, filename)
        with open(filepath, 'wb') as f:
            while True:
                chunk = file_item.file.read(4096)
                if not chunk:
                    break
                f.write(chunk)


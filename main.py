import gevent
import os
import re
import select
import shutil
import subprocess
import threading

from datetime import datetime
from flask import Flask, request, redirect, url_for, jsonify, render_template
from flask.ext.socketio import SocketIO, emit
from PIL import Image, ImageChops
from werkzeug import secure_filename

ROOT_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')
TEST_DIR = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(ROOT_DIR, 'build')

ALLOWED_EXTENSIONS = set(['cpp'])
IMAGE_SIZE = [512, 512]

app = Flask(__name__)
app.debug = True
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

lock = threading.Lock()

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

def run(command, cwd, namespace):
    proc = subprocess.Popen(command, shell=True, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    while proc.poll() is None:
        (rlist, wlist, xlist) = select.select([proc.stderr], [], [], 0)
        if rlist:
            socketio.emit('stderr', {'data': rlist[0].readline()}, namespace=namespace)
        (rlist, wlist, xlist) = select.select([proc.stdout], [], [], 0)
        if rlist:
            socketio.emit('stdout', {'data': rlist[0].readline()}, namespace=namespace)
    for line in proc.stderr:
        socketio.emit('stderr', {'data': line}, namespace=namespace)
    if proc.returncode:
        raise Exception('Command \''+command+'\' failed, Code: '+str(proc.returncode))

def upload_file(files, directory):
    files = files.getlist("files[]")
    file = files[0]

    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        file.save(os.path.join(directory, filename))
        return filename
    else:
        raise Exception('Error uploading file')

def compare_images(directory, exercise, pattern):
    image_build = os.path.join(BUILD_DIR, pattern+'.bmp')
    image_original = os.path.join(TEST_DIR, 'static', 'img', 'solutions', exercise+'.png')
    image_generated = os.path.join(directory, pattern+'.png')
    image_diff = os.path.join(directory, 'diff-'+pattern+'.png')

    Image.open(image_build).resize(IMAGE_SIZE).save(image_generated, "PNG", optimize=True)
    generated = Image.open(image_generated).resize(IMAGE_SIZE)
    original = Image.open(image_original).resize(IMAGE_SIZE)

    diff = ImageChops.difference(original, generated)
    diff.save(image_diff, "PNG", optimize=True)

@app.route("/bresenham", methods=['GET', 'POST'])
def bresenham():
    if request.method == 'POST':
        lock.acquire()

        ERROR_FLAG = False

        try:
            id = datetime.now().strftime('%s-%f')
            RESULT_DIR = os.path.join(TEST_DIR, 'static', 'gen', 'bresenham', 'results', id)
            os.makedirs(RESULT_DIR)

            filename = upload_file(request.files, RESULT_DIR)

            new = ''
            with open(os.path.join(RESULT_DIR, filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
            with open(os.path.join(ROOT_DIR, 'test.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'code', 'bresenham.cpp'), 'r') as test:
                fout.write(new)
                fout.write('\n')
                fout.write(test.read())

            run('cmake ..', BUILD_DIR, '/test')
            run('make test', BUILD_DIR, '/test')
            run('./test', BUILD_DIR, '/test')

            compare_images(RESULT_DIR, 'bresenham', 'rays')
        except Exception as ex:
            socketio.emit('error', {'data': str(ex)}, namespace='/test')
            ERROR_FLAG = True

        lock.release()

        return jsonify(id=id, error=ERROR_FLAG)

    return render_template('bresenham.html')

@app.route("/floodfill", methods=['GET', 'POST'])
def floodfill():
    if request.method == 'POST':
        lock.acquire()

        ERROR_FLAG = False

        try:
            id = datetime.now().strftime('%s-%f')
            RESULT_DIR = os.path.join(TEST_DIR, 'static', 'gen', 'floodfill', 'results', id)
            os.makedirs(RESULT_DIR)

            filename = upload_file(request.files, RESULT_DIR)

            new = ''
            with open(os.path.join(RESULT_DIR, filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
            with open(os.path.join(ROOT_DIR, 'test.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'code', 'floodfill.cpp'), 'r') as test:
                fout.write(new)
                fout.write('\n')
                fout.write(test.read())

            run('cmake ..', BUILD_DIR, '/test')
            run('make test', BUILD_DIR, '/test')
            run('./test', BUILD_DIR, '/test')

            compare_images(RESULT_DIR, 'floodfill', 'rand')
        except Exception as ex:
            socketio.emit('error', {'data': str(ex)}, namespace='/test')
            ERROR_FLAG = True

        lock.release()

        return jsonify(id=id, error=ERROR_FLAG)

    return render_template('floodfill.html')

@app.route("/rasterization", methods=['GET', 'POST'])
def rasterization():
    if request.method == 'POST':
        lock.acquire()

        ERROR_FLAG = False

        try:
            id = datetime.now().strftime('%s-%f')
            RESULT_DIR = os.path.join(TEST_DIR, 'static', 'gen', 'rasterization', 'results', id)
            os.makedirs(RESULT_DIR)

            filename = upload_file(request.files, RESULT_DIR)

            new = ''
            with open(os.path.join(RESULT_DIR, filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
                new = re.sub('#include \<minmax.h\>', 'using namespace std;', new)
            with open(os.path.join(ROOT_DIR, 'test.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'code', 'rasterization.cpp'), 'r') as test:
                fout.write(new)
                fout.write('\n')
                fout.write(test.read())

            run('cmake ..', BUILD_DIR, '/test')
            run('make test', BUILD_DIR, '/test')
            run('./test', BUILD_DIR, '/test')

            compare_images(RESULT_DIR, 'rasterization', 'rand')
        except Exception as ex:
            socketio.emit('error', {'data': str(ex)}, namespace='/test')
            ERROR_FLAG = True

        lock.release()

        return jsonify(id=id, error=ERROR_FLAG)

    return render_template('rasterization.html')

@app.route("/zbuffer", methods=['GET', 'POST'])
def zbuffer():
    if request.method == 'POST':
        lock.acquire()

        ERROR_FLAG = False

        try:
            id = datetime.now().strftime('%s-%f')
            RESULT_DIR = os.path.join(TEST_DIR, 'static', 'gen', 'zbuffer', 'results', id)
            os.makedirs(RESULT_DIR)

            filename = upload_file(request.files, RESULT_DIR)

            new = ''
            with open(os.path.join(RESULT_DIR, filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
                new = re.sub('#include \<minmax.h\>', 'using namespace std;', new)
            with open(os.path.join(ROOT_DIR, 'test.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'code', 'zbuffer.cpp'), 'r') as test:
                fout.write(new)
                fout.write('\n')
                fout.write(test.read())

            run('cmake ..', BUILD_DIR, '/test')
            run('make test', BUILD_DIR, '/test')
            run('./test', BUILD_DIR, '/test')

            compare_images(RESULT_DIR, 'zbuffer', 'rand')
        except Exception as ex:
            socketio.emit('error', {'data': str(ex)}, namespace='/test')
            ERROR_FLAG = True

        lock.release()

        return jsonify(id=id, error=ERROR_FLAG)

    return render_template('zbuffer.html')

@socketio.on('connect', namespace='/test')
def test_connect():
    emit('system', {'data': 'Connected'})

if __name__ == "__main__":
    socketio.run(app, host='0.0.0.0')
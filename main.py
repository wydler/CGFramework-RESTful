import gevent
import os
import re
import shutil
import subprocess
import threading

from datetime import datetime
from flask import Flask, request, redirect, url_for, jsonify, render_template
from flask.ext.socketio import SocketIO, emit
from PIL import Image, ImageChops
from werkzeug import secure_filename

ROOT_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')
print(ROOT_DIR)
TEST_DIR = os.path.dirname(os.path.realpath(__file__))
print(TEST_DIR)
BUILD_DIR = os.path.join(ROOT_DIR, 'build')
print(BUILD_DIR)

UPLOAD_FOLDER = 'tmp'
ALLOWED_EXTENSIONS = set(['cpp'])

app = Flask(__name__)
app.debug = True
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

lock = threading.Lock()

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

@app.route("/bresenham", methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        lock.acquire()

        id = datetime.now().strftime('%s-%f')
        RESULT_DIR = os.path.join(TEST_DIR, 'static', 'bresenham', 'results', id)
        os.makedirs(RESULT_DIR)

        files = request.files.getlist("files[]")
        file = files[0]

        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(RESULT_DIR, filename))

            STOP_FLAG = False

            new = ''
            with open(os.path.join(RESULT_DIR, filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
            with open(os.path.join(ROOT_DIR, 'test.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'bresenham', 'test.cpp'), 'r') as test:
                fout.write(new)
                fout.write('\n')
                fout.write(test.read())

            if not STOP_FLAG:
                proc = subprocess.Popen('cmake ..', shell=True, cwd=BUILD_DIR, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                while proc.poll() is None:
                    line = proc.stdout.readline()
                    if line:
                        socketio.emit('build', {'data': line}, namespace='/test')
                        gevent.sleep(0)
                STOP_FLAG = proc.returncode

            if not STOP_FLAG:
                proc = subprocess.Popen('make test', shell=True, cwd=BUILD_DIR, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                while proc.poll() is None:
                    line = proc.stdout.readline()
                    if line:
                        socketio.emit('build', {'data': line}, namespace='/test')
                        gevent.sleep(0)
                STOP_FLAG = proc.returncode

            if not STOP_FLAG:
                proc = subprocess.Popen('./test', shell=True, cwd=BUILD_DIR, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                while proc.poll() is None:
                    line = proc.stdout.readline()
                    if line:
                        socketio.emit('build', {'data': line}, namespace='/test')
                        gevent.sleep(0)
                STOP_FLAG = proc.returncode

            if not STOP_FLAG:
                shutil.copy(os.path.join(BUILD_DIR, 'rand.bmp'), os.path.join(RESULT_DIR, 'rand.bmp'))
                shutil.copy(os.path.join(BUILD_DIR, 'rays.bmp'), os.path.join(RESULT_DIR, 'rays.bmp'))
                solution = Image.open(os.path.join(RESULT_DIR, 'rays.bmp')).resize([512, 512])
                image = Image.open(os.path.join(TEST_DIR, 'static', 'bresenham', 'solutions', 'rays.bmp')).resize([512, 512])
                diff = ImageChops.difference(solution, image)

                solution.save(os.path.join(RESULT_DIR, 'rays.png'), "PNG", optimize=True)
                diff.save(os.path.join(RESULT_DIR, 'diff-rays.png'), "PNG", optimize=True)

            lock.release()

            return jsonify(id=id, success=STOP_FLAG)

    return render_template('index.html')

@socketio.on('connect', namespace='/test')
def test_connect():
    emit('system', {'data': 'Connected'})

@app.route('/_add_numbers', methods=['GET', 'POST'])
def add_numbers():
    """Add two numbers server side, ridiculous but well..."""
    a = request.args.get('a', 0, type=int)
    b = request.args.get('b', 0, type=int)
    return jsonify(result=a + b)

if __name__ == "__main__":
    #app.run(host='0.0.0.0', debug=True)
    socketio.run(app, host='0.0.0.0')
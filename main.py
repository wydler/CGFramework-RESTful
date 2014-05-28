import os
import re
import shutil
import subprocess
import threading

from datetime import datetime
from flask import Flask, request, redirect, url_for, jsonify, render_template
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
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

lock = threading.Lock()

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1] in ALLOWED_EXTENSIONS

@app.route("/bresenham", methods=['GET', 'POST'])
def index():
    print('index')
    if request.method == 'POST':
        lock.acquire()

        id = datetime.now().strftime('%s-%f')
        RESULT_DIR = os.path.join(TEST_DIR, 'static', 'bresenham', 'results', id)
        os.makedirs(RESULT_DIR)
        print('post')
        files = request.files.getlist("files[]")
        file = files[0]
        print(file)
        if file and allowed_file(file.filename):
            filename = secure_filename(id+'_'+file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))

            new = ''
            with open(os.path.join(app.config['UPLOAD_FOLDER'], filename), 'r') as source:
                new = re.sub('int main\(', 'int _main(', source.read())
            with open(os.path.join(ROOT_DIR, 'main.imagebuffer.cpp'), 'w') as fout, open(os.path.join(TEST_DIR, 'bresenham', 'test.cpp'), 'r') as test:
                fout.write(new)
                fout.write(test.read())

            ret = subprocess.Popen('cmake ..', shell=True, cwd=BUILD_DIR).wait()
            print('CMake exit code: '+str(ret))
            ret = subprocess.Popen('make', shell=True, cwd=BUILD_DIR).wait()
            print('Build exit code: '+str(ret))
            ret = subprocess.Popen('./example_imagebuffer', shell=True, cwd=BUILD_DIR).wait()
            print('Run exit code: '+str(ret))

            shutil.copy(os.path.join(BUILD_DIR, 'rand.bmp'), os.path.join(RESULT_DIR, 'rand.bmp'))
            shutil.copy(os.path.join(BUILD_DIR, 'rays.bmp'), os.path.join(RESULT_DIR, 'rays.bmp'))
            solution = Image.open(os.path.join(RESULT_DIR, 'rays.bmp')).resize([512, 512])
            image = Image.open(os.path.join(TEST_DIR, 'static', 'bresenham', 'solutions', 'rays.bmp')).resize([512, 512])
            diff = ImageChops.difference(solution, image)

            solution.save(os.path.join(RESULT_DIR, 'rays.png'), "PNG", optimize=True)
            diff.save(os.path.join(RESULT_DIR, 'diff-rays.png'), "PNG", optimize=True)

            lock.release()

            os.remove(os.path.join(app.config['UPLOAD_FOLDER'], filename))

            return jsonify(id=id)

    return render_template('index.html')

@app.route('/_add_numbers', methods=['GET', 'POST'])
def add_numbers():
    """Add two numbers server side, ridiculous but well..."""
    a = request.args.get('a', 0, type=int)
    b = request.args.get('b', 0, type=int)
    return jsonify(result=a + b)

if __name__ == "__main__":
    app.run(host='0.0.0.0', debug=True)
from flask import Flask, redirect, url_for, render_template, jsonify
import os
from dotenv import load_dotenv

load_dotenv()


app = Flask(__name__)

@app.route('/')
def index():
    return redirect(url_for('dashboard'))
    

@app.get('/dashboard')
def dashboard():
    return render_template('dashboard.html')

@app.post('/dashboard/<path>')
def devices(path):
    try:
        device_dir=os.getenv('DEVICE_DIR')
        device_dir=device_dir+path
        if os.path.isdir(device_dir):
            files = os.listdir(device_dir)
            return jsonify({'data': files})
        else:
            with open(device_dir, 'r') as file:
                data = file.read()
            return jsonify({'data': data})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.get('/content/<path>/<filename>')
def content(path, filename):
    device_dir=os.getenv('DEVICE_DIR')
    device_dir=device_dir+path

    data=''
    try:
        with open(device_dir+'/'+filename, 'r') as file:
            data = file.read()
        return jsonify({'data': data})
    except Exception as e:
        return data



if __name__=='__main__':
    app.run(debug=True)
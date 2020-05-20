import random
from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/")
def index():
    number = random.randint(1, 10)
    coin_toss = random.randint(0, 1)
    return render_template("index.html", name="Emma", number=number, coin=coin_toss)

@app.route("/goodbye")
def bye():
    return "Goodbye"

@app.route("/hello")
def hello():
    form_name = request.args.get("form_name")
    if not form_name:
        return render_template("failure.html")
    return render_template("hello.html", form_name=form_name)
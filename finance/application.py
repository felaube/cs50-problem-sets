import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get all trades made by the user
    trades = db.execute("SELECT symbol, shares, type FROM trades WHERE user_id=:id", id=session["user_id"])
    # Initialize list to store the shares and current price of each stock
    stocks = list()

    for trade in trades:
        stock_found = False
        for stock in stocks:
            # Check if the stock is already in "stocks"
            if trade["symbol"] == stock["symbol"]:
                stock_found = True
                # Just update the amount of shares
                if trade["type"] == "B":
                    stock["shares"] += trade["shares"]
                else:
                    stock["shares"] -= trade["shares"]
                break

        # Create new stock
        if not stock_found:
            if trade["type"] == "B":
                stocks.append({"symbol": trade["symbol"], "shares": trade["shares"]})
            else:
                stocks.append({"symbol": trade["symbol"], "shares": -trade["shares"]})

    patrimony = 0;

    for stock in stocks:
        price = lookup(stock["symbol"])
        stock["name"] = price["name"]
        stock["price"] = price["price"]
        stock["value"] = price["price"]*stock["shares"]

        patrimony += price["price"]*stock["shares"]

    cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    cash = cash[0]["cash"]

    patrimony += cash

    return render_template("index.html", stocks=stocks, cash="{:.2f}".format(cash), patrimony="{:.2f}".format(patrimony))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must provide stock symbol", 403)
        if not request.form.get("shares"):
            return apology("Must provide shares amount", 403)
        if float(request.form.get("shares")) <= 0 or not float(request.form.get("shares")).is_integer():
            return apology("The amount of shares must be an positive integer number", 403)

        price = lookup(request.form.get("symbol"))

        if price is None:
            return apology("Stock symbol was not found", 403)
        else:
            price = price["price"]

        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        cash = cash[0]["cash"]

        if price > cash:
            return apology("You do not have enough cash to buy the selected shares", 403)

        now = datetime.datetime.now()

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash-price*float(request.form.get("shares")), id=session["user_id"])
        db.execute("INSERT INTO trades VALUES(:symbol, :shares, :price, 'B', :datetime, :id)", symbol=request.form.get("symbol"),
                                                                                            shares=float(request.form.get("shares")),
                                                                                            price=price,
                                                                                            datetime=now.strftime("%Y-%m-%d %H:%M:%S"),
                                                                                            id=session["user_id"])

        return redirect("/")

    elif request.method == "GET":
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    trades = db.execute("SELECT * FROM trades WHERE user_id=:user_id ORDER BY datetime DESC", user_id=session["user_id"])

    for trade in trades:
        if trade["type"] == "B":
            trade["type"] = "Bought"
        if trade["type"] == "S":
            trade["type"] = "Sold"
        trade["value"] = trade["price"]*trade["shares"]

    return render_template("history.html", trades=trades)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        result = lookup(request.form.get("symbol"))
        if result is None:
            return apology("Stock symbol was not found", 403)
        return render_template("quoted.html", name=result["name"], price=result["price"], symbol=result["symbol"])
    elif request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username", 403)

        # Check if the username already exists
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if rows:
            return apology("Username already exists", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password", 403)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("Must provide password confirmation", 403)

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords don't match", 403)

        db.execute("INSERT INTO users(username, hash) VALUES (:username, :password)",
                    username=request.form.get("username"), password=generate_password_hash(request.form.get("password")))

        return redirect("/")

    # User reached route via GET (as by clicking a link)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        # Get all trades made by the user
        trades = db.execute("SELECT symbol, shares, type FROM trades WHERE user_id=:id", id=session["user_id"])
        # Initialize list to store the shares and current price of each stock
        stocks = list()

        for trade in trades:
            stock_found = False
            for stock in stocks:
                # Check if the stock is already in "stocks"
                if trade["symbol"] == stock["symbol"]:
                    stock_found = True
                    # Just update the amount of shares
                    if trade["type"] == "B":
                        stock["shares"] += trade["shares"]
                    else:
                        stock["shares"] -= trade["shares"]
                    break

            # Create new stock
            if not stock_found:
                if trade["type"] == "B":
                    stocks.append({"symbol": trade["symbol"], "shares": trade["shares"]})
                else:
                    stocks.append({"symbol": trade["symbol"], "shares": -trade["shares"]})

        return render_template("sell.html", stocks=stocks)

    elif request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Please select a symbol", 403)

        # Get all trades made by the user
        trades = db.execute("SELECT symbol, shares, type FROM trades WHERE user_id=:id", id=session["user_id"])
        # Initialize list to store the shares and current price of each stock
        stocks = list()

        for trade in trades:
            stock_found = False
            for stock in stocks:
                # Check if the stock is already in "stocks"
                if trade["symbol"] == stock["symbol"]:
                    stock_found = True
                    # Just update the amount of shares
                    if trade["type"] == "B":
                        stock["shares"] += trade["shares"]
                    else:
                        stock["shares"] -= trade["shares"]
                    break

            # Create new stock
            if not stock_found:
                if trade["type"] == "B":
                    stocks.append({"symbol": trade["symbol"], "shares": trade["shares"]})
                else:
                    stocks.append({"symbol": trade["symbol"], "shares": -trade["shares"]})

        # Check if the user has any share of the selected stock
        stock_found = False
        for stock in stocks:
            if stock["symbol"] == request.form.get("symbol"):
                if float(stock["shares"]) > 0:
                    stock_found = True
                    break
                else:
                    return apology("The user does not own any shares of the selected stock", 403)

        if not stock_found:
            return apology("The user does not own any shares of the selected stock", 403)

        if float(request.form.get("shares")) <= 0 or not float(request.form.get("shares")).is_integer():
            return apology("The amount of shares must be an positive integer number", 403)

        for stock in stocks:
            if request.form.get("symbol") == stock["symbol"]:
                if float(request.form.get("shares")) > stock["shares"]:
                    return apology("The selected amount of shares is larger than the owned amount of shares", 403)
                break

        price = lookup(request.form.get("symbol"))

        now = datetime.datetime.now()

        db.execute("INSERT INTO trades VALUES (:symbol, :shares, :price, 'S', :datetime, :user_id)",
                    symbol=request.form.get("symbol"), shares=request.form.get("shares"), price=price["price"],
                    datetime=now.strftime("%Y-%m-%d %H:%M:%S"), user_id=session["user_id"])

        cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        cash = cash[0]["cash"]

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash+price["price"]*float(request.form.get("shares")), id=session["user_id"])

        return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

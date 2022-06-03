import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]
    stocks = db.execute(
        'SELECT symbol, SUM(shares) as shares, price FROM transactions WHERE id=? GROUP BY symbol HAVING (SUM(shares)) > 0', session["user_id"])
    total = cash

    for stock in stocks:
        result = lookup(stock["symbol"])
        stock["name"] = result["name"]
        stock["total"] = stock["price"] * stock["shares"]
        total += stock["total"]

    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        result = lookup(symbol)

        if not result:
            return apology("Missing symbol", 400)

        try:
            shares = int(shares)
            if shares < 1:
                return apology("Too few shares", 400)
        except ValueError:
            return apology("Share must be positive integer", 400)

        rows = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
        cash = rows[0]["cash"]

        if cash < (shares * result["price"]):
            return apology("Can't afford", 400)

        db.execute("UPDATE users SET cash = cash - ? WHERE id=?", (shares * result["price"]), session["user_id"])
        db.execute("INSERT INTO transactions (id, symbol, shares, price, operation) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"], symbol.upper(), shares, result["price"], "buy")

        flash("Share successfully bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, price, date FROM transactions WHERE id=?", session["user_id"])

    for transaction in transactions:
        if transaction["shares"] < 0:
            transaction["price"] = transaction["price"] * (-1 * transaction["shares"])
        elif transaction["shares"] > 0:
            transaction["price"] = transaction["price"] * transaction["shares"]

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
        symbol = request.form.get("symbol")
        result = lookup(symbol)

        if not result:
            return apology("Invalid symbol: must provide symbol or symbol doesn't exist", 400)

        return render_template("quoted.html", name=result["name"], symbol=result["symbol"], price=usd(result["price"]))
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username or len(db.execute("SELECT username FROM users WHERE username = ?", username)) > 0:
            return apology("Invalid Username: must provide username or already exists", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must provide confirmation", 400)
        elif confirmation != password:
            return apology("provided password and confirmation must match", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Select a symbol", 400)

        if ((db.execute("SELECT SUM(shares) as shares FROM transactions WHERE symbol=? AND id=?", symbol, session["user_id"]))[0]["shares"] < 1):
            return apology("You don't own any shares of this stock", 400)

        shares = int(shares)
        if shares < 1:
            return apology("Invalid value of shares", 400)

        if ((db.execute("SELECT SUM(shares) as shares FROM transactions WHERE symbol=? AND id=?", symbol, session["user_id"]))[0]["shares"] < shares):
            return apology("You don't own this much shares of this stock", 400)

        price = lookup(symbol)["price"]
        db.execute("INSERT INTO transactions (id, symbol, shares, price, operation) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"], symbol.upper(), -shares, price, "sell")

        value = price * shares
        db.execute("UPDATE users SET cash = cash + ? WHERE id=?", value, session["user_id"])

        flash("Share successfully sold!")
        return redirect("/")
    else:
        stocks = db.execute("SELECT DISTINCT symbol FROM transactions WHERE id=?", session["user_id"])
        return render_template("sell.html", stocks=stocks)

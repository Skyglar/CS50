import os
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    transactions = db.execute("SELECT cash, company_name, company_symbol, price, SUM(shares) AS shares, SUM(shares*price) AS total"
                                +" FROM users"
                                +" JOIN transactions"
                                +" ON users.id = transactions.user_id"
                                +" WHERE users.id = :id"
                                +" GROUP BY company_name"
                                , id=session["user_id"])
    if not transactions:
        cash = db.execute("SELECT cash FROM users WHERE users.id = :id", id=session["user_id"])
        return render_template("index.html", cash=cash)
    total = transactions[0]["cash"]
    for i in range(len(transactions)):
        total += transactions[i]["total"]

    return render_template("index.html", transactions=transactions, total=usd(total))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol").lower()
        if not symbol:
            return apology("mising symbol", 400)
        data = lookup(symbol)
        if not data:
            return apology("unknown symbol", 400)
        return render_template("quoted.html", data=data)
    else:
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)
        elif not shares:
            return apology("missing shares", 400)
        elif not shares.isdigit() or not float(shares).is_integer() or int(shares) < 0:
            return apology("invalid input", 400)
        data = lookup(symbol)
        if not data:
            return apology("unknown symbol", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
        if cash < data["price"] * float(shares):
            return apology("not enough money", 400)
        else:
            cur_date = request.form.get("date")
            cur_time = request.form.get("time")
            if not cur_date or cur_time:
                cur_date = datetime.today().strftime('%Y-%m-%d')
                cur_time = datetime.today().strftime('%H:%M:%S')
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=(cash-data["price"]*float(shares)), id=session["user_id"])
            db.execute("INSERT INTO transactions (user_id, company_name, company_symbol, price, shares, date, time)" +
                        "VALUES (:user_id, :company_name, :symbol, :price, :shares, :date, :time)",
                        user_id=session["user_id"], company_name=data["name"], symbol=symbol, price=data["price"],
                        shares=int(shares), date=cur_date, time=cur_time)
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.args.get("username"))
    if rows:
        return jsonify(False)
    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT company_symbol, shares, price, date, time FROM transactions WHERE user_id = :id", id=session["user_id"])

    return render_template("history.html", transactions=rows)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("confrim your password", 400)
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords are different")
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                            username=username)
        if rows:
            return apology("user already exists")
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                    username = request.form.get("username"), hash = generate_password_hash(request.form.get("password")))
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not symbol:
            return apology("symbol was not choosen", 400)
        if not shares:
            return apology("shares is empty", 400)
        currentUserTransactions = db.execute("SELECT cash, company_symbol, SUM(shares) AS shares FROM users"
                                            +" JOIN transactions ON users.id = transactions.user_id"
                                            +" WHERE user_id = :user_id AND company_symbol = :symbol"
                                            +" GROUP BY company_name",
                                            user_id = session["user_id"], symbol=symbol)
        data = lookup(symbol)
        if shares > currentUserTransactions[0]["shares"]:
            return apology("not enough shares", 400)

        cur_date = request.form.get("date")
        cur_time = request.form.get("time")
        if not cur_date or cur_time:
            cur_date = datetime.today().strftime('%Y-%m-%d')
            cur_time = datetime.today().strftime('%H:%M:%S')
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                        cash=currentUserTransactions[0]["cash"] + (data["price"] * shares), id=session["user_id"])
        db.execute("INSERT INTO transactions (user_id, company_name, company_symbol, price, shares, date, time)" +
                        "VALUES (:user_id, :company_name, :symbol, :price, :shares, :date, :time)",
                        user_id=session["user_id"], company_name=data["name"], symbol=symbol, price=data["price"],
                        shares= -shares, date=cur_date, time=cur_time)
        return redirect("/")
    else:
        symbols = db.execute("SELECT company_symbol FROM transactions WHERE user_id = :id GROUP BY company_symbol"
                            , id=session["user_id"])
        return render_template("sell.html", symbols=symbols)

@app.route("/api/username")
def isUsernameExists():
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.args.get("username"))
    if len(rows) != 1:
        return jsonify({"text": False})

    return jsonify({"text": True})


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

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
    user = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])
    userCash = user[0]['cash']
    stocks = db.execute("SELECT * FROM userStock WHERE userId = :uid AND shares > 0", uid=session["user_id"])
    totalMoney = userCash
    for stock in stocks:
        totalMoney += stock['price'] * stock['shares']

    return render_template("index.html", totalMoney=totalMoney, stocks=stocks, userCash=userCash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = str(request.form.get("symbol"))
        shares = request.form.get("shares")

        if not shares.isnumeric():
            return apology("Provide integer!")

        shares = float(shares)

        if not shares.is_integer():
            return apology("Provide integer!")

        stock = lookup(symbol)
        user = db.execute("SELECT cash FROM users WHERE id=:uid", uid=session["user_id"])
        userCash = user[0]['cash']

        if not symbol:
            return apology("Provide symbol")
        if stock == None:
            return apology("Symbol does not exist")
        if not shares:
            return apology("Provide shares")
        if shares <= 0:
            return apology("Provide positive number of shares")
        if shares * stock['price'] > userCash:
            return apology("You cannot afford that")

        db.execute("INSERT INTO buy(userId,symbol,shares,price) VALUES(:uid,:symbol,:shares,:price)",
                   uid=session["user_id"], symbol=symbol, shares=shares, price=stock['price'])

        db.execute("INSERT INTO userStock(userId,symbol,price,shares,name,total) VALUES(:uid,:symbol,:price,:shares,:name,:total)",
                   uid=session["user_id"], symbol=symbol, price=stock['price'], shares=shares, name=stock['name'], total=stock['price']*shares)

        db.execute("UPDATE users SET cash = :newCash WHERE id=:uid",
                   newCash=userCash - stock['price'] * shares, uid=session['user_id'])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    sellStock = db.execute("SELECT * FROM sell WHERE userId=:uid", uid=session["user_id"])
    buyStock = db.execute("SELECT * FROM buy WHERE userId=:uid", uid=session["user_id"])
    return render_template("history.html", sellStock=sellStock, buyStock=buyStock)


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
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = str(request.form.get("symbol"))
        stock = lookup(symbol)

        if stock == None:
            return apology("Wrong symbol")

        return render_template("quoted.html", stock=stock)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    if request.method == "POST":
        if not username:
            return apology("must provide username", 403)
        elif not password:
            return apology("must provide password", 403)
        elif not confirmation:
            return apology("must provide confirmation", 403)
        elif password != confirmation:
            return apology("passwords must match", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) == 1:
            return apology("username already exists", 403)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        return login()
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        stock = lookup(symbol)

        if symbol == "symbol":
            return apology("Choose symbol!")
        if not shares:
            return apology("Provide shares!")
        if not shares.isnumeric():
            return apology("Provide integer shares!")

        shares = float(shares)

        if not shares.is_integer():
            return apology("Provide integer shares!")

        userStock = db.execute("SELECT * FROM userStock WHERE userId=:uid AND symbol=:symbol",
                               uid=session["user_id"], symbol=symbol)

        if shares > userStock[0]['shares']:
            return apology("You do not have that many shares!")

        user = db.execute("SELECT cash FROM users WHERE id=:uid", uid=session["user_id"])
        userCash = user[0]['cash']
        newShares = userStock[0]['shares'] - shares

        db.execute("INSERT INTO sell(userId,symbol,shares,price) VALUES(:uid,:symbol,:shares,:price)",
                   uid=session["user_id"], symbol=symbol, shares=shares, price=stock['price'])

        db.execute("UPDATE userStock SET shares = :newShares,total= :total WHERE userId=:uid AND symbol=:symbol",
                   symbol=symbol, uid=session["user_id"],  newShares=newShares, total=newShares * userStock[0]['price'])

        db.execute("UPDATE users SET cash = :newCash WHERE id=:uid",
                   newCash=userCash + stock['price'] * shares, uid=session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        userStocks = db.execute("SELECT * FROM userStock WHERE userId=:uid AND shares > 0", uid=session["user_id"])
        return render_template("sell.html", userStocks=userStocks)

#!/usr/bin/env python3
import os
import sys
from urllib.parse import parse_qs

method = os.environ.get("REQUEST_METHOD", "GET")

# Récupère les paramètres selon la méthode
if method == "POST":
    length = int(os.environ.get("CONTENT_LENGTH", 0))
    raw = sys.stdin.read(length) if length > 0 else ""
else:
    raw = os.environ.get("QUERY_STRING", "")

params = parse_qs(raw)
a     = params.get("a", [""])[0]
b     = params.get("b", [""])[0]
op    = params.get("op", [""])[0]

result = ""
error  = ""

if a and b and op:
    try:
        fa, fb = float(a), float(b)
        if op == "+":
            result = fa + fb
        elif op == "-":
            result = fa - fb
        elif op == "*":
            result = fa * fb
        elif op == "/":
            if fb == 0:
                error = "Division par zéro !"
            else:
                result = fa / fb
        else:
            error = "Opérateur inconnu : " + op
        if result != "":
            # Affiche entier si possible
            result = int(result) if result == int(result) else round(result, 10)
    except ValueError:
        error = "Valeurs invalides."

print("Content-Type: text/html")
print("")
print("<!DOCTYPE html>")
print("<html>")
print("<head><meta charset='UTF-8'><title>Calculatrice CGI</title>")
print("<style>")
print("  body { font-family: Arial, sans-serif; display: flex; justify-content: center; margin-top: 60px; background: #f0f0f0; }")
print("  .calc { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.2); width: 300px; }")
print("  h1 { text-align: center; font-size: 1.4em; }")
print("  input[type=number] { width: 100%; padding: 8px; margin: 6px 0; box-sizing: border-box; }")
print("  select { width: 100%; padding: 8px; margin: 6px 0; }")
print("  input[type=submit] { width: 100%; padding: 10px; background: #4CAF50; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 1em; }")
print("  input[type=submit]:hover { background: #45a049; }")
print("  .result { margin-top: 15px; text-align: center; font-size: 1.3em; font-weight: bold; color: #333; }")
print("  .error  { margin-top: 15px; text-align: center; color: red; font-weight: bold; }")
print("</style></head>")
print("<body><div class='calc'>")
print("  <h1>Calculatrice CGI</h1>")
print("  <form method='POST' action='/cgi-bin/hello.py'>")
print("    <label>Nombre A</label>")
print("    <input type='number' step='any' name='a' value='" + a + "' required>")
print("    <label>Opérateur</label>")
print("    <select name='op'>")
for sym in ["+", "-", "*", "/"]:
    sel = " selected" if op == sym else ""
    print("      <option value='" + sym + "'" + sel + ">" + sym + "</option>")
print("    </select>")
print("    <label>Nombre B</label>")
print("    <input type='number' step='any' name='b' value='" + b + "' required>")
print("    <input type='submit' value='Calculer'>")
print("  </form>")
if error:
    print("  <div class='error'>" + error + "</div>")
elif result != "":
    print("  <div class='result'>" + a + " " + op + " " + b + " = " + str(result) + "</div>")
print("</div></body></html>")

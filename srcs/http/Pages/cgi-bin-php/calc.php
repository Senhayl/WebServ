<?php
$a  = isset($_REQUEST['a'])  ? $_REQUEST['a']  : '';
$b  = isset($_REQUEST['b'])  ? $_REQUEST['b']  : '';
$op = isset($_REQUEST['op']) ? $_REQUEST['op'] : '';

$result = '';
$error  = '';

if ($a !== '' && $b !== '' && $op !== '') {
    $fa = floatval($a);
    $fb = floatval($b);
    if ($op === '+')      $result = $fa + $fb;
    elseif ($op === '-')  $result = $fa - $fb;
    elseif ($op === '*')  $result = $fa * $fb;
    elseif ($op === '/') {
        if ($fb == 0) $error = 'Division par zéro !';
        else          $result = $fa / $fb;
    } else {
        $error = 'Opérateur inconnu : ' . htmlspecialchars($op);
    }
    if ($result !== '') {
        $result = ($result == floor($result)) ? (int)$result : round($result, 10);
    }
}

function sel($op, $val) { return $op === $val ? ' selected' : ''; }
?>
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Calculatrice PHP CGI</title>
  <style>
    body { font-family: Arial, sans-serif; display: flex; justify-content: center; margin-top: 60px; background: #e8f4ff; }
    .calc { background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.2); width: 300px; }
    h1 { text-align: center; font-size: 1.4em; color: #0066cc; }
    .php-badge { text-align: center; font-size: 0.85em; color: #777; margin-bottom: 15px; }
    input[type=number] { width: 100%; padding: 8px; margin: 6px 0; box-sizing: border-box; border: 1px solid #ccc; border-radius: 4px; }
    select { width: 100%; padding: 8px; margin: 6px 0; border: 1px solid #ccc; border-radius: 4px; }
    input[type=submit] { width: 100%; padding: 10px; background: #0066cc; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 1em; margin-top: 8px; }
    input[type=submit]:hover { background: #0052a3; }
    .result { margin-top: 15px; text-align: center; font-size: 1.3em; font-weight: bold; color: #333; }
    .error  { margin-top: 15px; text-align: center; color: red; font-weight: bold; }
  </style>
</head>
<body>
<div class="calc">
  <h1>Calculatrice PHP CGI</h1>
  <p class="php-badge">PHP <?= phpversion() ?></p>
  <form method="POST" action="/cgi-bin-php/calc.php">
    <label>Nombre A</label>
    <input type="number" step="any" name="a" value="<?= htmlspecialchars($a) ?>" required>
    <label>Opérateur</label>
    <select name="op">
      <option value="+"<?= sel($op, '+') ?>>+</option>
      <option value="-"<?= sel($op, '-') ?>>-</option>
      <option value="*"<?= sel($op, '*') ?>>×</option>
      <option value="/"<?= sel($op, '/') ?>>÷</option>
    </select>
    <label>Nombre B</label>
    <input type="number" step="any" name="b" value="<?= htmlspecialchars($b) ?>" required>
    <input type="submit" value="Calculer">
  </form>
  <?php if ($error): ?>
    <div class="error"><?= htmlspecialchars($error) ?></div>
  <?php elseif ($result !== ''): ?>
    <div class="result"><?= htmlspecialchars($a) ?> <?= htmlspecialchars($op) ?> <?= htmlspecialchars($b) ?> = <?= $result ?></div>
  <?php endif; ?>
</div>
</body>
</html>

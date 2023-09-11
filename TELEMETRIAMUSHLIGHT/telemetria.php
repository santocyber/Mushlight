<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Telemetria GrANANET</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 10px;
            padding: 0;
        }
        h1 {
            text-align: center;
        }
        .container {
            display: flex;
            flex-direction: column;
        }
        .card {
            border: 1px solid #ddd;
            margin-bottom: 20px;
            padding: 10px;
        }
        table {
            border-collapse: collapse;
            width: 100%;
        }
        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
        select, input[type="text"] {
            width: 100%;
            padding: 5px;
            margin-bottom: 10px;
        }
        button {
            width: 100%;
            padding: 10px;
            background-color: #007BFF;
            color: #fff;
            border: none;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <h1>Telemetria GrANANET</h1>

    <!-- Formulário de busca -->
    <div class="search-container">
        <form method="get">
            <label for="searchTerm">Buscar por Identificador, MAC, Nome, Wi-Fi Local, Ações ou Parceiros:</label>
            <input type="text" id="searchTerm" name="searchTerm" required>
            <button type="submit">Buscar</button>
        </form>
    </div>
    <br>
    <!-- Botão para listar todas as linhas -->
    <div class="search-container">
        <form method="get">
            <input type="hidden" name="listAll" value="1">
            <button type="submit">Listar Tudo</button>
        </form>
    </div>

    <div class="container">
        <?php
        try {
            $pdo = new PDO('mysql:host=santocyber.helioho.st;dbname=santocyber_mushlight', 'santocyber_mushlight', 'telemetria');
            $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

            if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['mac'], $_POST['action'])) {
                $mac = $_POST['mac'];
                $action = $_POST['action'];

                // Atualize a coluna 'acoes' na tabela 'telemetria' com a ação selecionada
                $stmt = $pdo->prepare("UPDATE telemetria SET acoes = :acoes WHERE mac = :mac");
                $stmt->bindParam(':acoes', $action, PDO::PARAM_STR);
                $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
                $stmt->execute();

                // Verifique se a ação é "mensagem"
                if ($action === 'mensagem') {
                    // Obtém o valor de crédito inserido pelo usuário
                    $msg = $_POST['mensagem'];

                    // Atualize a coluna 'mensagem' na tabela 'telemetria' com o valor de crédito
                    $stmt = $pdo->prepare("UPDATE telemetria SET mensagem = :mensagem WHERE mac = :mac");
                    $stmt->bindParam(':mensagem', $msg, PDO::PARAM_STR);
                    $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
                    $stmt->execute();
                }
            }

            // Construir a consulta base
            $query = "SELECT identificador, nome, ip, mac, timelocal, wifilocal, last_update, acoes, mensagem, geolocalizacao, estado, imagem FROM telemetria";

            if ($_SERVER['REQUEST_METHOD'] === 'GET') {
                if (isset($_GET['searchTerm'])) {
                    $searchTerm = $_GET['searchTerm'];
                    $query .= " WHERE mac LIKE :searchTerm OR nome LIKE :searchTerm OR wifilocal LIKE :searchTerm OR identificador LIKE :searchTerm OR acoes LIKE :searchTerm OR parceiros LIKE :searchTerm";
                } elseif (isset($_GET['listAll'])) {
                    // Se a opção de listar todas for selecionada, não é necessário modificar a consulta
                }
            }

            $stmt = $pdo->prepare($query);

            if (isset($searchTerm)) {
                $stmt->bindValue(':searchTerm', '%' . $searchTerm . '%', PDO::PARAM_STR);
            }

            $stmt->execute();

            while ($linha = $stmt->fetch(PDO::FETCH_ASSOC)) {
                echo "<div class='card'>";
                echo "<table>";
                echo "<tr><th>Informações</th><th>Ação</th></tr>";
                echo "<tr><td>Registro: {$linha['identificador']}</td><td rowspan='3'><form method='POST'><input type='hidden' name='mac' value='{$linha['mac']}'><select name='action'><option value='ligado'>Ligar</option><option value='desligado'>Desligar</option><option value='rainbow'>Rainbow</option><option value='mensagem'>Mensagem</option></select>";
                echo "<input type='text' name='mensagem' placeholder='Valor de crédito'>";
                echo "<button type='submit'>Executar Ação</button></form></td></tr>";
                echo "<tr><td>Nome: {$linha['nome']}</td></tr>";
                echo "<tr><td>Endereço MAC: {$linha['mac']}</td></tr>";
                echo "<tr><td>IP local: {$linha['ip']}</td></tr>";
                echo "<tr><td>WiFilocal: {$linha['wifilocal']}</td></tr>";
                echo "<tr><td>Hora no local: {$linha['timelocal']}</td></tr>";
                echo "<tr><td>Última Atualização: {$linha['last_update']}</td></tr>";
                echo "<tr><td>Mensagem: {$linha['mensagem']}</td></tr>";
                echo "<tr><td>Geolocalização: {$linha['geolocalizacao']}</td></tr>";
                echo "<tr><td>Imagem:</td></tr>"; // Adicione uma linha para mostrar a imagem
                echo "<tr><td colspan='2'><img src='data:image/jpeg;base64," . base64_encode($linha['imagem']) . "' width='800'></td></tr>"; // Mostra a imagem
                echo "</table>";
                echo "</div>";
            }

        } catch (PDOException $e) {
            echo "Erro: " . $e->getMessage();
        }
        ?>
    </div>
</body>
</html>

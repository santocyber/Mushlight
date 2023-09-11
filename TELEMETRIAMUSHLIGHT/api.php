<?php
header('Content-Type: application/json');

try {
    // Configurar as informações de conexão ao banco de dados
    $host = 'santocyber.helioho.st';
    $dbname = 'santocyber_mushlight';
    $username = 'santocyber_mushlight';
    $password = 'telemetria';

    // Conectar ao banco de dados
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    // Verificar se o parâmetro 'mac' está presente na solicitação GET
    if (isset($_GET['mac'])) {
        $mac = $_GET['mac'];

        // Usar uma instrução SQL SELECT para obter os valores das colunas 'mensagem' e 'acoes'
        $stmt = $pdo->prepare("SELECT mensagem, acoes FROM telemetria WHERE mac = :mac");
        $stmt->bindParam(':mac', $mac, PDO::PARAM_STR);
        $stmt->execute();

        // Verificar se há resultados
        $row = $stmt->fetch(PDO::FETCH_ASSOC);

        if ($row !== false) {
            $mensagem = $row['mensagem'];
            $acoes = $row['acoes'];

            // Responder com os valores das colunas 'mensagem' e 'acoes' em formato JSON
            echo json_encode(['message' => 'Valores das colunas "mensagem" e "acoes" para o MAC ' . $mac . ' foram obtidos', 'mensagem' => $mensagem, 'acoes' => $acoes]);
        } else {
            echo json_encode(['error' => 'Não foi possível encontrar um registro com o MAC especificado']);
        }
    } else {
        echo json_encode(['error' => 'O parâmetro "mac" não foi especificado']);
    }
} catch (PDOException $e) {
    echo json_encode(['error' => 'Erro: ' . $e->getMessage()]);
}
?>

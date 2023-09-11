<?php
// Configurações do banco de dados
$servername = "santocyber.helioho.st";
$username = "santocyber_mushlight";
$password = "telemetria";
$dbname = "santocyber_mushlight";

// Conexão com o banco de dados usando a extensão mysqli
$conn = new mysqli($servername, $username, $password, $dbname);

// Verificar a conexão
if ($conn->connect_error) {
    die("Conexão falhou: " . $conn->connect_error);
}

// Verificar se o parâmetro 'mac' está presente na URL
if (isset($_GET['mac'])) {
    // Capturar e sanitizar o MAC address da URL
    $mac = sanitizeInput($_GET['mac']);

    // Verificar se os dados binários da imagem estão presentes no corpo da solicitação POST
    $imageData = file_get_contents('php://input');

    $imageData = mysqli_escape_string($conn, $imageData);

    if ($imageData !== false) {

        // Atualizar a coluna "imagem" na tabela "telemetria" com base no MAC
        $updateSql = "UPDATE telemetria SET imagem = '$imageData' WHERE mac = '$mac'";
        $stmt = $conn->prepare($updateSql);

        if ($stmt->execute()) {
            echo "Imagem atualizada com sucesso na tabela telemetria para o MAC $mac.";
        } else {
            echo "Erro ao atualizar a imagem na tabela telemetria: " . $stmt->error;
        }
    } else {
        echo "Dados binários da imagem não foram fornecidos na solicitação POST.";
    }
} else {
    echo "Parâmetro 'mac' não foi fornecido na URL.";
}

// Fechar a conexão
$conn->close();

function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}
?>

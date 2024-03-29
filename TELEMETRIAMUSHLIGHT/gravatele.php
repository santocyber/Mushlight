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

// Função para validar e sanitizar os dados
function sanitizeInput($input) {
    return htmlspecialchars(stripslashes(trim($input)));
}

// Verificar se os parâmetros necessários estão presentes na URL
if (
	isset($_GET['mac_value']) &&
    isset($_GET['iplocal']) &&
    isset($_GET['wifilocal']) &&
    isset($_GET['timelocal']) &&
    
    isset($_GET['nomedobot']) &&
    isset($_GET['geo']) 
) {
    // Capturar e sanitizar os dados da URL
    $ip = sanitizeInput($_GET['iplocal']);
    $timelocal = sanitizeInput($_GET['timelocal']);
    $wifilocal = sanitizeInput($_GET['wifilocal']);
    $mac_value = sanitizeInput($_GET['mac_value']);
	$nomedobot = sanitizeInput($_GET['nomedobot']);
	$geo = sanitizeInput($_GET['geo']);
    $last_update = date('Y-m-d H:i:s');

    // Verificar se a linha já existe na tabela com base no endereço MAC
    $checkSql = "SELECT * FROM telemetria WHERE mac = '$mac_value'";
    $result = $conn->query($checkSql);

    if ($result->num_rows > 0) {
        // A linha já existe, então vamos atualizá-la
        $updateSql = "UPDATE telemetria 
                      SET ip = '$ip',wifilocal = '$wifilocal', nome = '$nomedobot', geolocalizacao = '$geo', timelocal = '$timelocal' WHERE mac = '$mac_value'";

        if ($conn->query($updateSql) === TRUE) {
            echo "Dados atualizados com sucesso na tabela";
        } else {
            echo "Erro ao atualizar dados na tabela: " . $conn->error . "<br>";
        }
    } else {
        // A linha não existe, então vamos inseri-la
        $insertSql = "INSERT INTO telemetria (mac, ip , wifilocal, timelocal, nome, geolocalizacao)
                      VALUES ('$mac_value', '$ip', '$wifilocal', '$timelocal', '$nomedobot', '$geo')";

        if ($conn->query($insertSql) === TRUE) {
            echo "Dados inseridos com sucesso na tabela";
        } else {
            echo "Erro ao inserir dados na tabela: " . $conn->error . "<br>";
        }
    }
} else {
    echo "Parâmetros necessários não foram fornecidos na URL";
}

// Fechar a conexão
$conn->close();
?>

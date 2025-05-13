const form = document.getElementById("formularioComando");
const resposta = document.getElementById("resposta");

form.addEventListener("submit", function (e) {
    e.preventDefault();
    const comando = document.getElementById("comando").value.toUpperCase();

    fetch(`/enviar_serial/${comando}/`)
        .then(response => response.json())
        .then(data => {
            resposta.textContent = data.mensagem;
            resposta.style.color = "green";
        })
        .catch(error => {
            resposta.textContent = "Servidor desconectado!";
            resposta.style.color = "red";
            console.error(error);
        });
    document.getElementById("comando").value ="";
});
document.getElementById("submitBtn").addEventListener("click", fetchData);
document.getElementById("keyInput").addEventListener("keypress", function(event) {
    if (event.key === "Enter") {
        fetchData();
    }
});

function fetchData() {
    let keyInput = document.getElementById("keyInput");
    let key = keyInput.value.trim();
    let resultBox = document.getElementById("result");
    let loader = document.getElementById("loader");

    if (!key) {
        keyInput.classList.add("error-animation");
        setTimeout(() => keyInput.classList.remove("error-animation"), 500);
        resultBox.innerHTML = "<span style='color: red;'>Please enter a valid key.</span>";
        return;
    }

    resultBox.innerHTML = "";
    loader.style.display = "block";

    fetch("http://localhost:8080/lookup?key=" + encodeURIComponent(key))
        .then(response => response.text())
        .then(data => {
            loader.style.display = "none";
            if (data.includes("Key not found")) {
                keyInput.classList.add("error-animation");
                setTimeout(() => keyInput.classList.remove("error-animation"), 500);
                keyInput.value = "";
                resultBox.innerHTML = "<span style='color: red;'>Key not found ❌</span>";
            } else {
                document.body.innerHTML = `
                    <div class='container'>
                        <h1>💻 Retrieved Data</h1>
                        <p style='color: #ffeb3b; font-size: 18px;'>${data}</p>
                        <button id='backBtn'>⬅ Go Back</button>
                    </div>
                `;
                document.getElementById("backBtn").addEventListener("click", goHome);
            }
        })
        .catch(error => {
            console.error("Error:", error);
            loader.style.display = "none";
            resultBox.innerHTML = "<span style='color: red;'>Server is not running. Please start the application or downloads Server.</span>";
        });
}

function goHome() {
    location.reload();
}

document.addEventListener("keydown", function(event) {
    if (event.key === "Enter" && document.getElementById("backBtn")) {
        goHome();
    }
});

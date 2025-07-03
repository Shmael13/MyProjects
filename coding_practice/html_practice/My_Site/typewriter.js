document.addEventListener("DOMContentLoaded", () => {
  const text = "Ismail Syed Portfolio Website";
  const target = document.getElementById("typewriter");
  let index = 0;

  function typeNextChar() {
    if (index < text.length) {
      target.textContent += text.charAt(index);
      index++;
      setTimeout(typeNextChar, 100);
    }
  }

  typeNextChar();
});


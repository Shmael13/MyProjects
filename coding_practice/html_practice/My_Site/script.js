// Theme toggle with localStorage
const toggleBtn = document.getElementById('theme-toggle');
const body = document.getElementById('body');

if (localStorage.getItem('theme') === 'dark') {
  body.classList.add('dark-mode');
  toggleBtn.textContent = 'Toggle Light Mode';
}

toggleBtn.onclick = () => {
  body.classList.toggle('dark-mode');
  const isDark = body.classList.contains('dark-mode');
  toggleBtn.textContent = isDark ? 'Toggle Light Mode' : 'Toggle Dark Mode';
  localStorage.setItem('theme', isDark ? 'dark' : 'light');
};

// Active nav highlight
const links = document.querySelectorAll('nav a');
window.addEventListener('scroll', () => {
  const scrollPos = window.scrollY;
  links.forEach(link => {
    const section = document.querySelector(link.getAttribute('href'));
    if (section && section.offsetTop <= scrollPos + 60 && section.offsetTop + section.offsetHeight > scrollPos + 60) {
      links.forEach(l => l.classList.remove('active'));
      link.classList.add('active');
    }
  });
});

// Removed: old dynamic project injection

// Back to top button
const backToTopBtn = document.getElementById('backToTop');
window.onscroll = () => {
  backToTopBtn.style.display = window.scrollY > 300 ? 'block' : 'none';
};
backToTopBtn.onclick = () => window.scrollTo({ top: 0, behavior: 'smooth' });

// Fade-in sections on scroll
const observer = new IntersectionObserver((entries) => {
  entries.forEach(entry => {
    if (entry.isIntersecting) {
      entry.target.classList.add('visible');
    }
  });
}, { threshold: 0.1 });

document.querySelectorAll('section').forEach(section => {
  section.classList.add('fade-in');
  observer.observe(section);
});

const cursor = document.querySelector('.cursor');
const trail = document.querySelector('.cursor-trail');

document.addEventListener('mousemove', e => {
  cursor.style.transform = `translate(${e.clientX}px, ${e.clientY}px)`;
  trail.style.transform = `translate(${e.clientX}px, ${e.clientY}px)`;
});

const hoverTargets = document.querySelectorAll('a, button');

hoverTargets.forEach(el => {
  el.addEventListener('mouseenter', () => {
    cursor.style.transform += ' scale(1.5)';
    trail.style.opacity = '0.5';
  });
  el.addEventListener('mouseleave', () => {
    cursor.style.transform = cursor.style.transform.replace(' scale(1.5)', '');
    trail.style.opacity = '1';
  });
});



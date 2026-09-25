// Shared behaviour: menu, header, scroll progress, reveals, and the homepage's scroll-driven pieces.
(function () {
  var doc = document.documentElement;
  doc.classList.add('js');
  var reduce = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  // Hero entrance
  requestAnimationFrame(function () { requestAnimationFrame(function () { doc.classList.add('is-loaded'); }); });

  // Mobile menu
  var toggle = document.querySelector('.nav__toggle'), menu = document.getElementById('menu');
  if (toggle && menu) {
    toggle.addEventListener('click', function () {
      var open = toggle.getAttribute('aria-expanded') !== 'true';
      toggle.setAttribute('aria-expanded', String(open));
      menu.classList.toggle('is-open', open);
    });
    menu.addEventListener('click', function (e) {
      if (e.target.tagName === 'A') { toggle.setAttribute('aria-expanded', 'false'); menu.classList.remove('is-open'); }
    });
  }

  // Header hairline, reading progress, timeline fill
  var nav = document.querySelector('.nav'), bar = document.getElementById('progress'), tl = document.querySelector('.tl');
  function onScroll() {
    var y = window.scrollY, max = doc.scrollHeight - innerHeight;
    if (nav) nav.classList.toggle('is-scrolled', y > 8);
    if (bar) bar.style.transform = 'scaleX(' + (max > 0 ? Math.min(1, y / max) : 0) + ')';
    if (tl) {
      var r = tl.getBoundingClientRect(), seen = Math.max(0, Math.min(r.height, innerHeight * 0.65 - r.top));
      tl.style.setProperty('--fill', seen + 'px');
    }
  }
  window.addEventListener('scroll', onScroll, { passive: true }); window.addEventListener('resize', onScroll); onScroll();

  var y = document.getElementById('y'); if (y) y.textContent = new Date().getFullYear();

  // Reveal on scroll
  var items = document.querySelectorAll('.rv, [data-reveal], .courtdraw');
  document.querySelectorAll('[data-reveal]').forEach(function (el) { el.classList.add('reveal'); });
  if ('IntersectionObserver' in window && !reduce) {
    var io = new IntersectionObserver(function (entries) {
      entries.forEach(function (en) { if (en.isIntersecting) { en.target.classList.add('is-in'); io.unobserve(en.target); } });
    }, { rootMargin: '0px 0px -10% 0px' });
    items.forEach(function (el) { io.observe(el); });
  } else items.forEach(function (el) { el.classList.add('is-in'); });

  // Court lines draw themselves
  document.querySelectorAll('.courtdraw .draw').forEach(function (el) {
    try { el.style.setProperty('--len', Math.ceil(el.getTotalLength()) + 1); } catch (e) {}
  });

  // Work: sticky stage that follows the project you are reading
  var stage = document.getElementById('stage'), pjs = document.querySelectorAll('.pj');
  if (stage && pjs.length) {
    var frames = [];
    pjs.forEach(function (pj, i) {
      var f = pj.querySelector('.frame'); if (!f) return;
      var c = f.cloneNode(true); c.querySelectorAll('img').forEach(function (im) { im.loading = 'eager'; im.alt = ''; });
      if (i === 0) c.classList.add('is-active');
      stage.appendChild(c); frames[i] = c;
    });
    var io2 = new IntersectionObserver(function (entries) {
      entries.forEach(function (en) {
        if (!en.isIntersecting) return;
        var i = +en.target.dataset.i;
        frames.forEach(function (f, j) { if (f) f.classList.toggle('is-active', j === i); });
      });
    }, { rootMargin: '-45% 0px -45% 0px' });
    pjs.forEach(function (pj) { io2.observe(pj); });
  }

  // Copy email
  var copy = document.getElementById('copy');
  if (copy) copy.addEventListener('click', function () {
    var label = document.getElementById('copyLabel'), mail = copy.dataset.email;
    function done() { label.textContent = 'Copied'; setTimeout(function () { label.textContent = 'Copy'; }, 1800); }
    if (navigator.clipboard) navigator.clipboard.writeText(mail).then(done, function () { location.href = 'mailto:' + mail; });
    else location.href = 'mailto:' + mail;
  });
})();

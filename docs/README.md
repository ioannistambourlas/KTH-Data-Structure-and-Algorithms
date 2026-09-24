# Ioannis Tambourlas — portfolio

Plain HTML and CSS, no build step. Served by GitHub Pages from this `docs/` folder.

```
docs/
├── index.html                    the portfolio
├── styles.css
├── Ioannis-Tambourlas-CV.pdf     résumé linked from the site (generated from cv.html)
├── cv.html                       source for the PDF (A4, print-ready)
├── assets/
│   ├── favicon.svg
│   ├── portrait.jpg              ← add your photo here (4:5, ~900×1125)
│   └── shots/                    project screenshots, taken from the demos
└── demos/
    ├── futsalaki/                Futsalaki venue dashboard — interactive, sample data
    └── student-market/           SSSB Market — interactive, sample data
```

## Photo

Save your headshot as `docs/assets/portrait.jpg`. Until it exists the hero shows an empty photo frame.

## Publish

Repo → Settings → Pages → Source: *Deploy from a branch* → Branch `main`, folder `/docs`.

## Preview locally

```bash
cd docs && python3 -m http.server 8000   # http://localhost:8000
```

Both demos run entirely in the browser; nothing is sent to a server.

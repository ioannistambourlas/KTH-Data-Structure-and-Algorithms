# ioannistambourlas — personal site

Plain HTML and CSS, no build step. Served by GitHub Pages from this `docs/` folder.

```
docs/
├── index.html                   the site
├── styles.css
├── assets/
│   ├── favicon.svg
│   └── portrait.jpg             ← add your photo here (4:5, ~800×1000)
└── demos/
    ├── futsalaki/               interactive Futsalaki demo (sample data)
    └── student-market/          interactive SSSB Market demo (sample data)
```

## Photo

Save your LinkedIn / CV photo as `docs/assets/portrait.jpg`. Until it exists the page shows an "IT" monogram in the same spot.

## Publish

Repo → Settings → Pages → Source: *Deploy from a branch* → Branch `main`, folder `/docs`.
The site appears at `https://ioannistambourlas.github.io/KTH-Data-Structure-and-Algorithms/`.
A custom domain can be added on the same settings page.

## Preview locally

```bash
cd docs && python3 -m http.server 8000   # http://localhost:8000
```

The demos run entirely in the browser. Listings you post in the market demo are kept in your browser's localStorage only.

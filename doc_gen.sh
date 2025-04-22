pandoc doc_ch.md -f markdown -t html -o doc.html  --metadata title="王德宇_笃实41_2024013267_$(date +%Y-%m-%d)"
pandoc doc_ch.md -f markdown -t pdf -o doc.pdf --pdf-engine=xelatex -V mainfont='JetBrains Mono' -V CJKmainfont='Noto Serif CJK SC'

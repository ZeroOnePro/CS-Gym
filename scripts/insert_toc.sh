echo '<!--ts-->\n<!--te-->\n' | sed -i '' '2r/dev/stdin' README.md && ~/gh-md-toc --insert README.md
find . -name "README.md.*" -delete
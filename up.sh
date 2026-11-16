#!/bin/bash
dr="$(cd "$(dirname "${BASH_SOURCE[0]}")"&&pwd)"
bn="$HOME/.local/bin"
fl="$bn/zhanode"
sr="$dr/sursi/baza.c"
mkdir -p "$bn"
if [ ! -x "$fl" ]||[ "$sr" -nt "$fl" ];then
gcc -O2 -std=c11 -Wall -Wextra "$sr" -o "$fl"||exit 1
echo "обноалвенонроеыпрвадпукрпукркуыр"
else
echo "оккккккккккккккккккк"
fi
case ":$PATH:" in
*":$bn:"*) ;;
*) export PATH="$bn:$PATH";;
esac
if ! grep -q ".local/bin" "$HOME/.bashrc" 2>/dev/null;then
echo 'export PATH="$HOME/.local/bin:$PATH"'>>"$HOME/.bashrc"
fi
if [ "${BASH_SOURCE[0]}" = "$0" ];then
echo "укажи source ./up.sh"
fi

#!/usr/bin/env bash
# Trim trailing spaces and tabs from files (in-place).
# Usage: trim_trailing_whitespace.sh [-b] FILE...
#   -b  create a backup for each file as <file>.bak

set -euo pipefail

# Detect sed in-place behavior (GNU vs BSD/macOS)
if sed --version >/dev/null 2>&1; then
  SED_INPLACE=(-i)
else
  # BSD sed (macOS) requires a backup suffix argument to -i, using empty string
  SED_INPLACE=(-i '')
fi

usage() {
  echo "Usage: $0 [-b] FILE..."
  exit 1
}

BACKUP=false
while getopts ":b" opt; do
  case $opt in
    b) BACKUP=true ;;
    *) usage ;;
  esac
done
shift $((OPTIND - 1))

if [ $# -lt 1 ]; then
  usage
fi

for file in "$@"; do
  if [ ! -f "$file" ]; then
    echo "Skipping: '$file' (not a regular file)" >&2
    continue
  fi
  if [ ! -r "$file" ]; then
    echo "Skipping: '$file' (not readable)" >&2
    continue
  fi
  if [ ! -w "$file" ]; then
    echo "Error: '$file' is not writable" >&2
    continue
  fi

  # skip non-text files when 'file' utility is available
  if command -v file >/dev/null 2>&1; then
    mime=$(file -b --mime-type -- "$file" 2>/dev/null || echo "")
    case "$mime" in
      text/*|application/json|application/xml) ;;
      "") ;;
      *) echo "Skipping: '$file' (non-text mime: $mime)" >&2; continue ;;
    esac
  fi

  if [ "$BACKUP" = true ]; then
    bak="$file.bak"
    if [ -e "$bak" ]; then
      bak="$file.bak.$(date +%s)"
    fi
    cp -p -- "$file" "$bak"
    sed "${SED_INPLACE[@]}" -E 's/[[:blank:]\r]*$//' "$file"
    echo "Trimmed trailing spaces/tabs in '$file' (backup: '$bak')"
  else
    sed "${SED_INPLACE[@]}" -E 's/[[:blank:]\r]*$//' "$file"
    echo "Trimmed trailing spaces/tabs in '$file'"
  fi
done

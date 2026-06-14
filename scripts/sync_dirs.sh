#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<EOF
Usage: $0 [--dry-run] [--no-delete] [--reverse] SRC_DIR DEST_DIR

Synchronize the contents of SRC_DIR into DEST_DIR (make DEST_DIR mirror SRC_DIR).

Options:
  --dry-run, -n    Show what would change without modifying files
  --no-delete      Do not remove files in DEST that are absent in SRC
  --reverse        Swap roles: synchronize DEST into SRC instead
  -h, --help       Show this help
EOF
}

DRY_RUN=false
DELETE=true
REVERSE=false
SRC=""
DEST=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --dry-run|-n) DRY_RUN=true; shift ;;
    --no-delete) DELETE=false; shift ;;
    --reverse) REVERSE=true; shift ;;
    -h|--help) usage; exit 0 ;;
    --) shift; break ;;
    *)
      if [[ -z "$SRC" ]]; then
        SRC="$1"
      elif [[ -z "$DEST" ]]; then
        DEST="$1"
      else
        echo "Unexpected argument: $1" >&2; usage; exit 2
      fi
      shift
      ;;
  esac
done

if [[ -z "$DEST" || -z "$SRC" ]]; then
  echo "Error: SRC_DIR and DEST_DIR are required." >&2
  usage
  exit 2
fi

if $REVERSE; then
  # Swap so that semantics remain: sync SRC -> DEST; reverse flips them
  tmp="$DEST"; DEST="$SRC"; SRC="$tmp"
fi

if ! command -v rsync >/dev/null 2>&1; then
  echo "rsync is required but not found. Install rsync and retry." >&2
  exit 3
fi

# Ensure destination exists
mkdir -p -- "$DEST"

RSYNC_OPTS=( -a --links --times --perms )
# Preserve owner/group when run as root; harmless otherwise
RSYNC_OPTS+=( --owner --group --devices --specials )
if $DELETE; then
  RSYNC_OPTS+=( --delete --delete-excluded )
fi
if $DRY_RUN; then
  RSYNC_OPTS+=( -n --itemize-changes )
fi

echo "Synchronizing '$SRC' -> '$DEST'"
if $DRY_RUN; then
  echo "(dry run)"
fi

# Trailing slash on SRC copies contents of directory rather than directory itself
rsync "${RSYNC_OPTS[@]}" --progress -- "$SRC/" "$DEST/"

echo "Done."

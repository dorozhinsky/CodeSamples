#!/bin/bash
# Usage: ./replace_tabs.sh <file> [tab_width]
# Default tab width is 4 spaces

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <file> [tab_width]"
    exit 1
fi

FILE="$1"
TAB_WIDTH="${2:-4}"

if ! [[ "$TAB_WIDTH" =~ ^[0-9]+$ ]]; then
    echo "Tab width must be a positive integer."
    exit 2
fi

# Create a string of spaces of length TAB_WIDTH
SPACES=""
for ((i=0; i<TAB_WIDTH; i++)); do
    SPACES+=" "
done

# Use sed to replace tabs with the specified number of spaces
sed -i.bak $'s/\t/'"$SPACES"'/g' "$FILE"
echo "Tabs replaced with $TAB_WIDTH spaces in $FILE (backup: $FILE.bak)"

#!/bin/bash

MESSAGE=$1

if [ -z "$MESSAGE" ]; then
  echo "Error: Please provide a commit message."
  echo "Usage: ./antigravity.sh 'your commit message'"
  exit 1
fi

# Check if the repository has any commits yet
if git rev-parse HEAD >/dev/null 2>&1; then
  # Get the last commit date in Unix epoch seconds
  LAST_DATE_EPOCH=$(git log -1 --format=%ct)
  # Add exactly 24 hours (86400 seconds) to advance to the next day
  NEW_DATE_EPOCH=$((LAST_DATE_EPOCH + 86400))
  # Format back to an ISO timestamp
  NEW_DATE=$(date -d @"$NEW_DATE_EPOCH" +"%Y-%m-%dT%H:%M:%S")
else
  # If this is the very first commit, start on August 15, 2026
  NEW_DATE="2026-08-15T10:00:00"
fi

echo "🚀 Antigravity Sequence Initiated..."
echo "Staging files and committing for date: $NEW_DATE"

# Execute the backdated commit and push
git add .
GIT_AUTHOR_DATE="$NEW_DATE" GIT_COMMITTER_DATE="$NEW_DATE" git commit -m "$MESSAGE"

echo "✅ Commit successful (push skipped)."
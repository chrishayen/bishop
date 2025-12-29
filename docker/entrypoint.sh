#!/bin/bash
set -e

echo "Generating GitHub App token..."
export GH_TOKEN=$(/home/agent/.local/bin/gh-token generate --key /home/agent/app-private-key.pem --app-id "$GH_APP_ID" --installation-id "$GH_INSTALLATION_ID" --token-only)

git config --global user.name "$GIT_NAME"
git config --global user.email "$GIT_EMAIL"

echo "Cloning $REPO..."
gh repo clone "$REPO" /workspace/repo
cd /workspace/repo

echo "Starting Claude Code..."
claude --dangerously-skip-permissions -p "$TASK"

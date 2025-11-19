#!/usr/bin/env bash
# Stop ProjectXS Launcher and watchdog processes safely.
# Usage: ./stop-launcher.sh

set -eu
LAUNCHER_DIR="$(cd "$(dirname "$0")/.." && pwd)"
PATTERN="$LAUNCHER_DIR"
PIDFILE="$LAUNCHER_DIR/launcher_watchdog.pid"

echo "Launcher dir: $LAUNCHER_DIR"

# Find processes that reference the launcher directory
mapfile -t PIDS < <(pgrep -af "$PATTERN" | awk '{print $1}') || true

if [ ${#PIDS[@]} -eq 0 ]; then
  echo "No launcher-related processes found."
else
  echo "Found launcher-related PIDs: ${PIDS[*]}"
  for pid in "${PIDS[@]}"; do
    # Avoid killing this script or init
    if [ "$pid" -eq $$ ] || [ "$pid" -le 2 ]; then
      continue
    fi
    echo "Sending SIGTERM to $pid"
    kill "$pid" 2>/dev/null || true
  done

  # Give graceful time, then force kill remaining
  sleep 1
  mapfile -t REMAIN < <(pgrep -af "$PATTERN" | awk '{print $1}') || true
  if [ ${#REMAIN[@]} -ne 0 ]; then
    echo "Force killing remaining: ${REMAIN[*]}"
    for pid in "${REMAIN[@]}"; do
      if [ "$pid" -eq $$ ] || [ "$pid" -le 2 ]; then
        continue
      fi
      kill -9 "$pid" 2>/dev/null || true
    done
  fi
fi

# Remove pidfile if present
if [ -f "$PIDFILE" ]; then
  echo "Removing PID file: $PIDFILE"
  rm -f "$PIDFILE"
fi

echo "Stop complete. Current launcher processes:"
pgrep -af "$PATTERN" || echo "none"

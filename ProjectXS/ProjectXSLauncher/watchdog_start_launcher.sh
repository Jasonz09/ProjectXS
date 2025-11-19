#!/usr/bin/env bash
# Simple watchdog to keep the ProjectXS launcher running
WORKDIR="/Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXSLauncher"
LOGFILE="$WORKDIR/launcher_watchdog.log"
PIDFILE="$WORKDIR/launcher_watchdog.pid"
cd "$WORKDIR" || exit 1

# Prevent multiple watchdog instances
if [ -f "$PIDFILE" ]; then
  OLD_PID=$(cat "$PIDFILE" 2>/dev/null)
  if [ -n "$OLD_PID" ] && kill -0 "$OLD_PID" 2>/dev/null; then
    echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Watchdog already running with PID $OLD_PID - exiting." >> "$LOGFILE"
    exit 0
  else
    echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Stale PID file found - cleaning up." >> "$LOGFILE"
    rm -f "$PIDFILE"
  fi
fi

# Write our PID
echo $$ > "$PIDFILE"

# Ensure PID file is removed on exit
cleanup() {
  rm -f "$PIDFILE"
}
trap cleanup EXIT

while true; do
  # If an Electron launcher process is already running, don't try to start another
  EXISTING_LAUNCHER_PID=$(pgrep -f "$WORKDIR/node_modules/electron/dist/Electron.app" | head -n 1 || true)
  if [ -n "$EXISTING_LAUNCHER_PID" ]; then
    echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Detected existing launcher PID $EXISTING_LAUNCHER_PID - not starting another instance. Exiting watchdog." >> "$LOGFILE"
    break
  fi

  echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Starting launcher..." >> "$LOGFILE"
  npm start >> "$LOGFILE" 2>&1
  EXIT_CODE=$?
  if [ "$EXIT_CODE" -eq 0 ]; then
    echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Launcher exited cleanly with code 0 - stopping watchdog." >> "$LOGFILE"
    break
  else
    echo "[$(date -u '+%Y-%m-%d %H:%M:%S')] Launcher exited with code $EXIT_CODE - restarting in 2s..." >> "$LOGFILE"
    sleep 2
  fi
done

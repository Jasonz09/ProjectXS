#!/usr/bin/env bash
# Simple watchdog to keep the ProjectXS launcher running
WORKDIR="/Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXSLauncher"
LOGFILE="$WORKDIR/launcher_watchdog.log"
cd "$WORKDIR" || exit 1

while true; do
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

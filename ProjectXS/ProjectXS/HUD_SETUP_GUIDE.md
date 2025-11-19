# HUD Widget Setup Guide

## Overview
This guide will help you create the UI widgets for the Dreamland Assault game mode.

---

## 1. Main Game HUD (WBP_GameHUD)

### Create the Widget:
1. Content Browser → Right-click → User Interface → Widget Blueprint
2. Name it: `WBP_GameHUD`
3. Open the widget

### Layout Structure:
```
Canvas Panel (Root)
├── Top Bar (Horizontal Box)
│   ├── Team Score Left (Guardians)
│   ├── Round Timer (Center)
│   └── Team Score Right (Devils)
├── Center Crosshair (Image)
├── Interaction Prompt (Text Block) - Bottom Center
├── Plant/Defuse Progress Bar (Progress Bar)
└── Bottom Info (Horizontal Box)
    ├── Health Display
    ├── Armor Display
    └── Ammo Display
```

### Top Bar - Team Scores:

**Guardians Score (Left):**
- Text Block
- Anchor: Top-Left
- Position: X=50, Y=20
- Font Size: 36
- Color: Blue (0.2, 0.5, 1.0)
- Binding: Get Guardians Score from Game Mode

**Round Timer (Center):**
- Text Block
- Anchor: Top-Center
- Position: X=0, Y=20
- Font Size: 48
- Color: White
- Format: "1:40" (minutes:seconds)
- Binding: Get remaining time from Game Mode

**Devils Score (Right):**
- Text Block
- Anchor: Top-Right
- Position: X=-50, Y=20
- Font Size: 36
- Color: Red (1.0, 0.2, 0.2)
- Binding: Get Devils Score from Game Mode

### Crosshair:
- Image
- Anchor: Center
- Size: 32x32
- Alignment: 0.5, 0.5
- Color: White with slight transparency

### Interaction Prompt:
- Text Block
- Anchor: Bottom-Center
- Position: Y=-150
- Font Size: 24
- Color: Yellow
- Binding: Get interaction prompt from Interaction Component
- Visibility: Visible only when near interactable

### Plant/Defuse Progress Bar:
- Progress Bar
- Anchor: Bottom-Center
- Position: Y=-200
- Size: 400x20
- Fill Color: 
  - Blue for defusing
  - Red for planting
- Binding: Get progress from Interaction Component
- Visibility: Visible only when interacting

---

## 2. Round Phase Indicator (WBP_PhaseIndicator)

### Create the Widget:
1. Widget Blueprint → `WBP_PhaseIndicator`

### Layout:
```
Canvas Panel
└── Phase Text (Text Block)
    - Anchor: Top-Center
    - Position: Y=100
    - Font Size: 64
    - Shadow: Large drop shadow
```

### Phase Display Text:
- "BUY PHASE" - Green
- "ROUND STARTING" - Yellow
- "ARTIL PLANTED" - Red, pulsing
- "DEFUSING" - Blue
- "ROUND WON" - Team color

### Animation:
1. Create animation: "FadeInOut"
2. Opacity: 0 → 1 → 0
3. Duration: 3 seconds
4. Play on phase change

---

## 3. Score Board (WBP_ScoreBoard)

### Create the Widget:
1. Widget Blueprint → `WBP_ScoreBoard`

### Layout:
```
Overlay
└── Background (Image, semi-transparent black)
    └── Vertical Box
        ├── Title "SCOREBOARD"
        ├── Guardians Team List
        │   └── For each player:
        │       - Player Name
        │       - Kills / Deaths
        │       - Ping
        ├── Divider
        └── Devils Team List
            └── For each player:
                - Player Name
                - Kills / Deaths
                - Ping
```

### Toggle:
- Bind to Tab key
- Show: On key press
- Hide: On key release

---

## 4. Artil Planted Notification (WBP_ArtilNotification)

### Create the Widget:
1. Widget Blueprint → `WBP_ArtilNotification`

### Layout:
```
Canvas Panel
└── Notification Container (Border)
    └── Horizontal Box
        ├── Icon (Image) - Artil icon
        └── Vertical Box
            ├── Title "ARTIL PLANTED"
            └── Timer "0:50"
```

### Style:
- Background: Dark red with pulsing glow
- Title: Large, bold, red text
- Timer: Countdown in white
- Position: Top-center, below round timer

### Behavior:
- Show when Artil planted
- Update countdown timer
- Pulse animation
- Hide when defused or detonated

---

## 5. Blueprint Event Graph Setup

### In WBP_GameHUD:

#### Event Construct:
```cpp
Event Construct
→ Get Game Mode
→ Cast to XSRoundBasedGameMode
→ Bind Event (OnRoundPhaseChanged)
→ Bind Event (OnTeamScored)
```

#### Update Round Timer (Event Tick):
```cpp
Event Tick
→ Get Game Mode
→ Get Current Phase
→ Get Phase Time Remaining
→ Format as MM:SS
→ Set Timer Text
```

#### Update Scores:
```cpp
On Team Scored Event
→ Get Guardians Score
→ Set Guardians Score Text
→ Get Devils Score
→ Set Devils Score Text
→ Play Score Animation
```

#### Update Interaction Prompt:
```cpp
Event Tick
→ Get Player Pawn
→ Get Interaction Component
→ Get Interaction Prompt Text
→ Set Prompt Text
→ Set Visibility (Visible if text not empty)
```

#### Update Progress Bar:
```cpp
Event Tick
→ Get Player Pawn
→ Get Interaction Component
→ Is Interacting?
→ Branch:
   TRUE → Get Interaction Progress / Duration
          Set Progress Bar Percent
          Set Visible
   FALSE → Set Hidden
```

---

## 6. Adding HUD to Player

### In Your Player Controller Blueprint:

```cpp
Event BeginPlay
→ Create Widget (WBP_GameHUD)
→ Add to Viewport
→ Set Input Mode UI Only for HUD elements
```

---

## 7. Material/Style Settings

### Colors:
- **Guardians (Blue):** RGB(51, 128, 255) or Hex #3380FF
- **Devils (Red):** RGB(255, 51, 51) or Hex #FF3333
- **Neutral:** White or Light Gray
- **Warning:** Yellow/Orange for timers
- **Critical:** Pulsing Red for low time

### Fonts:
- Title/Large Text: Bold, 48-72pt
- Body Text: Medium, 24-36pt
- Small Text: Regular, 18-24pt
- Monospace for timers (00:00 format)

### Effects:
- Drop shadows on all text for readability
- Slight glow on important elements
- Pulse animation for urgent notifications
- Fade transitions between states

---

## 8. Testing Checklist

Once you create the widgets:

- [ ] Team scores display correctly
- [ ] Round timer counts down
- [ ] Crosshair centered properly
- [ ] Interaction prompt appears near Artils
- [ ] Progress bar shows during plant/defuse
- [ ] Phase changes show notifications
- [ ] Scoreboard toggles with Tab key
- [ ] Artil planted warning appears
- [ ] All text readable against background
- [ ] Animations play smoothly

---

## Quick Start Steps:

1. **Create WBP_GameHUD** first (most important)
2. **Add to Player Controller** so it shows in game
3. **Test with basic elements** (just scores and timer)
4. **Add interaction elements** after testing
5. **Polish with animations** last

---

## Example Blueprint Bindings:

### Get Guardians Score (Text Binding):
```cpp
Get Game Mode
→ Cast to XSRoundBasedGameMode
→ Get Guardians Score
→ To String
→ Append Text "GUARDIANS: "
→ Return
```

### Get Round Timer (Text Binding):
```cpp
Get Game Mode
→ Cast to XSRoundBasedGameMode
→ Get Phase Time Remaining
→ To Int (truncate)
→ Minutes = Value / 60
→ Seconds = Value % 60
→ Format String "{0}:{1:02d}" (e.g., "1:23")
→ Return
```

### Get Interaction Prompt (Text Binding):
```cpp
Get Player Character
→ Get Component by Class (XSInteractionComponent)
→ Get Interaction Prompt Text
→ Return
```

---

Need help with any specific widget? Let me know!

# Dreamland Assault - Implementation Guide

## ✅ What I Just Created

I've added the core C++ classes for your Dreamland Assault game mode:

### 1. **XSRoundBasedGameMode** 
Game mode for both Unrated and Competitive matches
- Manages rounds, scoring, and match flow
- Handles team assignment (Guardians vs Devils)
- Tracks win conditions (elimination or objective completion)

### 2. **ArtilActor**
The spiky sphere bomb that Devils plant
- Can be picked up by Devils
- Drops on death
- 4-second plant time
- 7-second defuse time
- 50-second detonation countdown
- Radial explosion damage

### 3. **DreamlandZone**
Map locations where Artils can be planted
- Defines valid plant areas with blue/pink outline
- Multiple plant spots per zone (5 default spots)
- Hidden plant spots option
- Visual debug outlines in editor

---

## 🛠️ How to Use These Classes

### Step 1: Compile the Code

1. **Close Unreal Editor** (if it's open)
2. **Open Terminal** and navigate to your project:
   ```bash
   cd "/Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXS"
   ```

3. **Build the project:**
   ```bash
   # On Mac, use Xcode build
   xcodebuild -project "ProjectXS (Mac).xcworkspace" -scheme "ProjectXS" -configuration Development
   ```

   **OR** just open the .uproject file and Unreal will prompt you to rebuild.

4. **Open ProjectXS.uproject** - It will compile automatically

---

### Step 2: Set Up Your Game Mode in Unreal

1. **Open Project Settings**
   - Edit → Project Settings
   - Maps & Modes

2. **Create Blueprint Game Mode**
   - Content Browser → Right-click in Content folder
   - Blueprint Class → XSRoundBasedGameMode
   - Name it: `BP_DreamlandGameMode`

3. **Set as Default Game Mode**
   - In Project Settings → Maps & Modes
   - Set Default GameMode to `BP_DreamlandGameMode`

---

### Step 3: Create Dreamland Zones in Your Map

1. **Open your map** (or create new: File → New Level)

2. **Place Dreamland Zone:**
   - Place Actors panel → Search "DreamlandZone"
   - Drag into viewport
   - Name it "Dreamland_A" in World Outliner

3. **Configure the Zone:**
   - Select the zone
   - Details panel:
     - Zone Name: "Dreamland A"
     - Zone Outline Color: Adjust blue/pink color
     - Plant Spots: Add/edit plant locations
     - Show Debug Outline: ✅ Check this

4. **Position the Zone:**
   - Move it to desired location on map
   - Scale the box volume to fit your area
   - You'll see blue outline and cyan spheres for plant spots

5. **Create Second Zone (Optional):**
   - Duplicate the zone (Alt+Drag)
   - Rename to "Dreamland_B"
   - Move to different location

---

### Step 4: Create Artil Actor Blueprint

1. **Create Blueprint:**
   - Content Browser → Right-click
   - Blueprint Class → ArtilActor
   - Name: `BP_Artil`

2. **Set Up Mesh:**
   - Open BP_Artil
   - Select "ArtilMesh" component
   - Static Mesh: Add a sphere or spiky mesh
   - Material: Make it glow red/orange

3. **Configure Settings:**
   - Details panel:
     - Plant Duration: 4 seconds
     - Defuse Duration: 7 seconds
     - Detonation Time: 50 seconds
     - Explosion Radius: 1500 (15 meters)
     - Explosion Damage: 200

---

### Step 5: Spawn Artils at Round Start

In your `BP_DreamlandGameMode` Blueprint:

1. **Add Event Graph logic:**
   ```
   Event BeginPlay
   → Delay (3 seconds)
   → Start New Round
   → Select 2 Random Devils
   → Spawn 2 Artils
   → Give Artils to selected Devils
   ```

2. **Or use C++ (recommended):**
   - I can add automatic Artil spawning to the game mode if you want

---

### Step 6: Test Basic Functionality

1. **Play in Editor (PIE):**
   - Click Play ▶️
   - Check Output Log for messages:
     - "Round 1 started"
     - "Dreamland Zone initialized"
     - "Artil spawned"

2. **Test with Multiple Players:**
   - Play dropdown → Advanced Settings
   - Number of Players: 2
   - Net Mode: Play As Listen Server
   - This simulates multiplayer

---

## 🎮 How Players Will Interact (To Be Implemented)

### For Devils (Attackers):
1. **Pick Up Artil:** Press E near dropped Artil
2. **Plant Artil:** Hold E for 4 seconds in Dreamland zone
3. **Protect Plant:** Defend planted Artil for 50 seconds

### For Guardians (Defenders):
1. **Find Artil:** Look for glowing Artil in Dreamland zones
2. **Defuse Artil:** Hold E for 7 seconds on planted Artil
3. **Interrupt Planter:** Damage Devil planting to stop them

---

## 📝 Next Steps

### What YOU Need to Do:

1. **Compile the code** (open .uproject, let it build)
2. **Create BP_DreamlandGameMode** blueprint
3. **Place 1-2 Dreamland Zones** in your test map
4. **Create BP_Artil** blueprint with a mesh
5. **Test in Play mode**

### What I Can Add Next:

Tell me which you want:
- [ ] Player interaction system (E to pickup/plant/defuse)
- [ ] HUD showing timers, plant progress, scores
- [ ] Team assignment logic (auto-assign players to teams)
- [ ] Economy/buy phase system
- [ ] Spawn point system for each team
- [ ] Character classes integration with existing code

---

## 🐛 Troubleshooting

**"Code won't compile"**
- Make sure all #include statements are correct
- Verify Unreal Engine 5.7 is installed
- Check Output Log for specific errors

**"Can't find DreamlandZone in editor"**
- File → Refresh Visual Studio Project
- Restart Unreal Editor
- Check Filters in Place Actors panel

**"Artil doesn't show up"**
- Make sure you created the Blueprint version
- Check if it's spawned (use ~ console: `showdebug`)
- Look in World Outliner for spawned actors

---

## 📚 Code Architecture Overview

```
XSRoundBasedGameMode (Game Rules)
├── Manages Rounds
├── Tracks Scores
├── Assigns Teams
└── Checks Win Conditions
    ├── All enemies eliminated?
    ├── Both Artils detonated?
    └── Artils defused?

ArtilActor (Bomb)
├── States: Dropped → Carried → Planting → Planted → Defusing → Defused/Detonated
├── Pickup (Devils only)
├── Drop on death
├── Plant in Dreamland zones
└── Defuse or explode

DreamlandZone (Bomb Site)
├── Defines valid plant area
├── Shows visual outline
├── Multiple plant spots
└── Validates plant locations
```

---

## 🎯 Testing Checklist

Once you have it compiled and set up:

- [ ] Game mode loads without errors
- [ ] Dreamland zones visible with debug outlines
- [ ] Artils can be spawned
- [ ] Round timer counts down
- [ ] Scores increment properly
- [ ] Console shows round messages

Let me know when you're ready for the next step!

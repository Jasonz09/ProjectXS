# 🚀 Quick Start Guide - Get Playing in 30 Minutes!

## ✅ What You Have Now

All the code for Dreamland Assault is ready! Here's what's been created:

### Core Systems:
- ✅ **XSRoundBasedGameMode** - Game rules, rounds, scoring
- ✅ **ArtilActor** - Spiky bomb with plant/defuse mechanics
- ✅ **DreamlandZone** - Bomb sites with visual markers
- ✅ **XSInteractionComponent** - E key for pickup/plant/defuse
- ✅ **XSTeamPlayerStart** - Team spawn points

### Documentation:
- ✅ **DREAMLAND_ASSAULT_GDD.md** - Full game design
- ✅ **IMPLEMENTATION_GUIDE.md** - Detailed setup steps
- ✅ **HUD_SETUP_GUIDE.md** - UI widget instructions

---

## 🎯 30-Minute Setup Checklist

Follow these steps in order to get your first playable test:

### ⏱️ **Step 1: Compile (5 minutes)**

1. **Close Unreal Editor** if it's open
2. **Double-click** `ProjectXS.uproject`
3. When prompted: **"Would you like to rebuild?"** → Click **YES**
4. Wait for compilation (first time takes longer)
5. Editor opens → You're ready!

**Troubleshooting:**
- If compile fails, check Output Log for errors
- Make sure you have Xcode installed (Mac)
- Try: File → Refresh Visual Studio Project

---

### ⏱️ **Step 2: Create Test Map (5 minutes)**

1. **File → New Level → Empty Level**
2. **Save As:** `Content/Maps/Dreamland_TestMap`

3. **Add Lighting:**
   - Place Actors panel → Search "Directional Light"
   - Drag into scene

4. **Add Floor:**
   - Search "Plane" → Drag into scene
   - Details panel → Scale: `(100, 100, 1)`

5. **Add Some Walls:**
   - Search "Cube" → Drag 5-10 into scene
   - Scale to make cover: `(1, 5, 2)`
   - Position around the map

6. **Save:** Ctrl+S (Cmd+S on Mac)

---

### ⏱️ **Step 3: Add Dreamland Zones (3 minutes)**

1. **Place Actors → Search "DreamlandZone"**
   - If you don't see it: Window → Place Actors → Search again
   
2. **Drag into viewport** (create 2 zones)

3. **Zone A Setup:**
   - Name in World Outliner: `Dreamland_A`
   - Position: One side of map
   - Details → Zone Name: "Dreamland A"
   - Show Debug Outline: ✅ Checked

4. **Zone B Setup:**
   - Name: `Dreamland_B`
   - Position: Other side of map
   - Zone Name: "Dreamland B"
   - Show Debug Outline: ✅ Checked

5. You should see **blue/pink outlines** and **cyan spheres** (plant spots)

---

### ⏱️ **Step 4: Add Team Spawn Points (5 minutes)**

1. **Place Actors → Search "XSTeamPlayerStart"**

2. **Add 6 Guardian Spawns:**
   - Drag 6 spawn points near one side
   - Select each one → Details:
     - Team: **Guardians**
   - Should turn **BLUE**

3. **Add 6 Devil Spawns:**
   - Drag 6 spawn points on opposite side
   - Select each → Details:
     - Team: **Devils**
   - Should turn **RED**

4. **Spacing:**
   - Space spawn points ~2 meters apart
   - Don't overlap them

---

### ⏱️ **Step 5: Create Game Mode Blueprint (5 minutes)**

1. **Content Browser → Right-click**
2. **Blueprint Class → XSRoundBasedGameMode**
3. **Name:** `BP_DreamlandGameMode`

4. **Open BP_DreamlandGameMode:**
   - Class Defaults → Details:
     - Game Mode Type: **Unrated** (or Competitive)
     - Rounds To Win: **6** (or 7 for Competitive)
     - Players Per Team: **6**

5. **Compile & Save**

6. **Set as Default Game Mode:**
   - Edit → Project Settings
   - Maps & Modes
   - Default GameMode: **BP_DreamlandGameMode**
   - Save

---

### ⏱️ **Step 6: Create Artil Blueprint (3 minutes)**

1. **Content Browser → Right-click**
2. **Blueprint Class → ArtilActor**
3. **Name:** `BP_Artil`

4. **Open BP_Artil:**
   - Select **ArtilMesh** component
   - Static Mesh: Choose **Sphere** (temporary)
   - Material: Any bright color (red/orange)

5. **Class Defaults:**
   - Plant Duration: **4.0**
   - Defuse Duration: **7.0**
   - Detonation Time: **50.0**
   - Explosion Radius: **1500.0**

6. **Compile & Save**

---

### ⏱️ **Step 7: Test Basic Functionality (4 minutes)**

1. **World Settings:**
   - Window → World Settings
   - Game Mode Override: **BP_DreamlandGameMode**

2. **Play Settings:**
   - Play button dropdown → Advanced Settings
   - Number of Players: **2**
   - Net Mode: **Play As Listen Server**

3. **Click Play ▶️**

4. **Check Output Log:**
   - Window → Developer Tools → Output Log
   - Look for:
     - ✅ "Round 1 started"
     - ✅ "Dreamland Zone initialized"
     - ✅ "Player assigned to team"

5. **Visual Check:**
   - See blue Dreamland zone outlines?
   - See spawn points in scene?
   - Can you move around?

---

## 🎮 Testing Your Game

### Quick Test (5 minutes):

1. **Spawn 2 Artils** (for now, manually in Event Graph):
   ```
   Event BeginPlay
   → Delay 3 seconds
   → Spawn Actor (BP_Artil)
   → Location: Near Dreamland zone
   → Spawn another BP_Artil
   ```

2. **Add Interaction to Character:**
   - Open your Character Blueprint
   - Add Component → **XSInteractionComponent**
   - Compile & Save

3. **Bind E Key:**
   - Edit → Project Settings → Input
   - Action Mappings → Add: "Use"
   - Key: **E**

4. **In Character Blueprint:**
   ```
   Input Action Use (Pressed)
   → Get Component (XSInteractionComponent)
   → Start Interaction
   
   Input Action Use (Released)
   → Get Component (XSInteractionComponent)
   → Stop Interaction
   ```

5. **Test in Play Mode:**
   - Walk near Artil
   - Press E to pick up (if Devil)
   - Walk to Dreamland zone
   - Hold E to plant
   - Watch for progress

---

## ✅ Success Checklist

You've set it up correctly if:

- [ ] Game compiles without errors
- [ ] Two Dreamland zones visible with blue/pink outlines
- [ ] Plant spots show as cyan spheres
- [ ] Spawn points visible (blue for Guardians, red for Devils)
- [ ] Players spawn at team-specific locations
- [ ] Output log shows "Round 1 started"
- [ ] Can walk around the map
- [ ] (If Artils spawned) Can see them in scene

---

## 🐛 Common Issues

### "Can't find DreamlandZone in Place Actors"
**Fix:** File → Refresh Visual Studio Project → Restart editor

### "Compile errors"
**Fix:** Check that all .h and .cpp files are in Source/ProjectXS/
Copy exact error from Output Log

### "Players don't spawn at team locations"
**Fix:** Make sure World Settings → Game Mode is set to BP_DreamlandGameMode

### "Can't interact with Artils"
**Fix:** Make sure:
- XSInteractionComponent added to Character
- E key bound in Project Settings → Input
- Character Blueprint has Input Action event

### "Zones don't show outlines"
**Fix:** Select zone → Details → Show Debug Outline: ✅ Check

---

## 🎯 Next Steps (After Basic Test Works)

1. **Create HUD Widgets** (follow HUD_SETUP_GUIDE.md)
2. **Add Artil spawn logic** to Game Mode
3. **Add economy/buy phase** UI
4. **Create weapon systems** integration
5. **Polish VFX and SFX**

---

## 📚 Key Files Reference

**C++ Code:**
- `/Source/ProjectXS/XSRoundBasedGameMode.h/cpp`
- `/Source/ProjectXS/ArtilActor.h/cpp`
- `/Source/ProjectXS/DreamlandZone.h/cpp`
- `/Source/ProjectXS/XSInteractionComponent.h/cpp`
- `/Source/ProjectXS/XSTeamPlayerStart.h/cpp`

**Blueprints to Create:**
- `BP_DreamlandGameMode` (created ✓)
- `BP_Artil` (created ✓)
- `WBP_GameHUD` (follow HUD guide)

**Documentation:**
- `DREAMLAND_ASSAULT_GDD.md` - Game design
- `IMPLEMENTATION_GUIDE.md` - Detailed setup
- `HUD_SETUP_GUIDE.md` - UI widgets

---

## 💡 Pro Tips

1. **Use PIE (Play In Editor) with 2 players** to test team mechanics
2. **Check Output Log constantly** for debug messages
3. **Save often** - Ctrl+S / Cmd+S
4. **Test one feature at a time** before adding more
5. **Use Show Debug Outline** on zones during development

---

## 🆘 Need Help?

1. **Check Output Log** for error messages
2. **Review IMPLEMENTATION_GUIDE.md** for detailed steps
3. **Look at HUD_SETUP_GUIDE.md** for UI help
4. **Read code comments** in C++ files for usage notes

---

## 🎉 You're Ready!

If you've completed all 7 steps above, you have:
- ✅ Compiled code working
- ✅ Test map with zones and spawns
- ✅ Game mode configured
- ✅ Basic systems ready to test

**Now just add the Interaction Component to your character and you can start testing pickup/plant/defuse!**

Good luck! 🚀

# Dreamland Assault - Quick Blueprint Setup Card

**Time to first playable: ~3 hours**  
**Prerequisites: C++ code already compiled** ✅

---

## 🎯 Step 1: Create Game Mode Blueprint (5 min)

1. Content Browser → Right-click → Blueprint Class
2. Search for `XSRoundBasedGameMode` → Select
3. Name: `BP_DreamlandGameMode`
4. Open → Class Defaults → Set:
   - Rounds To Win: `6`
   - Players Per Team: `6`  
   - Buy Phase Duration: `30`
   - Combat Phase Duration: `100`
   - Post Plant Duration: `50`
5. Save

---

## 💣 Step 2: Create Artil Blueprint (10 min)

1. Content Browser → Right-click → Blueprint Class
2. Search for `ArtilActor` → Select
3. Name: `BP_Artil`
4. Open → Viewport:
   - Select `ArtilMesh` component
   - Details → Static Mesh → Select `Sphere` (or any mesh)
   - Scale: `0.3, 0.3, 0.3`
5. Create Material `M_Artil_Glow`:
   - Right-click → Material
   - Set Shading Model: `Unlit`
   - Constant3Vector (pink `1, 0, 1`) → Emissive Color
   - Apply to ArtilMesh
6. Class Defaults:
   - Plant Duration: `4.0`
   - Defuse Duration: `7.0`
   - Detonation Time: `50.0`
   - Explosion Radius: `1500`
7. Save

---

## 🎨 Step 3: Create Dreamland Zone Blueprint (10 min)

1. Content Browser → Right-click → Blueprint Class
2. Search for `DreamlandZone` → Select
3. Name: `BP_DreamlandZone`
4. Open → Viewport:
   - Select `ZoneVolume` component
   - Scale: `10, 10, 3` (10m x 10m x 3m)
5. Details → Plant Spots:
   - Click `+` twice to add 2 elements
   - Element 0: Location `(0, -200, 0)`, Radius `100`
   - Element 1: Location `(0, 200, 0)`, Radius `100`
6. Check `Show Debug` (to visualize in editor)
7. Save

---

## 🗺️ Step 4: Create Test Map (30 min)

### 4A. New Level
1. File → New Level → Empty Level
2. Save as `Dreamland_Prototype`

### 4B. Add Lighting
1. Place Actors → Lights → Directional Light
2. Place Actors → Visual Effects → Sky Atmosphere
3. Place Actors → Visual Effects → Post Process Volume
   - Details → check `Infinite Extent (Unbound)`

### 4C. Add Geometry
1. Place Actors → Geometry → Box (for floor)
   - Scale: `50, 50, 1`
2. Place a few Box BSPs for walls/cover (optional)

### 4D. Place Zones
1. Drag `BP_DreamlandZone` from Content Browser → Viewport
2. Place 2 zones ~30m apart (one for Site A, one for Site B)
3. Rotate/position as desired

### 4E. Place Player Starts
**Option A: If you have BP_TeamPlayerStart**
1. Drag `BP_TeamPlayerStart` into level
2. Place 6 near Guardians spawn
   - Select each → Details → Team = `Guardians`
3. Place 6 near Devils spawn
   - Details → Team = `Devils`

**Option B: Use base PlayerStart + manual team assignment**
1. Place Actors → Player Start
2. Place 12 total, spread ~2m apart
3. (Team assignment will happen in GameMode)

### 4F. Place Artils
1. Drag `BP_Artil` into level
2. Place 2 Artils at Devils spawn area

### 4G. Set Game Mode
1. Window → World Settings
2. Game Mode Override → `BP_DreamlandGameMode`
3. Save All

---

## 👤 Step 5: Setup Player Interaction (10 min)

1. Open your Character Blueprint (e.g., `BP_ShooterCharacter`)
2. Components panel → Add Component → search `XSInteractionComponent`
3. Select the component → Details → check settings
4. Event Graph:
   - Add InputAction event for "Use" key (E)
   - On Pressed → call `XSInteractionComponent → Start Interaction`
   - On Released → call `XSInteractionComponent → Stop Interaction`
5. Compile & Save

**If "Use" input doesn't exist:**
1. Edit → Project Settings → Input
2. Action Mappings → Add `Use`
3. Bind to `E` key

---

## 🎮 Step 6: Create Basic HUD (20 min)

### 6A. Main HUD Widget
1. Content Browser → Right-click → User Interface → Widget Blueprint
2. Name: `WBP_GameHUD`
3. Open → Designer:
   - Drag `Canvas Panel` (root)
   - Add `Text` → rename "ScoreText"
     - Anchors: Top Center
     - Text: "Guardians 0 | 0 Devils"
   - Add `Text` → rename "RoundText"
     - Position below scores
     - Text: "Round 1"
   - Add `Text` → rename "TimerText"
     - Position top-right
     - Text: "1:40"
4. Graph:
   - Event Construct:
     - Get Game Mode → Cast To XSRoundBasedGameMode → Promote to variable `GameModeRef`
   - Event Tick:
     - Get `GameModeRef`
     - Get Guardians Score, Devils Score
     - Format String: "Guardians {0} | {1} Devils" → Set Text (ScoreText)
     - Get Current Round → Format → Set Text (RoundText)
5. Save

### 6B. Hook HUD to GameMode
1. Open `BP_DreamlandGameMode`
2. Event BeginPlay:
   - Create Widget → Class: `WBP_GameHUD`
   - Add to Viewport
3. Save

---

## ✅ Step 7: First Test (5 min)

1. Open `Dreamland_Prototype` map
2. Click **Play** (PIE)
3. **Expected**:
   - You spawn in the map
   - HUD shows at top
   - Walk to Artil, press `E` to pick up
   - Walk to blue debug zone
   - Hold `E` to plant (4 seconds)
   - See countdown start
4. **If it works**: Core systems are functional! ✅
5. **If not**: Check Output Log for errors

---

## 🌐 Step 8: Network Test (5 min)

1. Editor Preferences → Play → Advanced Settings:
   - Number of Players: `2`
   - Net Mode: `Play As Listen Server`
2. Click **Play**
3. **Window 1** (server): Pick up Artil, plant it
4. **Window 2** (client): See Artil state, try to defuse
5. **Expected**: Both windows see synced Artil state ✅

---

## 🐛 Troubleshooting

| Issue | Fix |
|-------|-----|
| Can't pick up Artil | Add `XSInteractionComponent` to Character BP + bind E key |
| Artil won't plant | Check you're inside DreamlandZone (blue debug box) |
| HUD not showing | Hook Create Widget in GameMode BeginPlay |
| Compile errors | Open Visual Studio, rebuild solution |
| Network issues | Use "Play As Listen Server", not Standalone |

---

## 📋 Final Checklist

- [ ] `BP_DreamlandGameMode` created
- [ ] `BP_Artil` created with glowing mesh
- [ ] `BP_DreamlandZone` created with 2 plant spots
- [ ] `Dreamland_Prototype` map created
- [ ] 2 DreamlandZones placed in map
- [ ] 12 PlayerStarts placed (or 6+6 team starts)
- [ ] 2 Artils placed
- [ ] World Settings → GameMode set
- [ ] Character has `XSInteractionComponent`
- [ ] E key bound to Start/Stop Interaction
- [ ] `WBP_GameHUD` created and hooked to GameMode
- [ ] Solo test successful
- [ ] Network test (2 players) successful

---

## 🎉 You're Ready!

Once all checkboxes are ticked, you have a **playable prototype** ready for testing and iteration.

**Next steps:**
- Test full round flow (plant → defuse → score)
- Add more detailed HUD elements
- Replace placeholder meshes with custom art
- Add VFX/SFX
- Build more detailed map geometry
- Tune gameplay values (timers, damage, radius)

**Total time invested: ~3 hours**  
**Result: Fully functional game prototype** 🎮

---

**Need help?** Check `DREAMLAND_TESTING_GUIDE.md` for detailed instructions and debugging tips.

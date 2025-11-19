# Dreamland Assault - Complete Testing & Setup Guide

**Status**: Core C++ classes implemented, ready for Blueprint/Asset creation and testing

## What You Have (Already Implemented)

✅ **C++ Core Systems**
- `AXSRoundBasedGameMode` - Round flow, team scoring, phase management
- `AArtilActor` - Bomb with plant/defuse/explosion logic
- `ADreamlandZone` - Plant zone validation and plant spots
- `UXSInteractionComponent` - Player interaction (E key)
- `AXSTeamPlayerStart` - Team-based spawn points

✅ **Existing Game Foundation**
- Shooter variant with weapons, character movement, health system
- Ability system (GAS) for weapons and abilities
- Basic UI framework
- Network replication infrastructure

## What You Need to Create (Blueprint/Assets)

### 1. Blueprints (30 minutes)

#### A. Game Mode Blueprint
**File**: `Content/Blueprints/BP_DreamlandGameMode.uasset`
1. Create new Blueprint: Right-click Content Browser → Blueprint Class → `XSRoundBasedGameMode`
2. Name it `BP_DreamlandGameMode`
3. Set defaults in Class Defaults panel:
   - Game Mode Type: Unrated
   - Rounds To Win: 6
   - Players Per Team: 6
   - Buy Phase Duration: 30s
   - Combat Phase Duration: 100s
   - Post Plant Duration: 50s
4. Save

#### B. Artil Blueprint
**File**: `Content/Blueprints/Items/BP_Artil.uasset`
1. Create Blueprint: Blueprint Class → `ArtilActor`
2. Name it `BP_Artil`
3. In the viewport:
   - Select `ArtilMesh` component
   - Add a temporary mesh (sphere or cube scaled to ~30cm)
   - Set material to glowing red/pink
4. Set Class Defaults:
   - Plant Duration: 4.0
   - Defuse Duration: 7.0
   - Detonation Time: 50.0
   - Explosion Radius: 1500 (15m)
   - Explosion Damage: 200
5. Save

#### C. Dreamland Zone Blueprint
**File**: `Content/Blueprints/Zones/BP_DreamlandZone.uasset`
1. Create Blueprint: Blueprint Class → `DreamlandZone`
2. Name it `BP_DreamlandZone`
3. In viewport:
   - Select `ZoneVolume` box component
   - Scale to desired plant area size (e.g., 10m x 10m x 3m)
4. In Details panel → Plant Spots array:
   - Add 2 elements
   - Set Location for each spot (e.g., {0, -200, 0} and {0, 200, 0})
   - Set Radius: 100 for each
5. Enable "Show Debug" to see zone visualization in editor
6. Save

### 2. Test Map (45 minutes)

**File**: `Content/Maps/Dreamland_Prototype.umap`

#### Step-by-Step Map Creation:
1. **Create Map**: File → New Level → Empty Level → Save as `Dreamland_Prototype`

2. **Add Basic Geometry** (use BSP or simple meshes):
   - Floor: 50m x 50m plane
   - Some walls/cover (boxes 2m tall)
   - Two distinct areas for Dreamland zones (e.g., opposite sides)

3. **Lighting**:
   - Add Directional Light (sun)
   - Add Sky Atmosphere
   - Add Post Process Volume (set to Unbound)

4. **Place Dreamland Zones**:
   - Drag `BP_DreamlandZone` into map
   - Place 2 zones (one at each bomb site)
   - Position them ~30m apart
   - Rotate/scale to fit your layout

5. **Place Player Starts**:
   - Drag `AXSTeamPlayerStart` into level (or create BP from it first)
   - Place 6 near Guardians spawn
     - Set Team = Guardians in Details
   - Place 6 near Devils spawn
     - Set Team = Devils
   - Spread them out ~2m apart

6. **Place Artils**:
   - Drag 2x `BP_Artil` into level
   - Place both at Devils spawn initially (they'll pick them up)

7. **Set World Settings**:
   - Window → World Settings
   - Game Mode Override → `BP_DreamlandGameMode`
   - Save

### 3. HUD Widgets (UMG - 60 minutes)

#### A. Main HUD (`WBP_GameHUD`)
Create widget blueprint with:
- **Top bar**: Team scores (Guardians 0 | 0 Devils)
- **Round indicator**: "Round 1/13"
- **Phase timer**: "1:40" countdown
- **Artil carrier icon**: "Devil 1 has Artil A" (only show when carried)
- **Plant timer**: "0:50" (only show when planted)
- **Crosshair**: simple center dot

**Quick setup**:
1. Create Widget Blueprint → `WBP_GameHUD`
2. Add Canvas Panel
3. Add Text blocks bound to GameMode variables:
   - Bind Text → Get Game Mode → Cast to XSRoundBasedGameMode → Get Guardians Score
4. Add Event Tick to update timer displays
5. Set as HUD in GameMode BeginPlay

#### B. Scoreboard (`WBP_Scoreboard`)
- Toggle with Tab key
- Show all 12 players, their team, kills, deaths, score
- Bind to PlayerState arrays from GameMode

#### C. Artil Notification (`WBP_ArtilNotification`)
- Temporary pop-up: "Artil A Planted at Site B"
- Fade in/out animation
- Triggered by GameMode events

### 4. Placeholder Assets (10 minutes)

**You don't need custom art yet - use Engine Content:**

| Asset Needed | Placeholder Solution |
|--------------|---------------------|
| Player mesh | Use `Engine/Content/Characters/Mannequins/Meshes/SKM_Quinn` |
| Weapon model | Use simple static mesh cube scaled to gun shape |
| Artil mesh | Use sphere or `Engine/Content/BasicShapes/Sphere` + emissive material |
| Plant spot marker | Use plane with decal material (blue/pink circles) |
| Explosion VFX | Use `Engine/Content/VFX/Explosions/` particle system |
| Gunshot sound | Use `Engine/Content/Audio/` placeholder |

**Creating placeholder Artil mesh**:
1. Content Browser → Right-click → Material → M_Artil_Placeholder
2. Set Material Domain: Surface
3. Set Shading Model: Unlit
4. Add Constant3Vector (pink color) → Emissive Color
5. Set Bloom: 2.0
6. Save and apply to sphere mesh in BP_Artil

---

## Testing Workflow

### Phase 1: Solo Functionality Test (5 minutes)
1. **Open Dreamland_Prototype map**
2. **Click Play** (single player PIE)
3. **Test basic movement** - WASD, jump, camera
4. **Find an Artil** - walk to it, press E to pick up
5. **Verify pickup** - Artil should attach to character, UI should show carrier
6. **Walk to Dreamland zone** - look for blue debug box
7. **Hold E to plant** - progress bar should fill over 4s
8. **Release E early** - should interrupt plant
9. **Complete plant** - Artil becomes planted, countdown starts
10. **Switch to Guardian team** (manually possess other pawn if needed)
11. **Walk to planted Artil**
12. **Hold E to defuse** - 7s progress bar
13. **Complete defuse** - should end round, Guardians score +1

### Phase 2: Network Test (15 minutes)
1. **Editor Settings** → Play → Advanced:
   - Number of Players: 2
   - Net Mode: Play As Listen Server
2. **Click Play**
3. **Window 1 (Server)**: Pick up Artil, plant it
4. **Window 2 (Client)**: See Artil state replicate, attempt defuse
5. **Verify**:
   - Artil state syncs across clients
   - Plant/defuse progress visible to both
   - Round scoring updates for both

### Phase 3: Multi-Player Simulation (30 minutes)
1. **Set Number of Players: 4-6**
2. **Run PIE**
3. **Manually control different windows** to simulate:
   - Devils pick up both Artils
   - Plant one at Site A, one at Site B
   - Guardians defuse one
   - Let other detonate → check explosion damage/radius
4. **Test elimination win**:
   - Kill all Guardians → Devils should win round
5. **Test round rotation**:
   - Play multiple rounds
   - Verify score increments
   - Check first-to-6 match end

---

## Common Issues & Fixes

### "Can't pick up Artil"
- **Check**: Is `XSInteractionComponent` added to your Character Blueprint?
- **Fix**: Open your player Character BP → Add Component → XSInteractionComponent
- **Fix**: Bind Input Action "Use" (E key) to component's StartInteraction/StopInteraction

### "Artil won't plant"
- **Check**: Are you inside a DreamlandZone?
- **Fix**: Enable "Show Debug" on BP_DreamlandZone to see zone bounds
- **Check**: Is the plant spot close enough?
- **Fix**: Increase PlantSpot Radius in DreamlandZone

### "HUD not showing"
- **Check**: Is WBP_GameHUD set as the HUD widget in GameMode?
- **Fix**: In BP_DreamlandGameMode BeginPlay → Create Widget → Add to Viewport

### "Round never ends"
- **Check**: GameMode logs in Output Log
- **Fix**: Add debug logs to XSRoundBasedGameMode::CheckRoundEndConditions
- **Check**: Are player teams assigned correctly?

### "Replication not working"
- **Check**: Are you testing with "Play As Listen Server" or Dedicated Server?
- **Fix**: Don't use "Play As Standalone" for network tests
- **Check**: Is the Artil's GetLifetimeReplicatedProps called?
- **Fix**: Ensure DOREPLIFETIME macros are in ArtilActor.cpp

---

## Next Steps Checklist

Use this to track your progress:

- [ ] **Setup** (30 min)
  - [ ] Create BP_DreamlandGameMode
  - [ ] Create BP_Artil with placeholder mesh
  - [ ] Create BP_DreamlandZone
  - [ ] Create Dreamland_Prototype map

- [ ] **Map Building** (45 min)
  - [ ] Add floor, walls, cover
  - [ ] Place 2 DreamlandZones
  - [ ] Place 12 PlayerStarts (6 per team)
  - [ ] Place 2 Artils
  - [ ] Set World Settings GameMode

- [ ] **Player Setup** (15 min)
  - [ ] Add XSInteractionComponent to Character BP
  - [ ] Bind "Use" input action (E key)
  - [ ] Test pickup in PIE

- [ ] **HUD Creation** (60 min)
  - [ ] Create WBP_GameHUD with scores/timers
  - [ ] Create WBP_Scoreboard
  - [ ] Create WBP_ArtilNotification
  - [ ] Hook up to GameMode events

- [ ] **Testing** (60 min)
  - [ ] Solo test: pickup → plant → defuse
  - [ ] Network test: 2 players
  - [ ] Multi-player: 4-6 players
  - [ ] Full round flow test

- [ ] **Polish** (optional, later)
  - [ ] Replace placeholder meshes
  - [ ] Add VFX for plant/defuse/explosion
  - [ ] Add SFX
  - [ ] Improve UI styling
  - [ ] Add minimap

---

## File Structure Summary

After completing the above, your project should look like:

```
ProjectXS/
├── Source/ProjectXS/              # C++ (already done)
│   ├── XSRoundBasedGameMode.h/cpp
│   ├── ArtilActor.h/cpp
│   ├── DreamlandZone.h/cpp
│   ├── XSInteractionComponent.h/cpp
│   └── XSTeamPlayerStart.h/cpp
│
├── Content/
│   ├── Blueprints/
│   │   ├── BP_DreamlandGameMode     ← Create this
│   │   ├── Items/
│   │   │   └── BP_Artil             ← Create this
│   │   └── Zones/
│   │       └── BP_DreamlandZone     ← Create this
│   │
│   ├── Maps/
│   │   └── Dreamland_Prototype      ← Create this
│   │
│   └── UI/
│       ├── WBP_GameHUD              ← Create this
│       ├── WBP_Scoreboard           ← Create this
│       └── WBP_ArtilNotification    ← Create this
```

---

## Quick Start (5-Minute Smoke Test)

**Goal**: See if core systems work

1. Open Unreal Editor
2. Create map: File → New Level → Empty → Save as `Test`
3. Add floor BSP box
4. Place 1 PlayerStart
5. Window → World Settings → Game Mode = BP_DreamlandGameMode (if created) or XSRoundBasedGameMode
6. Play
7. Open Output Log → look for "[XSRoundBasedGameMode] Round 1 started" messages

If you see GameMode logs → **Core systems working!** ✅

If no logs → Check compilation errors in Output Log

---

## Debugging Tips

### Enable Verbose Logging
Add to `DefaultEngine.ini`:
```ini
[Core.Log]
LogGameMode=Verbose
LogNet=Verbose
LogPlayerController=Verbose
```

### Console Commands (in PIE)
- `stat fps` - Show FPS
- `stat unit` - Show frame time
- `stat net` - Show network stats
- `ShowDebug Net` - Detailed replication info
- `DumpConsoleCommands` - List all commands

### Useful Breakpoints (if using Visual Studio)
- `AXSRoundBasedGameMode::StartNewRound()` - Round start
- `AArtilActor::PickUp()` - Pickup logic
- `AArtilActor::StartPlanting()` - Plant logic
- `AXSRoundBasedGameMode::OnArtilPlanted()` - Score logic

---

## Additional Resources

- **UE5 Replication**: https://docs.unrealengine.com/5.0/en-US/networking-and-multiplayer-in-unreal-engine/
- **UMG UI**: https://docs.unrealengine.com/5.0/en-US/umg-ui-designer-for-unreal-engine/
- **GAS (Abilities)**: https://docs.unrealengine.com/5.0/en-US/gameplay-ability-system-for-unreal-engine/
- **Testing Multiplayer**: https://docs.unrealengine.com/5.0/en-US/testing-multiplayer-in-unreal-engine/

---

## Summary

**You already have**: All core C++ game logic (game mode, Artil, zones, interaction, spawns)

**You need to create**: 
1. 3 Blueprints (GameMode, Artil, Zone) - 30 min
2. 1 Test map with geometry and actors - 45 min  
3. 3 UMG widgets (HUD, scoreboard, notifications) - 60 min
4. Testing and iteration - 60 min

**Total estimated time to first playable**: ~3 hours

**You do NOT need custom art/models yet** - use engine placeholder assets and simple geometry. Focus on getting the gameplay loop working first, then iterate on visuals later.

Ready to start? Begin with the "Setup" checklist and work through each section. Good luck! 🎮

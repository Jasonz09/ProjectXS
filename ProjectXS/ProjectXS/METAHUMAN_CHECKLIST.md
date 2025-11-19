# ProjectXS MetaHuman Quick Start Checklist

## 🎯 Phase 1: MetaHuman Creation (Do This First)

### Prerequisites
- [ ] Epic Games account created
- [ ] Unreal Engine 5.x installed
- [ ] At least 50GB free disk space
- [ ] Stable internet connection

### Create Characters (1-2 hours each)

#### Shadow (Purple - Challenger)
- [ ] Go to https://metahuman.unrealengine.com/
- [ ] Create new MetaHuman
- [ ] Select: Athletic Male, Angular face, Short hair
- [ ] Eyes: Dark/Gray, Skin: Pale/Olive
- [ ] Export as "Shadow_MetaHuman"
- [ ] Download completed

#### Void (Cyan - Controller)
- [ ] Create new MetaHuman
- [ ] Select: Medium Male, Symmetric features
- [ ] Hair: Long/flowing, Eyes: Ice blue
- [ ] Skin: Cool-toned
- [ ] Export as "Void_MetaHuman"
- [ ] Download completed

#### Tech (Orange - Analyst)
- [ ] Create new MetaHuman
- [ ] Select: Heavy/Stocky Male, Square jaw
- [ ] Hair: Short/Bald, Eyes: Brown/Hazel
- [ ] Optional stubble
- [ ] Export as "Tech_MetaHuman"
- [ ] Download completed

#### Blaze (Red - Duelist)
- [ ] Create new MetaHuman
- [ ] Select: Athletic (any gender)
- [ ] Hair: FIERY RED, Wild/Spiky
- [ ] Eyes: Amber/Orange/Green
- [ ] Add burn scar if possible
- [ ] Export as "Blaze_MetaHuman"
- [ ] Download completed

#### Frost (Ice Blue - Sentinel)
- [ ] Create new MetaHuman
- [ ] Select: Large/Heavy Male
- [ ] Hair: White/Platinum/Gray
- [ ] Eyes: Icy blue/Pale gray
- [ ] Weathered features
- [ ] Export as "Frost_MetaHuman"
- [ ] Download completed

#### Splinter (Green - Splinter)
- [ ] Create new MetaHuman
- [ ] Select: Lean/Athletic Male
- [ ] Sharp features, messy hair
- [ ] Eyes: Bright green (or heterochromia)
- [ ] Unique styling
- [ ] Export as "Splinter_MetaHuman"
- [ ] Download completed

---

## 🎯 Phase 2: Unreal Engine Setup

### Project Setup
- [ ] Open ProjectXS in Unreal Engine
- [ ] Edit → Plugins → Enable "MetaHuman" plugin
- [ ] Edit → Plugins → Enable "Quixel Bridge" plugin
- [ ] Restart Unreal Engine

### Import MetaHumans
- [ ] Window → Quixel Bridge
- [ ] Sign in with Epic account
- [ ] For each character:
  - [ ] MetaHumans tab → Add MetaHuman
  - [ ] Select downloaded folder
  - [ ] Click "Add to Project"
  - [ ] Wait for import (5-10 min per character)
  - [ ] Verify in Content/MetaHumans/[Name]

---

## 🎯 Phase 3: Character Blueprints

### Create Blueprint for Each Character

#### Shadow Blueprint
- [ ] Content/Blueprints/Characters/ → New Blueprint
- [ ] Parent: XSAbilityCharacter
- [ ] Name: BP_Shadow_Character
- [ ] Mesh component → Set to Shadow MetaHuman
- [ ] Adjust: Location Z = -90, Rotation Z = -90
- [ ] Create Material Instance (Purple theme)
- [ ] Save

#### Void Blueprint
- [ ] Create: BP_Void_Character
- [ ] Assign Void MetaHuman mesh
- [ ] Material Instance (Cyan theme)
- [ ] Save

#### Tech Blueprint
- [ ] Create: BP_Tech_Character
- [ ] Assign Tech MetaHuman mesh
- [ ] Material Instance (Orange theme)
- [ ] Save

#### Blaze Blueprint
- [ ] Create: BP_Blaze_Character
- [ ] Assign Blaze MetaHuman mesh
- [ ] Material Instance (Red theme)
- [ ] Save

#### Frost Blueprint
- [ ] Create: BP_Frost_Character
- [ ] Assign Frost MetaHuman mesh
- [ ] Material Instance (Ice Blue theme)
- [ ] Save

#### Splinter Blueprint
- [ ] Create: BP_Splinter_Character
- [ ] Assign Splinter MetaHuman mesh
- [ ] Material Instance (Green theme)
- [ ] Save

---

## 🎯 Phase 4: Testing

### Basic Test (Per Character)
- [ ] Drag blueprint into level
- [ ] Press Alt+P to play
- [ ] Character spawns correctly
- [ ] Materials load properly
- [ ] No pink/purple missing textures
- [ ] Can move with WASD
- [ ] Animations play

### Full Game Test
- [ ] Set game mode to use character selection
- [ ] Test spawning each character from launcher
- [ ] Verify abilities work
- [ ] Check multiplayer (if applicable)

---

## 🎯 Phase 5: Polish (Optional - Do Later)

### Add Accessories
- [ ] Shadow: Tactical mask + visor
- [ ] Void: Ethereal helmet + energy streams
- [ ] Tech: AR visor + mechanical jaw
- [ ] Blaze: Half-burned mask
- [ ] Frost: Crystalline helmet
- [ ] Splinter: Split visor

### Animations
- [ ] Download combat animation pack from Marketplace
- [ ] Retarget to MetaHuman skeleton
- [ ] Create Animation Blueprints
- [ ] Test ability animations

---

## ⚡ Quick Commands (Terminal)

```bash
# Check Unreal project size
du -sh "/Users/jasonzhao/Documents/Unreal Projects/ProjectXS/ProjectXS"

# Create MetaHumans folder
mkdir -p "/Users/jasonzhao/Documents/MetaHumans"

# Check available disk space
df -h
```

---

## 🆘 Troubleshooting

### MetaHuman Creator Issues
- **Can't login**: Clear browser cache, try incognito mode
- **Slow loading**: Use Chrome/Edge, disable extensions
- **Export fails**: Check internet connection, try again

### Unreal Import Issues
- **Plugin not found**: Epic Games Launcher → Library → Unreal Engine → Options → Verify
- **Import fails**: Check disk space, restart Unreal
- **Pink materials**: Re-import MetaHuman, check file paths

### Character Blueprint Issues
- **T-pose in game**: Animation Blueprint not assigned
- **Floating**: Adjust Mesh Z-offset (-90 typical)
- **Facing wrong way**: Rotate Mesh Z = -90°
- **No movement**: Check input bindings, verify controller

---

## 📊 Progress Tracker

**Characters Created**: ☐ 0/6
**Characters Imported**: ☐ 0/6
**Blueprints Created**: ☐ 0/6
**Characters Tested**: ☐ 0/6

**Estimated Time Remaining**: ~30-50 hours
**Current Phase**: ☐ Planning / ☐ Creation / ☐ Import / ☐ Setup / ☐ Testing / ☐ Polish

---

## 🎬 Getting Started NOW

1. **Open browser** → https://metahuman.unrealengine.com/
2. **Sign in** with Epic Games account
3. **Click "Create MetaHuman"**
4. **Start with Shadow** (easiest to validate workflow)
5. **Follow character specs** from main guide
6. **Come back** when you have questions!

**Good luck! You've got this! 🚀**

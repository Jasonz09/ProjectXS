# MetaHuman Character Setup Guide for ProjectXS

## Overview
This guide will walk you through creating custom MetaHuman characters for all 6 agents in ProjectXS and integrating them into Unreal Engine.

---

## Part 1: MetaHuman Creator Setup

### Step 1: Access MetaHuman Creator
1. Open your web browser and go to: **https://metahuman.unrealengine.com/**
2. Sign in with your Epic Games account (same account you use for Unreal Engine)
3. Click **"Create MetaHuman"** to start

### Step 2: Character Creation Process

For each of our 6 characters, you'll create a unique MetaHuman:

---Create a stylized‑realistic playable character named "Shadow" for a competitive shooter. Male, mid‑20s, lean athletic build, natural proportions (playable hero, not cute or scary). Readable silhouette at gameplay/icon scale. Primary color family: layered purples with charcoal base and small cyan/lilac accents.

Face & Head:
- Angular but natural features: defined jaw, high cheekbones, straight nose, focused neutral expression.
- Skin: olive/medium, realistic skin shading, no explicit or extreme markings (a small tasteful scar near eyebrow optional).
- Hair: short tactical cut with slight undercut; dark base with subtle purple rim highlights.
- Eyes: natural iris color; optional faint lilac rim or tiny tech accent (grounded, non‑supernatural).

Outfit & Gear:
- Modern tactical / light cyberpunk: short hooded jacket over fitted combat vest, modular pouches, slim utility belt, compact hip holster, knee pads, urban tactical boots.
- Keep silhouette compact & readable; avoid long flowing cloth.
- Materials: matte technical fabric, low‑gloss carbon/ceramic armor panels, brushed metal hardware, leather straps.
- Distinct items: slim transparent half‑visor/eyepiece (separate mesh), single shoulder pauldron, detachable neck scarf, wrist device with faint cyan UI glow, small abstract emblem on chest.

Color Palette (include swatches):
- Primary purple #7B4CFF, deep charcoal #0F1724, mid lilac #8B5CF6, highlight lilac #C8A8FF, cyan accent #56F0FF, warm metal #E0B24C.

Pose, Lighting & Composition:
- Full‑body 3/4 view (heroic but playable), relaxed ready stance (one hand near holster), plus neutral headshot.
- Studio lighting: cool soft key, strong purple rim light from one side/back, subtle warm fill to reveal texture.

Style & Output:
- Stylized‑realistic PBR look, crisp surface detail, painterly polish. Deliver high‑res assets: 4k full‑body render, 3k headshot, front/side/back orthographics.
- Game‑ready notes: separate meshes for visor/jacket/armor/accessories, UVs non

## Character 1: Shadow (Shadow Operative)

### Visual Goals:
- **Face Shape**: Angular, sharp jawline, intimidating
- **Age**: Mid-20s to early 30s
- **Build**: Lean, athletic
- **Expression**: Serious, focused

### MetaHuman Settings:
1. **Body Type**: Select "Athletic Male" or "Lean Male"
2. **Face**:
   - Browse presets and select an angular, sharp face
   - Adjust: Cheekbones (high), Jaw (defined), Eyes (intense)
3. **Hair**: Short tactical cut or buzzed
4. **Skin**: Slightly pale or olive tone
5. **Eyes**: Dark brown or gray (piercing look)
6. **Special**: Add facial scars if available

### Post-Creation Notes:
- We'll add the tactical mask/visor in Unreal Engine as a separate mesh
- Save as: **"Shadow_MetaHuman"**

---

## Character 2: Void (Void Walker)

### Visual Goals:
- **Face Shape**: Mysterious, otherworldly features
- **Age**: Appears timeless (late 20s)
- **Build**: Medium build
- **Expression**: Calm, ethereal

### MetaHuman Settings:
1. **Body Type**: "Medium Male" or "Balanced Male"
2. **Face**:
   - Symmetric features
   - Slightly elongated face
   - High cheekbones
3. **Hair**: Long or medium flowing hair (will be partially covered by helmet)
4. **Skin**: Cool-toned, possibly with slight blue undertones
5. **Eyes**: Light blue or ice blue
6. **Special**: Minimal facial hair

### Post-Creation Notes:
- Helmet will cover most of head (create partial visibility design)
- Save as: **"Void_MetaHuman"**

---

## Character 3: Tech (Tech Specialist)

### Visual Goals:
- **Face Shape**: Square, strong, technical look
- **Age**: Mid-30s (experienced veteran)
- **Build**: Sturdy, medium-heavy
- **Expression**: Analytical, determined

### MetaHuman Settings:
1. **Body Type**: "Heavy Male" or "Stocky Male"
2. **Face**:
   - Square jaw
   - Strong brow
   - Calculating eyes
3. **Hair**: Short military cut or bald
4. **Skin**: Various tones (your preference)
5. **Eyes**: Brown or hazel
6. **Special**: Optional stubble or goatee

### Post-Creation Notes:
- Tech visor will cover upper face/eyes
- Mechanical jaw can be added as separate mesh
- Save as: **"Tech_MetaHuman"**

---

## Character 4: Blaze (Fire Duelist)

### Visual Goals:
- **Face Shape**: Wild, fierce, battle-hardened
- **Age**: Mid-20s (young but experienced)
- **Build**: Athletic, agile
- **Expression**: Intense, aggressive

### MetaHuman Settings:
1. **Body Type**: "Athletic Female" or "Athletic Male" (your choice)
2. **Face**:
   - Sharp features
   - Intense eyes
   - Strong expression
3. **Hair**: Fiery red, wild spiky style
4. **Skin**: Warm tones, can add freckles
5. **Eyes**: Amber, orange-brown, or bright green
6. **Special**: Add burn scar texture on one side if available

### Post-Creation Notes:
- Half-burned mask will cover part of face
- Red hair is signature element
- Save as: **"Blaze_MetaHuman"**

---

## Character 5: Frost (Ice Sentinel)

### Visual Goals:
- **Face Shape**: Strong, stoic, tank-like
- **Age**: 30s-40s (seasoned warrior)
- **Build**: Large, imposing
- **Expression**: Calm, unmovable

### MetaHuman Settings:
1. **Body Type**: "Heavy Male" or "Large Male"
2. **Face**:
   - Broad features
   - Strong jaw
   - Weathered appearance
3. **Hair**: White/platinum blonde or light gray (icy look)
4. **Skin**: Cool pale tones
5. **Eyes**: Icy blue or pale gray
6. **Special**: Mature features, possible scars

### Post-Creation Notes:
- Crystalline helmet will frame face
- Need imposing presence
- Save as: **"Frost_MetaHuman"**

---

## Character 6: Splinter (Splinter Agent)

### Visual Goals:
- **Face Shape**: Sharp, mysterious, deceptive
- **Age**: Late 20s
- **Build**: Lean, agile
- **Expression**: Mischievous, unpredictable

### MetaHuman Settings:
1. **Body Type**: "Lean Male" or "Athletic Male"
2. **Face**:
   - Sharp, fox-like features
   - Asymmetric styling
   - Playful yet dangerous
3. **Hair**: Medium length, messy/styled
4. **Skin**: Various tones
5. **Eyes**: Bright green or heterochromia (different colored eyes)
6. **Special**: Unique identifying mark/tattoo if available

### Post-Creation Notes:
- Split visor design (we'll add in Unreal)
- Holographic effects in engine
- Save as: **"Splinter_MetaHuman"**

---

## Part 2: Export from MetaHuman Creator

### For Each Character:

1. **After finishing customization**:
   - Click the **"Download"** button
   - Choose: **"Unreal Engine 5.x"** (match your version)
   - Select quality: **High Quality** (recommended)
   
2. **Export Options**:
   - ✅ Include LODs (Level of Detail)
   - ✅ Include animations (base set)
   - ✅ Include facial rig
   
3. **Download** will start - files are large (2-5GB per character)

4. **Save Location**: 
   - Create folder: `/Users/jasonzhao/Documents/MetaHumans/`
   - Each character gets subfolder: `Shadow/`, `Void/`, `Tech/`, etc.

---

## Part 3: Import into Unreal Engine

### Step 1: Install MetaHuman Plugin (if not already)

1. Open Unreal Engine
2. **Edit → Plugins**
3. Search for **"MetaHuman"**
4. Enable:
   - ✅ MetaHuman
   - ✅ MetaHuman Identity
   - ✅ Quixel Bridge
5. Restart Unreal Engine

### Step 2: Import MetaHuman to Project

#### Method A: Using Quixel Bridge (Recommended)

1. Open **Quixel Bridge** (Window → Quixel Bridge)
2. Sign in with Epic account
3. Go to **"MetaHumans"** tab
4. Click **"Add MetaHuman"**
5. Select your exported MetaHuman folder
6. Click **"Download"** or **"Add to Project"**
7. Bridge will automatically:
   - Import all assets
   - Set up materials
   - Configure skeletal mesh
   - Add to Content Browser

#### Method B: Manual Import

1. In Content Browser, create folder: **Content/Characters/MetaHumans/**
2. Create subfolders for each character:
   ```
   Content/Characters/MetaHumans/
   ├── Shadow/
   ├── Void/
   ├── Tech/
   ├── Blaze/
   ├── Frost/
   └── Splinter/
   ```
3. Navigate to downloaded MetaHuman folder
4. Drag and drop into Unreal Content Browser
5. Import settings:
   - ✅ Skeletal Mesh
   - ✅ Materials
   - ✅ Textures
   - ✅ Animation Blueprint

### Step 3: Verify Import

For each character:
1. Open **Content/Characters/MetaHumans/[CharacterName]/**
2. Double-click the **Skeletal Mesh** asset
3. You should see:
   - ✅ Character model loads
   - ✅ Materials applied correctly
   - ✅ No missing textures (pink/purple)
   - ✅ Skeleton appears in outliner

---

## Part 4: Create Character Blueprints

### Step 1: Base Character Class

We'll use the existing `XSAbilityCharacter` C++ class:

1. In Content Browser: **Content/Blueprints/Characters/**
2. **Right-click → Blueprint Class**
3. **Parent Class**: Select **"XSAbilityCharacter"**
4. Name it: **"BP_Shadow_Character"**

### Step 2: Assign MetaHuman Mesh

1. Open **BP_Shadow_Character**
2. Select **Mesh** component in Components panel
3. In Details panel → **Skeletal Mesh**:
   - Click dropdown
   - Select: **MetaHumans/Shadow/[ShadowMeshName]**
4. Adjust transform:
   - **Location Z**: -90 (offset to align with capsule)
   - **Rotation Z**: -90 (face forward)

### Step 3: Set Materials/Colors

1. In **Mesh → Materials** section
2. Create **Material Instance** for character color theme:
   - Right-click in Content Browser
   - **Create Material Instance**
   - Name: **MI_Shadow_Purple**
   - Set base color parameters to purple (#8B5CF6)

### Step 4: Repeat for All Characters

Create blueprints:
- **BP_Shadow_Character** (Purple theme)
- **BP_Void_Character** (Cyan theme)
- **BP_Tech_Character** (Orange theme)
- **BP_Blaze_Character** (Red theme)
- **BP_Frost_Character** (Ice blue theme)
- **BP_Splinter_Character** (Green theme)

---

## Part 5: Add Custom Armor/Accessories

### Creating Helmet/Mask Meshes

Since MetaHumans are just the base characters, you'll need to add:
- Tactical masks
- Helmets
- Visors
- Armor pieces

#### Option A: Modeling Software (Blender - Free)

1. **Install Blender**: https://www.blender.org/
2. **Export MetaHuman head** from Unreal:
   - Select head mesh → Export → FBX
3. **Import to Blender**:
   - File → Import → FBX
4. **Model helmet/mask** around head
5. **Export** as FBX
6. **Import to Unreal**
   - Place in: `Content/Characters/Accessories/`

#### Option B: Unreal Marketplace Assets

1. Browse **Unreal Marketplace**
2. Search: "Sci-fi helmet", "tactical gear", "cyberpunk armor"
3. Download free or paid assets
4. Attach to character blueprint as child components

### Attaching Accessories to Character

1. Open Character Blueprint (e.g., **BP_Shadow_Character**)
2. **Add Component → Static Mesh** or **Skeletal Mesh**
3. Name it: **"Helmet"** or **"Mask"**
4. **Parent Socket**: Attach to **"head"** socket of character mesh
5. **Transform**: Adjust position/rotation to fit
6. **Material**: Apply character-themed material

---

## Part 6: Animation Setup

### MetaHuman Animations

MetaHumans come with:
- ✅ Idle animations
- ✅ Walk/run cycles
- ✅ Facial animations
- ✅ Basic gestures

### Add Combat Animations

You'll need to add:
1. **Marketplace Animation Packs**:
   - Search: "Combat animations", "FPS animations", "Shooter pack"
   - Download and apply to character
   
2. **Retarget to MetaHuman Skeleton**:
   - In Unreal: **Window → Retargeting**
   - Source: Animation pack skeleton
   - Target: MetaHuman skeleton
   - Map bones automatically

3. **Create Animation Blueprint**:
   - Right-click → Animation → Animation Blueprint
   - Skeleton: MetaHuman skeleton
   - Add states: Idle, Walk, Run, Jump, Shoot, Ability

---

## Part 7: Testing Characters

### In-Editor Testing:

1. **Place Character in Level**:
   - Drag **BP_Shadow_Character** into viewport
   - Press **Play (Alt+P)**

2. **Test Movement**:
   - WASD to move
   - Space to jump
   - Mouse to look

3. **Verify**:
   - ✅ Character moves smoothly
   - ✅ Animations play correctly
   - ✅ Materials/colors show properly
   - ✅ Accessories attached correctly

### PIE (Play in Editor) with All Characters:

1. Create **Character Selection** system (link to launcher)
2. Spawn chosen character based on launcher selection
3. Test ability system integration

---

## Part 8: Integration with Launcher

### Connect to Electron Launcher:

In your game mode, read the selected character from launcher:
```cpp
// In your game mode BeginPlay
FString SelectedCharacterName = GetSelectedCharacterFromLauncher();

if (SelectedCharacterName == "Shadow") {
    SpawnCharacter(BP_Shadow_Character);
}
else if (SelectedCharacterName == "Void") {
    SpawnCharacter(BP_Void_Character);
}
// etc...
```

---

## Estimated Timeline

- **MetaHuman Creation**: 1-2 hours per character (6-12 hours total)
- **Export & Import**: 30 min per character (3 hours total)
- **Blueprint Setup**: 30 min per character (3 hours total)
- **Accessories/Armor**: 2-4 hours per character (12-24 hours total)
- **Animation Setup**: 4-8 hours total
- **Testing & Polish**: 4-8 hours

**Total**: ~30-50 hours of work

---

## Tips & Best Practices

### Performance:
- Use LODs (Level of Detail) to optimize for distance
- MetaHumans are high-poly - limit number on screen
- Use simpler meshes for multiplayer if needed

### Consistency:
- Keep similar body proportions across characters
- Use same animation rig for easier retargeting
- Maintain consistent scale (all characters same height base)

### Organization:
- Name everything clearly: `BP_CharacterName_Type`
- Use folders: `/Characters/MetaHumans/CharacterName/`
- Document your work in comments

### Common Issues:
- **Pink/Purple Materials**: Missing texture files - re-import
- **T-Pose Character**: Animation blueprint not assigned
- **Floating Character**: Adjust mesh Z-offset in blueprint
- **Wrong Facing**: Rotate mesh -90° on Z-axis

---

## Next Steps

1. Start with **Character 1 (Shadow)** - complete the full pipeline
2. Learn the workflow, take notes on issues
3. Apply lessons to remaining 5 characters
4. Test in-game before finalizing all
5. Add abilities and weapons
6. Polish and optimize

---

## Resources

- **MetaHuman Creator**: https://metahuman.unrealengine.com/
- **Unreal Docs - MetaHuman**: https://docs.unrealengine.com/5.0/en-US/metahumans/
- **Blender Download**: https://www.blender.org/download/
- **Unreal Marketplace**: https://www.unrealengine.com/marketplace/
- **MetaHuman Tutorial Playlist**: Search YouTube for "MetaHuman Unreal Engine tutorial"

---

## Questions?

If you encounter issues:
1. Check Unreal Engine logs (Window → Developer Tools → Output Log)
2. Verify plugin versions match
3. Ensure sufficient disk space (MetaHumans are large)
4. Community forums: https://forums.unrealengine.com/

Good luck creating your characters! 🎮

# Dreamland Assault - Game Design Document

## Game Overview
**Genre:** Tactical Team-based FPS  
**Mode:** Dreamland Assault (Unrated & Competitive)  
**Players:** 12 (6v6)  
**Teams:** Guardians vs Devils  
**Objective:** Attack/Defend objective-based gameplay with bomb planting mechanics

---

## Core Game Loop

### Win Condition
- **Unrated:** First team to win 6 rounds
- **Competitive:** Best of 13 rounds (first to 7 wins)
- Teams switch sides at halftime

### Round Structure
1. **Buy Phase** (30 seconds) - Players purchase weapons and equipment
2. **Combat Phase** (100 seconds) - Devils attempt to plant Artils, Guardians defend
3. **Post-Plant Phase** (50 seconds) - If planted, Guardians must defuse before detonation
4. **Round End** - Score update, economy adjustment, next round prep

---

## Teams & Roles

### Guardians (Defenders)
- **Objective:** Prevent Devils from planting both Artils OR defuse planted Artils
- **Win Conditions:**
  - Eliminate all Devils before both Artils are planted
  - Defuse at least one Artil before detonation
  - Survive until round timer expires (if no plants)
- **Strategy:** Map control, site holding, retake execution

### Devils (Attackers)
- **Objective:** Plant both Artils in Dreamland zones and protect them until detonation
- **Artil Carriers:** 2 random Devils selected at round start
- **Win Conditions:**
  - Plant both Artils and prevent defusal for 50 seconds
  - Eliminate all Guardians
- **Strategy:** Map pressure, execute coordination, post-plant positioning

---

## Artil Mechanics (Explosive Device)

### Visual Design
- **Shape:** Spiky sphere with pulsing energy core
- **Size:** Medium (similar to basketball size)
- **Appearance:** Dark metallic with glowing red veins/spikes
- **Audio:** Low hum when carried, high-pitched beeping when planted

### Carrier Mechanics
- **Selection:** 2 random Devils receive Artils at round start
- **Carry State:**
  - Slightly reduced movement speed (-5%)
  - Artil visible on player's back/side
  - Cannot be dropped voluntarily
  - Can switch weapons normally
- **Death Drop:**
  - Artil drops at death location
  - Glows with pickup indicator (red outline)
  - Any living Devil can pick up
  - Pickup time: 1 second (hold interaction)

### Planting Process
- **Location Requirements:**
  - Must be within designated Dreamland zone
  - Blue/pink outline marks valid plant locations
  - Multiple plant spots per zone (5-7 per site)
- **Plant Time:** 4 seconds (vulnerable, interruptible)
- **Plant Progress:**
  - Progress bar visible to planter
  - Audio cue increases in pitch
  - Taking damage interrupts plant (must restart)
- **Planted State:**
  - Artil embedded in ground, spikes extended
  - Pulsing red glow with countdown display
  - Audible beeping (increases frequency over time)

### Defusal Mechanics
- **Defuse Time:** 7 seconds per Artil
- **Requirements:**
  - Must be within defuse radius (2 meters)
  - Interruptible by damage
  - Progress bar visible to defuser
- **Partial Defuse:**
  - Progress saved if interrupted
  - Can be completed by different Guardian
- **Defuse Kit (Optional Item):**
  - Reduces defuse time to 3.5 seconds
  - Purchasable during buy phase
  - Single-use per round

### Detonation
- **Timer:** 50 seconds after both Artils planted
- **Explosion Radius:** 15 meters (instant kill)
- **Damage Falloff:** 15-25 meters (75-25% damage)
- **Visual Effect:** Massive energy burst, shockwave, debris
- **Audio:** Warning alarm (last 10 seconds), massive explosion sound
- **Map Impact:** Creates crater, destroys cover in radius

---

## Dreamland Zones (Bomb Sites)

### Zone Characteristics
- **Count:** 2 zones per map (Zone A, Zone B)
- **Size:** Large enough for tactical play (20x20 meters approx)
- **Visual Markers:**
  - Blue/pink holographic boundary outline
  - Floor markings indicating valid plant locations
  - Zone letter displayed on HUD when inside
- **Strategic Elements:**
  - Multiple entry points (3-4 per zone)
  - Cover positions for both attack and defense
  - High ground and low ground options
  - Choke points for Guardian control

### Plant Spot Types
1. **Default Spots** (5 per zone)
   - Standard planting locations
   - Visible from multiple angles
   
2. **Hidden Spots** (2 per zone)
   - Requires destruction of cover or special positioning
   - Harder to find for defusal
   - Riskier to plant (exposed during plant)

---

## Economy System

### Round Income
- **Round Loss:** $1,900
- **Round Loss (Consecutive):** +$500 per loss (caps at $3,400)
- **Round Win:** $3,000
- **Round Win (Plant):** +$300 bonus for Devils
- **Round Win (Defuse):** +$300 bonus for Guardians
- **Elimination Bonus:** $200 per kill

### Equipment Costs
- **Weapons:** $800 - $4,700
- **Armor:** $1,000 (full armor)
- **Defuse Kit:** $400 (Guardians only)
- **Utilities:** $200 - $600 (grenades, abilities)

### Economic Strategies
- **Eco Round:** Save money, minimal buys
- **Force Buy:** Spend all money despite economy
- **Full Buy:** Team has optimal loadouts
- **Save Round:** Preserve weapons for next round

---

## Round Flow Example

### Round Start (0:00)
1. Teams spawn at designated areas
2. 2 random Devils receive Artils (highlighted on team HUD)
3. 30-second buy phase begins
4. Teams strategize and purchase equipment

### Early Round (0:30 - 1:00)
1. Combat phase begins
2. Guardians take defensive positions in/around Dreamland zones
3. Devils advance toward zones, gathering map control
4. Early skirmishes determine map control

### Mid Round (1:00 - 1:30)
1. Devils execute site take (coordinated push)
2. Guardians rotate to defend or retake
3. Artil carriers move to plant locations
4. First Artil planted (if successful)

### Post-Plant (1:30 - 2:20)
1. Both Artils planted - 50-second timer begins
2. Devils hold defensive positions around Artils
3. Guardians execute retake strategy
4. Intense close-quarters combat around plant sites

### Round End
**Devils Win:**
- Both Artils detonate OR all Guardians eliminated
- Devils receive win bonus + economy

**Guardians Win:**
- At least one Artil defused OR all Devils eliminated
- Guardians receive win bonus + economy

---

## HUD Elements

### Always Visible
- Team score (top center)
- Round timer (top center)
- Player health/armor (bottom left)
- Ammo count (bottom right)
- Mini-map (top left/right)
- Ability cooldowns (bottom center)
- Team economy (top corner)

### Contextual
- **Artil Carrier:**
  - Artil status icon (carrying, planted)
  - Plant progress bar (when planting)
  
- **Near Artil:**
  - Distance to nearest Artil
  - Artil countdown timer
  - Defuse progress (if defusing)
  
- **In Dreamland Zone:**
  - Zone identifier (A or B)
  - Plant spot indicators (blue/pink outline)

---

## Game Modes

### Unrated
- First to 6 rounds wins
- No rank impact
- More casual matchmaking
- Full game mechanics enabled
- Practice for competitive

### Competitive
- Best of 13 rounds (first to 7)
- Ranked matchmaking
- Skill-based teams
- Round RR (Rank Rating) gain/loss
- Stricter leaver penalties
- Overtime rules (if 6-6, play until 2-round lead)

---

## Technical Requirements

### Networking (Server-Authoritative)
- Artil pickup/drop synchronization
- Plant/defuse progress replication
- Explosion damage calculation (server-side)
- Economy tracking per player
- Round state management
- Team assignment and Artil carrier selection

### Replication Priorities
1. **Critical (Immediate):**
   - Player positions and health
   - Artil status (carried, dropped, planted)
   - Plant/defuse progress
   - Explosion events

2. **High (Fast):**
   - Weapon fire and hits
   - Ability usage
   - Economy updates

3. **Medium (Regular):**
   - HUD updates
   - Score updates
   - Team chat

### Anti-Cheat Considerations
- Server validates all Artil interactions
- Plant location verification
- Defuse range checks
- Economy manipulation prevention
- Movement speed verification for carriers

---

## Character Integration

### All Characters Can:
- Carry/plant/defuse Artils
- Pick up dropped Artils (Devils only)
- Use unique abilities while carrying Artil
- Purchase equipment during buy phase

### Character Abilities Impact
- **Mobility Abilities:** Help Artil carriers reach plant sites faster
- **Defensive Abilities:** Protect planted Artils or deny plant attempts
- **Information Abilities:** Reveal enemy positions, Artil locations
- **Damage Abilities:** Clear defenders from sites or retake areas

---

## Map Design Principles

### Three-Lane Structure
- **Lane A:** Leads to Dreamland Zone A
- **Lane B:** Leads to Dreamland Zone B  
- **Mid:** Connector with rotation potential

### Key Areas
1. **Devil Spawn:** Safe starting area for attackers
2. **Guardian Spawn:** Safe starting area for defenders
3. **Dreamland Zone A:** First bomb site
4. **Dreamland Zone B:** Second bomb site
5. **Mid Control:** Fought-over area connecting lanes
6. **Choke Points:** Narrow passages Devils must push through
7. **Rotation Paths:** Routes for Guardians to switch sites

### Timing Considerations
- Devil spawn to Zone A: ~15 seconds
- Devil spawn to Zone B: ~15 seconds
- Guardian spawn to either zone: ~10 seconds
- Mid to either zone: ~8 seconds
- Zone A to Zone B rotation: ~12 seconds

---

## Audio Design

### Artil Sounds
- **Pickup:** Mechanical latch sound
- **Carrying:** Subtle humming (audible to nearby enemies)
- **Planting:** Increasing beep frequency + mechanical drilling
- **Planted:** Rhythmic beeping (faster as timer decreases)
- **Defusing:** Electronic whirring, wire cutting sounds
- **Detonation:** Massive explosion with shockwave audio

### Callout System
- **Automatic:** "Artil A planted!" "Artil B planted!"
- **Manual:** Player voice lines for callouts
- **Team Radio:** Filtered communication voice

---

## Victory Conditions Summary

### Devils Win Round If:
1. Both Artils planted + 50-second timer expires
2. All 6 Guardians eliminated (before or after plant)

### Guardians Win Round If:
1. At least one Artil defused (prevents full detonation)
2. All 6 Devils eliminated before both Artils planted
3. Round timer expires with neither Artil planted

### Match Victory:
- **Unrated:** First team to 6 round wins
- **Competitive:** First team to 7 round wins (overtime at 6-6)

---

## Next Steps for Implementation

### Phase 1: Core Mechanics (Weeks 1-4)
- [ ] Artil actor with pickup/drop/plant/defuse
- [ ] Dreamland zone boundaries and plant spots
- [ ] Basic round timer and win condition logic
- [ ] Team assignment and spawning

### Phase 2: Gameplay Systems (Weeks 5-8)
- [ ] Economy system and buy phase
- [ ] HUD elements and timer displays
- [ ] Explosion damage and radius
- [ ] Post-plant defusal mechanics

### Phase 3: Polish & Balance (Weeks 9-12)
- [ ] Audio implementation
- [ ] VFX for Artils and explosions
- [ ] Map design and playtesting
- [ ] Network optimization and testing

### Phase 4: Competitive Features (Weeks 13-16)
- [ ] Ranked matchmaking
- [ ] Leaderboards and stats
- [ ] Competitive ruleset and overtime
- [ ] Anti-cheat integration

---

## Open Questions for Design

1. **Artil Distribution:** Should carriers be truly random or role-based?
2. **Partial Completion:** Do both Artils need to detonate, or does one detonation = Devils win?
3. **Economy Reset:** Should losing team economy reset after certain conditions?
4. **Overtime Rules:** Sudden death? Extended rounds? Different win condition?
5. **Spectator Mode:** What information should dead players see?

---

**Document Version:** 1.0  
**Last Updated:** November 18, 2025  
**Status:** Initial Draft - Ready for Technical Review

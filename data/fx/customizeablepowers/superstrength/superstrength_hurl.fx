#########################################################
## Super Strength - Hurl
########################################################

FxInfo
Flags InheritAnimScale

Lifespan 105

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmR
		Sound Forcefield5_loop 60 60 0.4
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		part3	CustomizeablePowers\SuperStrength\StrengthGlowToss.part
		Sound rockpickup7 100.0 100.0 0.85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthStreak.part
		part3	CustomizeablePowers\SuperStrength\StrengthGlowToss.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	concrete
		Type	local
		At	Mystic
		bhvr	CustomizeablePowers\SuperStrength\projectileSuperStrength.bhvr
		geom	Hurl
		Lifespan 58
	End

	Event
		EName	Quake
		Type	local
		At	chest
		bhvr	CustomizeablePowers\SuperStrength\SlightQuake.bhvr
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	mystic
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\SuperStrength\superStrengthDust1.part
		part3	CustomizeablePowers\SuperStrength\superStrengthDust2.part
		Lifespan 20
	End
End

## SPLAT ######################################################

Condition
	On 	Time
	Time 	25

	Event
		EName	crack
		Type	start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\Splat_Hurl.bhvr
		Splat	StoneCracks.tga
		Lifespan 35
	End
End

########################################################

Condition
	On 	Time
	Time 	85

	Event
		EName	Streak1
		Type	local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows1.part
		part2	CustomizeablePowers\SuperStrength\SuperStrengthStar.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\SuperStrength\StrengthBodyGlows.part
		part2	CustomizeablePowers\SuperStrength\SuperStrengthStar.part
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	89

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	CustomizeablePowers\SuperStrength\SuperStrengthprojectile.bhvr
		geom	Hurl_projectile
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 0.9
	End
End

## ON HIT ######################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

########################################################

End
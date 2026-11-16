#########################################################
## Super Strength - Hurl
########################################################

FxInfo

Flags InheritAnimScale

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UARMR
		part1	:StrengthBodyGlows.part
		PMagnet	LarmR
		Sound Forcefield5_loop 60 60 .4
	End

	Event
		EName	Streak1
		Type	local
		At	UARML
		part1	:StrengthBodyGlows.part
		PMagnet	LarmL
	End

	Event
		EName	Streak1
		Type	local
		At	LarmR
		part1	:StrengthBodyGlows.part
		PMagnet	WepR
	End

	Event
		EName	Streak1
		Type	local
		At	LarmL
		part1	:StrengthBodyGlows.part
		PMagnet	WepL
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		part1	:StrengthStreak.part
		#part2	:StrengthPulse.part
		part3	:StrengthGlowToss.part
		Sound rockpickup7 100.0 100.0 .85
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		part1	:StrengthStreak.part
		#part2	:StrengthPulse.part
		part3	:StrengthGlowToss.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	concrete
		Type	local
		At	wepR
		geom	toss
		bhvr	behaviors/projectileSuperStrength.bhvr
	End

	Event
		EName	Quake
		Type	local
		At	chest
		bhvr	behaviors/SlightQuake.bhvr
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	cracks
		Type	start
		At	mystic
		#Part1	:SuperStrengthCrack01.part
		Part2	:superStrengthDust1.part
		part3	:superStrengthDust2.part
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
		Bhvr	:Splat_Hurl.bhvr
		Splat	StoneCracks.tga
		Lifespan	60
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
		part1	:StrengthBodyGlows1.part
		part2	:SuperStrengthStar.part
	End

	Event
		EName	Streak1
		Type	local
		At	WepR
		part1	:StrengthBodyGlows.part
		part2	:SuperStrengthStar.part
	End
End

Condition
	On 	Time
	Time 	95

	Event
		EName	concrete
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	93

	Event
		EName 	Prime
		Type	Start
		At	WepR
		geom	toss
		Bhvr	behaviors/SuperStrengthprojectile.bhvr
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 .9
	End
End

########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName	cracks
		Type	Destroy
	End
End

Condition
	On 	Time
	Time 	200

	Event
		EName	All
		Type	Destroy
	End
End

########################################################

End
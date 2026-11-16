#########################################################
## Stone Melee - Hurl Boulder
########################################################

FxInfo

Lifespan 200

Flags DontSuppress

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	WepR
		Sound rockpickup7 100.0 100.0 0.85
	End
End

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneMelee\stonehands.bhvr
		geom	StoneHandR
		LifeSpan 49
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneMelee\stonehands.bhvr
		geom	StoneHandL
		LifeSpan 49
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	local
		At	WepR
		part1	Powers\EarthControl\EarthHands.part
		part2	Powers\EarthControl\EarthHands2.part
		part	Powers\EarthControl\EarthHandsRocks.part
		lifespan 70
	End

	Event
		Type	local
		At	WepL
		part1	Powers\EarthControl\EarthHands.part
		part2	Powers\EarthControl\EarthHands2.part
		part	Powers\EarthControl\EarthHandsRocks.part
		lifespan 70
	End
End

########################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Mystic
		bhvr	behaviors\projectileHurlBolder.bhvr
		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05
		Lifespan 37
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	Mystic
		bhvr	behaviors\CameraShakeBoulderHurl.bhvr
		Part	Powers\EarthControl\EarthWeaponDustRocks2.part
		Part2	Powers\EarthControl\superStrengthDust1.part
		part3	Powers\EarthControl\superStrengthDust2.part
		Lifespan 35
	End
End

########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	crack
		Type	start
		At	Mystic
		Part1	Powers\EarthControl\StalagtiteCrack4.part
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	crack
		Type	start
		At	Mystic
		Part1	Powers\EarthControl\StalagtiteCrack5.part
	End
End

## PROJECTILE ######################################################

Condition
	On 	Time
	Time 	57 #87

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	behaviors\projectileHurlBolder2.bhvr
		geom	FX_Stone01
		#geom	Parent
		Sound Rockthrow 100 100 0.9
		Magnet	Target
		LookAt	Target
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

########################################################

End
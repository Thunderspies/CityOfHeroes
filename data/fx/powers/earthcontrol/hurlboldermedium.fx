#########################################################
##	Fireweapon
########################################################

FxInfo

Flags InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	WepR

		Sound rockpickup7 100.0 100.0 .85
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	Streak1
		Type	local
		At	WepR
		part1	:EarthHands.part
		part2	:EarthHands2.part
		part	:EarthHandsRocks.part
		lifespan	70
	End

	Event
		EName	Streak1
		Type	local
		At	WepL
		part1	:EarthHands.part
		part2	:EarthHands2.part
		part	:EarthHandsRocks.part
		lifespan	70
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	concrete
		Type	local
		At	Mystic
		geom	FX_Stone01
		geom	FX_Stone02
		geom	FX_Stone03
		geom	FX_Stone04
		geom	FX_Stone05
		bhvr	behaviors/projectileHurlBolderMedium.bhvr

	End


End

Condition
	On 	Time
	Time 	25

	Event
		EName	cracks
		Type	start
		At	mystic
		bhvr	behaviors/CameraShakeBoulderHurl.bhvr
		Part	:EarthWeaponDustRocks2.part
		Part2	:superStrengthDust1.part
		part3	:superStrengthDust2.part
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	crack
		Type	start
		At	mystic

		Part1	:StalagtiteCrack4.part
	End

End

Condition
	On 	Time
	Time 	37

	Event
		EName	crack
		Type	start
		At	mystic

		Part1	:StalagtiteCrack5.part
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
	Time 	92

	Event

		EName 	Prime
		Type	Start
		At	concrete

		geom	Parent
		Bhvr	behaviors\projectileHurlBolderMedium2.bhvr
		Magnet	Target
		LookAt	Target
		Sound Rockthrow 100 100 .9


	End

End


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

End
##################################


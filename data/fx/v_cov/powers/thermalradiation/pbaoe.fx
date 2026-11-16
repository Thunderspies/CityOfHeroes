#########################################################
## Thermal Radiation - Warmth
#########################################################

FxInfo

LifeSpan	65

### SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	T_Root
		Sound Firehand_loop 100.0 100.0 .45
		Sound Ignite2 100.0 100.0 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound cauterize 70 70 .95
		LifeSpan 55
	End

	Event
		Type	Local
		At	Chest
		Sound buildup 70 70 .36
		LifeSpan 50
	End

	Event
		Type	Local
		At	Chest
		Sound ignite2 50 50 .5
	End
End

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	T_Root
#		geom	HeatWave
#		bhvr	:WaveScale.bhvr
#		LifeSpan	85
#	End
#End

Condition
	On	Time
	Time	10

	Event
		EName	rootnode
		Type	local
		At	Root
		Geom	RootToChestAdjustment
		Sound BloodBoil2 80 80 .88
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End

## HANDS #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		LifeSpan	70
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		LifeSpan	70
	End
End

## AOE FX #######################################################

Condition
	On	Time
	Time	15

	Event
		EName	Rings
		Type	start
		At	Root

		Part4	:heal\HealRingsFlat.part
		Part	:heal\HealRingsFlat2.part
		LifeSpan	35
	End
End

Condition
	On	Time
	Time	20

	Event
		EName   RadiusFxScalex1
		Type	local
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex1
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	:FireShieldCloud01B.part
		Part2	:FireShieldCloud02B.part
		Part4	:FireShieldSparksB.part
		LifeSpan	30
	End
End

Condition
	On	Time
	Time	30

	Event
		EName   RadiusFxScalex2
		Type	local
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex2
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	:FireShieldCloud01B.part
		Part2	:FireShieldCloud02B.part
		Part4	:FireShieldSparksB.part
		LifeSpan	30
	End
End

Condition
	On	Time
	Time	55

	Event
		EName   RadiusFxScalex3
		Type	local
		At	root
		Bhvr	powers/firecontrol/FireShield.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex3
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		Part1	:FireShieldCloud01B.part
		Part2	:FireShieldCloud02B.part
		Part4	:FireShieldSparksB.part
		LifeSpan	30
	End
End

#########################################################


End
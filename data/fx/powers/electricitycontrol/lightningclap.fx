#############################################################
## LightningClap.fx
#############################################################

FxInfo

LifeSpan 65

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 .4
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Sound punchAA 50 50 .4
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	mystic
		bhvr	behaviors/ScaleBubble.bhvr
		Geom	ElectricalMeleeBubble
		LifeSpan 17
	End

	Event
		EName 	Mallet2
		Type	start
		At	mystic
		Part1	:ElectricityMeleeHitEmber.part
		Part2	:ElectricityMeleeHitStar.part
		Part3	:ElectricityMeleeHitRing.part
		Part4	:ElectricityMeleeHitRing2.part
		Part5	:ElectricityMeleeHitGlows.part
		Sound elecmeleehit 100.0 100.0 .8
		LifeSpan 9
	End

	Event
		EName 	shock
		Type	start
		At	mystic
		Part1	:ElectricityArchContinuinga.part
		Part2	:ElectricityArchContinuing2a.part
	End

	Event
		EName 	shock2
		Type	start
		At	mystic
		Part	:ElectricityArchLarge2.part
		Part	:ElectricityArchLarge.part
		lifeSpan 10
	End
End

#############################################################

End
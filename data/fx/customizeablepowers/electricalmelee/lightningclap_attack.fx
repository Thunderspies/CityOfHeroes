#############################################################
## LightningClap_Attack.fx
#############################################################

FxInfo

LifeSpan 65

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Sound eleccontrol2 50 50 0.4
	End

	Event
		Type	local
		At	WepL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
		Sound punchAA 50 50 0.4
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	mystic
		bhvr	:ScaleBubble.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_ElectricalMeleeBubble
		LifeSpan 17
	End

	Event
		Type	start
		At	mystic
		Part1	:ElectricityMeleeHitEmber.part
		Part2	:ElectricityMeleeHitStar.part
		Part3	:ElectricityMeleeHitRing.part
		Part4	:ElectricityMeleeHitRing2.part
		Part5	:ElectricityMeleeHitGlows.part
		Sound elecmeleehit 100.0 100.0 0.8
		Lifespan 6
	End

	Event
		Type	start
		At	mystic
		Part1	:ElectricityArchContinuinga.part
		Part2	:ElectricityArchContinuing2a.part
	End

	Event
		Type	start
		At	mystic
		Part	:ElectricityArchLarge2.part
		Part	:ElectricityArchLarge.part
		lifeSpan 10
	End
End

#############################################################

End
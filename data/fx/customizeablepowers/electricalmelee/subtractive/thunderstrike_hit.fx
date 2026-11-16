#############################################################
## ThunderStrike_Hit.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Chest
		bhvr	:ScaleBubble.bhvr
		BhvrOverride
			Alpha			15
			TintGeom		1
		End
		Geom	Tintable_Dark_ElectricalMeleeBubble
		LifeSpan 5
	End

	Event
		Type	start
		At	Chest
		Part1	:ElectricityMeleeHitEmber.part
		Part2	:ElectricityMeleeHitStar.part
		Part3	:ElectricityMeleeHitRing.part
		Part4	:ElectricityMeleeHitRing2.part
		Part5	:ElectricityMeleeHitGlows.part
		Sound elecmeleehit 80.0 80.0 0.8
		Lifespan 6
	End

	Event
		Type	start
		At	Chest
		Part1	:ElectricityArchContinuinga.part
		Part2	:ElectricityArchContinuing2a.part
	End

	Event
		Type	start
		At	Chest
		Part	:ElectricityArchLarge2.part
		Part	:ElectricityArchLarge.part
		lifeSpan 10
	End

	Event
		Type	Local
		At	head
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:ElectricityMeleeGlowHit2.part
	End
End

####################################################################

End
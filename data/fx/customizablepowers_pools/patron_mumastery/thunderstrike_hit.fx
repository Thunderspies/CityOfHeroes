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
		bhvr	CustomizeablePowers\ElectricalMelee\ScaleBubble.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_ElectricalMeleeBubble
		LifeSpan 5
	End

	Event
		Type	start
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityMeleeHitEmber.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityMeleeHitStar.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityMeleeHitRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityMeleeHitRing2.part
		Part5	:ElectricityMeleeHitGlows.part
		Sound elecmeleehit 80.0 80.0 0.8
		Lifespan 6
	End

	Event
		Type	start
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuinga.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuing2a.part
	End

	Event
		Type	start
		At	Chest
		Part	CustomizeablePowers\ElectricalMelee\ElectricityArchLarge2.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityArchLarge.part
		lifeSpan 10
	End

	Event
		Type	Local
		At	head
		Part1	CustomizeablePowers\ElectricalMelee\HeadDizzies.part
		Part2	CustomizeablePowers\ElectricalMelee\HeadDizzyRings.part
		part3	CustomizeablePowers\ElectricalMelee\ElectricityMeleeGlowHit2.part
	End
End

####################################################################

End
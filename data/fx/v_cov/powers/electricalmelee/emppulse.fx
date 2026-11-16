#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	chest
		Part	V_COV\Powers\ElectricalMelee\Dots.part
		Part	V_COV\Powers\ElectricalMelee\DotsWhite.part
		sound	arrowhitEMP 80 70 .8
		Lifespan 10
	End

	Event
		Type	start
		At	chest
		Part	V_COV\Powers\ElectricalMelee\HitEmber.part
		Part	V_COV\Powers\ElectricalMelee\HitRing.part
		Part	V_COV\Powers\ElectricalMelee\HitRing2.part
		Part	V_COV\Powers\ElectricalMelee\HitGlows2.part
		Part	V_COV\Powers\ElectricalMelee\HitStar.part
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	start
		At	chest
		Part	V_COV\Powers\ElectricalMelee\EMPElectricityArch1.part
		Part	V_COV\Powers\ElectricalMelee\EMPElectricityArch2.part
		Part	V_COV\Powers\ElectricalMelee\EMPElectricityArch1White.part
		Part	V_COV\Powers\ElectricalMelee\EMPElectricityArch2White.part
		Lifespan 7
	End
End

#############################################################

End
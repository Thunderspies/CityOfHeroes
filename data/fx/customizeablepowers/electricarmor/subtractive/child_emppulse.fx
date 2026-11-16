#############################################################
## Child_EmpPulse.fx
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
		Part	:Dots.part
		Part	:DotsWhite.part
		sound	arrowhitEMP 80 70 0.8
		Lifespan 10
	End

	Event
		Type	start
		At	chest
		Part	:HitEmber.part
		Part	:HitRing.part
		Part	:HitRing2.part
		Part	:HitGlows2.part
		Part	:HitStar.part
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	start
		At	chest
		Part	:EMPElectricityArch1.part
		Part	:EMPElectricityArch2.part
		Part	:EMPElectricityArch1White.part
		Part	:EMPElectricityArch2White.part
		Lifespan 7
	End
End

#############################################################

End
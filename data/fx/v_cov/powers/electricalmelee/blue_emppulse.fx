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
		Part	:Dots_Blue.part
		Part	:DotsWhite.part
		sound	arrowhitEMP 80 70 .8
		Lifespan 10
	End

	Event
		Type	start
		At	chest
		Part	:Blue_HitEmber.part
		Part	:Blue_HitRing.part
		Part	:Blue_HitRing2.part
		Part	:Blue_HitGlows2.part
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
		Part	:Blue_EMPElectricityArch1.part
		Part	:Blue_EMPElectricityArch2.part
		Part	:EMPElectricityArch1White.part
		Part	:EMPElectricityArch2White.part
		Lifespan 7
	End
End

#############################################################

End
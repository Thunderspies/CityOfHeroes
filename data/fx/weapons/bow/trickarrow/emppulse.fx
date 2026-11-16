#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 1 1 1

LifeSpan 70

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet2
		Type	start
		At	Origin
		Part	:Dots.part
		sound	arrowhitEMP 80 70 .8
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet2
		Type	start
		At	Origin
		Part	:HitEmber.part
		Part	:HitStar.part
		Part	:HitRing.part
		Part	:HitRing2.part
		Part	:HitGlows2.part
	End
End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Mallet2
		Type	start
		At	Origin
		Part	:EMPElectricityArch1.part
		Part	:EMPElectricityArch2.part
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	7

	Event
		EName	Mallet1
		Type	Destroy
	End
End

#############################################################

End
#########################################################
##	EBlast
########################################################
FxInfo

Input
	Inpname	Chest
End

LifeSpan	90

#################################################################
##Hands
#################################################################



Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		#Part1	:ElectricityGlow.part
		#Part2	:ElectricityGlowStar.part
		#Part3	:ElectricityArch4a.part
		#Part4	:ElectricityArchSmalla.part
		Part	:BodyArcsSmall01.part
		Sound taser3 80 80 .92
	End

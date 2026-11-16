#########################################################
##	template

FxInfo


Lifespan 60

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time	
	Time	0

	Event
		Type Local
		At origin
		Sound gravitylift 80 80 .88
	End

	Event
		EName 	Mallet
		Type	local 
		At	Waist
		Part	Powers/Kinetics/RamdomStreaks.part
		Part	Powers/Kinetics/MiddleGlow.part
		Part	Powers/Kinetics/MiddleStreak2.part
		Part	Powers/Kinetics/MiddleGlowRing.part
		Part	Powers/Kinetics/MiddleGlowElipse.part
		Part	Powers/Kinetics/RamdomStreaksBlue.part

		Lifespan 10
	End

End


End		
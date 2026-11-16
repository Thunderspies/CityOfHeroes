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
		Part	:RandomStreaks.part
		Part	:MiddleGlow.part
		Part	:MiddleStreak2.part
		Part	:MiddleGlowRing.part
		Part	:MiddleGlowElipse.part
		Part	:RandomStreaksBlue.part

		Lifespan 10
	End

End


End
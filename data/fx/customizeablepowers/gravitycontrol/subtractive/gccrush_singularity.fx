#########################################################
##	template

FxInfo


Lifespan 60

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition

	On 	Time
	Time	30

	Event
		Type Local
		At origin
		Sound gravityzap2 80 80 .88
	End

	Event
		EName 	Mallet
		Type	local
		At	WepL
		Part	:RandomStreaks.part
		Part	:RandomStreaksBlue.part

		Lifespan 10
	End

	Event
		EName 	Mallet
		Type	local
		At	T_Waist
		Part	:RandomStreaks.part
		Part	:MiddleGlow.part
		Part	:MiddleGlow_Alpha.part
		Part	:MiddleStreak2.part
		Part	:MiddleGlowRing.part
		Part	:MiddleGlowElipse.part
		Part	:RandomStreaksBlue.part

		Lifespan 10
	End

End

End
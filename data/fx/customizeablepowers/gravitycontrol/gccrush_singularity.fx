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
		Part	CustomizeablePowers\GravityControl\RandomStreaks.part
		Part	CustomizeablePowers\GravityControl\RandomStreaksBlue.part

		Lifespan 10
	End

	Event
		EName 	Mallet
		Type	local
		At	T_Waist
		Part	CustomizeablePowers\GravityControl\RandomStreaks.part
		Part	CustomizeablePowers\GravityControl\MiddleGlow.part
		Part	CustomizeablePowers\GravityControl\MiddleStreak2.part
		Part	CustomizeablePowers\GravityControl\MiddleGlowRing.part
		Part	CustomizeablePowers\GravityControl\MiddleGlowElipse.part
		Part	CustomizeablePowers\GravityControl\RandomStreaksBlue.part

		Lifespan 10
	End

End

End
#############################################################
## EnduranceHands.fx
#############################################################

FxInfo

Lifespan 55

#############################################################
Condition
	On	Time
	Time	0

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Prime
		AltPiv 1
		BHVR	V_COV\Powers\Necromacy\Litcheye.bhvr
		Part	:Eye_Glow.part
		Part	:Eye_Glow_Bright.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv 2
		Part	:Eye_Glow.part
		Part	:Eye_Glow_Bright.part

	End
End

Condition
	On 	Time
	Time 	0


	Event
		EName	left
		Type	Local
		At	WepL
		Part	ENEMYFX\Ghouls\Inspiration_General_Glow_Continuing.part
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
		sound 	heal4 50 30 .55
	End

	Event
		EName	Right
		Type	Local
		At	WepR
		Part	ENEMYFX\Ghouls\Inspiration_General_Glow_Continuing.part
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
	End
End

#############################################################

End
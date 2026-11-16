#########################################################
## Umbral Blast - Starless Step
#########################################################

FxInfo

Lifespan 300

## EYE FX ######################################################

Condition
	On	Time
	Time	5
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Sound teletrans5 80 80 .88
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyes.fx
	End
End

## HANDS ######################################################

Condition
	On	Time
	Time	5
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:HeadGlow.part
		part	:HeadGlowPurple.part
		part	:HeadGlowBlue.part
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:HeadGlow.part
		part	:HeadGlowPurple.part
		part	:HeadGlowBlue.part
	End
End

########################################################

End
#########################################################
## Umbral Blast - Starless Step
#########################################################

FxInfo

Lifespan 40

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

## HANDS #######################################################

Condition
	On	Time
	Time	0

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:HeadGlow.part
		part	:HeadGlowPurple.part
		part	:HeadGlowBlue.part
		
		LifeSpan	27
	End
End

Condition
	On	Time
	Time	20
		
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:HandBurst.part
		part	:HandBurstPurple.part
		part	:HandBurstBlue.part
		part	:LightRays.part
	End
End

#########################################################

End
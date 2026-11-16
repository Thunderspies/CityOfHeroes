#########################################################
## Umbral Blast - Gravatic Emenation
#########################################################

FxInfo

LifeSpan	200

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Sound WarshadeQuick 80 80 .93
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Sound Warshadeblast1 60 60 .55
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

## HANDS #######################################################

Condition
	On 	Time
	Time 	15
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 5
	End
End

#########################################################

End			
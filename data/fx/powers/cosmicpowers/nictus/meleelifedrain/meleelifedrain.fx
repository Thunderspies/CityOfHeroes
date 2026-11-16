#########################################################
## Umbral Blast - Gravity Well
#########################################################

FxInfo

LifeSpan	60

## SOUND FX ######################################################

Condition
	On	Time
	time	16

	Event
		EName 	Orb2
		Type	Local 
		At	WepL
		Sound warshadelifedrain2 70 70 .7
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

## MELLE FX ######################################################

Condition
	Event
		EName 	Orb2
		Type	Local 
		At	UarmL
		part	:UpperTendrilBlue.part
		part	:UpperTendrilPurple.part
		part	:UpperTendrilBlack.part
		PMagnet	LarmL
	End

	Event
		EName 	Orb2
		Type	Local 
		At	LarmL
		part	:TendrilBlue.part
		part	:TendrilPurple.part
		part	:TendrilBlack.part
		PMagnet	WepL
	End
End

Condition
	On	Time
	time	16

	Event
		EName 	Orb2
		Type	Local 
		At	WepL
		part	:TendrilBlackHand.part
		part	:TendrilBlueHand.part
		part	:TendrilPurpleHand.part
		part	:TendrilBlackHandOut.part
		part	:TendrilPurpleHandOut.part
		part	:TendrilBlueHandOut.part
		PMagnet	T_Chest
	End
End

########################################################

End			
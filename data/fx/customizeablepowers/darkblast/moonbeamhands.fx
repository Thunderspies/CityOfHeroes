#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	100
#########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand1
		Type	Local
		At	UarmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:DarkBlastTenticleWhite.part
		Part3	:DarkHandsEmberb.part
		Part3	:DarkHandsEmberb_Alpha.part
		Sound darkwindup2 80 80 .7
		PMagnet	LarmR
	End

	Event
		EName 	Hand2
		Type	Local
		At	LarmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:DarkBlastTenticleWhite.part
		Part3	:DarkHandsEmberb.part
		Part3	:DarkHandsEmberb_Alpha.part

		PMagnet	WepR
	End
End

#########################################################

End



#############################################################
## HailOfBullets_Attack.fx
#############################################################

FxInfo

## DOVES ###########################################################

Condition
	On 	Cycle
	Time 	3
	Repeat	1
	RepeatJitter 1

	Event
		EName	Doves
		Type	Start
		At	Chest
		Bhvr	WEAPONS\DualPistols\FadeInFadeOut.bhvr
		BhvrOverride
			StartJitter		0 1 0
			Scale			0.375 0.375 0.375
			PyrRotateJitter		15 180 0
		End
		Anim	CardBoardBox_LG
		Lifespan 30
		LifespanJitter 10
	End
End

#############################################################

End
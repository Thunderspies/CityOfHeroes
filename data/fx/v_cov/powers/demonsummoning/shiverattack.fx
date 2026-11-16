#############################################################
## ShiverAttack.fx
#############################################################

FxInfo
LifeSpan	200

#####################################################################
Condition
	On	Time
	Time	8

	Event
		Ename   LeftHandMist
		Type	PositOnly
		At	WepL
		Part	:Shiver_SnowBurst.part
		Part	:Hand_Snow_Trail.part
		Sound Coldblast2 100 100 .8
		LifeSpan	70
	End


	Event
		Ename   RightHandMist
		Type	PositOnly
		At	WepR
		Part	:Shiver_SnowBurst.part
		Part	:Hand_Snow_Trail.part

		LifeSpan	70
	End
End

Condition
	On	Time
	Time	28

	Event
		EName 	Prime2
		Type	Local
		At	Head
		Part3	:Breath_Cold_Snowflakes.part
		Part3	:Shiver_Snowflakes.part
		Part3	:Shiver_Mist.part
		Part3	:Shiver_Mist_Alpha.part

		LifeSpan	37
	End

End

End
#############################################################
## Physics_Sparks.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	0
	Chance	.75
	Event
		Type	Start
		At	Chest
		BhvrOverride
			StartJitter	0.5 0 0.5
		End
	#	Part	:Physics_Fire.part
		Part	:Physics_Fire_Core.part
		Lifespan	90
		LifespanJitter	30
		CThresh 	.001
		CDestroy 	0
	End

End


Condition
	On	Time
	Time	0
	Chance	.3

	Event
		Type	Start
		At	Chest
		BhvrOverride
			StartJitter	0.5 0 0.5
		End
		Part	:Physics_Fire.part
		Part	:Physics_Fire_Core.part
		Lifespan	90
		LifespanJitter	30
		CThresh 	.001
		CDestroy 	0
	End
End
#############################################################

End

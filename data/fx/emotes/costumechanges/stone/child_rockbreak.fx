#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 60

#############################################################


Condition
	On 		Time
	Time 		0

	Event
		Type	start
		At	Origin

		Part	:CHILD_RockBreak_Dust.part
		Part	:CHILD_RockBreak.part
		CEvent	WORLD\tunnelsDebrisConcrete_SFX.fx
		Lifespan 500
		LifespanJitter	90
	#	CThresh 	0.00001
	End

	Event
		Type	start
		At	Origin

		Part	:CHILD_RockBreak_Dust.part
		Part	:CHILD_RockBreak.part

		Lifespan 500
		LifespanJitter	90
	#	CThresh 	0.00001
	End
End
#############################################################

End
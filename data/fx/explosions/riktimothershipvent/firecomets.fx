#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Lifespan 300

## FIERY COMET #######################################################

Condition
	Time 	3
	Chance	1.0
	Repeat	3
	RepeatJitter	1

	Event
		Type	Local
		At	Origin
		Bhvr	:FireComets.bhvr
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:FireTrail1.part
		Part1	:FireTrail2.part
		Lifespan	75
		LifespanJitter	15
	End
End

Condition
	Time 	3
	Chance	1.0
	Repeat	6
	RepeatJitter	2

	Event
		HardwareOnly
		Type	Local
		At	Origin
		Bhvr	:FireComets.bhvr
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	:FireTrail1.part
		Part1	:FireTrail2.part
		Lifespan	75
		LifespanJitter	15
	End
End

#######################################################################

End
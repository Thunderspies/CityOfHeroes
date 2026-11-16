#############################################################
## BiteHit.fx
#############################################################

FxInfo

Lifespan 105

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Burst
		Type	Local
		At	Waist
		bhvr	behaviors\FX_SnakeMen_HitOffset.bhvr
		Part	V_COV\EnemyPowers\SnakeMen\SM_BiteHit.part
		Part	V_COV\EnemyPowers\SnakeMen\SM_BiteHit1.part
		Part	V_COV\EnemyPowers\SnakeMen\SM_BiteHit2.part
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Burst
		Type	Local
		At	Waist
		bhvr	behaviors\FX_SnakeMen_HitOffset.bhvr
		Part	V_COV\EnemyPowers\SnakeMen\SM_PoisonSpark2.part
		Lifespan 5
	End
End

#############################################################

End
#########################################################
## Continuing FX - Buff Damage (Old)
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Chestnode
		Type	Posit
		At	Chest
		Geom	ChestPivotsBuffDam
		Bhvr	:BuffDamageSpin.bhvr
		Lifespan 0
	End
End

Condition
	On	Cycle
	Time	120

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	1
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		part	:BuffDamageGlows01.part
		Lifespan 60
	End
End

#############################################################

End
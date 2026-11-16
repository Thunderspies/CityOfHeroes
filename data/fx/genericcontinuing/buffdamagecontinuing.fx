#########################################################
## Continuing FX - Buff Damage
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
		Type	Local
		At	Chest
		BhvrOverride
			Alpha	1
		End
		Geom	ChestPivotsBuffDam
		Lifespan	75
	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	1
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At		Chestnode
		AltPiv	2
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At		Chestnode
		AltPiv	3
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At		Chestnode
		AltPiv	4
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At		Chestnode
		AltPiv	5
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	25
	Event
		Type	Local
		At		Chestnode
		AltPiv	6
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	30
	Event
		Type	Local
		At		Chestnode
		AltPiv	7
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	35
	Event
		Type	Local
		At		Chestnode
		AltPiv	8
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	40
	Event
		Type	Local
		At		Chestnode
		AltPiv	9
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	45
	Event
		Type	Local
		At		Chestnode
		AltPiv	10
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	50
	Event
		Type	Local
		At		Chestnode
		AltPiv	11
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	55
	Event
		Type	Local
		At		Chestnode
		AltPiv	12
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	60
	Event
		Type	Local
		At		Chestnode
		AltPiv	13
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	65
	Event
		Type	Local
		At		Chestnode
		AltPiv	14
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	70
	Event
		Type	Local
		At		Chestnode
		AltPiv	15
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

Condition
	Time	75
	Event
		Type	Local
		At		Chestnode
		AltPiv	1
		part	:BuffDamageShards01.part
		part	:BuffDamageShards02.part
		#part	:BuffDamageGlows01.part
		Lifespan	20
	End
End

#########################################################

Condition
	On	Cycle
	Time	150

	Event
		ENAME	CrossHairNode
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX Genericcontinuing\BuffDamageContinuing.fx
		Lifespan	225
	End
End

#########################################################

End
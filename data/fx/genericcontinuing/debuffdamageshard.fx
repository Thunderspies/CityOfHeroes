#############################################################
## HEADER
#############################################################

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
		At	Origin
		Geom	ChestPivotsBuffDam
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	15
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	5

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	14
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	13
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	15

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	12
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	11
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	10
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	9
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	8
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	40

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	45

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	50

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	55

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	60

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	65

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	70

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	1
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	75

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	15
		part	:DeBuffDamageShards01.part
		part	:DeBuffDamageShards02.part
		part	:DeBuffDamageGlows01.part
		Lifespan 60
	End
End

#############################################################

End
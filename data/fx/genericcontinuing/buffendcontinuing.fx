#########################################################
## Continuing FX - Buff End
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
		At	chest
		#Bhvr	:BuffHPSpin.bhvr
		BhvrOverride
			Alpha 1
		End
		Geom	ChestPivotsBuffEnd
		Lifespan	30
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
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition

	On	Time
	Time	6

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	8

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	6
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	14

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	8
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	16

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	9
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	18

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	10
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
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
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition

	On	Time
	Time	22

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	12
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition

	On	Time
	Time	24

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	13
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition
	On	Time
	Time	26

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	14
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

Condition

	On	Time
	Time	28

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	15
		part	:BuffEndFlash01.part
		part	:BuffEndGlows01.part
		part	:BuffEndGlows02.part
		Lifespan	30
	End
End

#########################################################

Condition
	On	Cycle
	Time	90

	Event
		ENAME	Repeat
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX Genericcontinuing\BuffEndContinuing.fx
		Lifespan	60
	End
End

#########################################################

End
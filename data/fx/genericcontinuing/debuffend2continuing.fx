#############################################################
## Continuing FX - Debuff Endurance
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	90

	Event

		Type	Local
		At	Chest
		Sound spiral_loop 30 30 .24
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 125
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Chestnode
		Type	Local
		At	chest
		Geom	ChestPivotsBuffEnd
		Bhvr	:BuffHPSpin.bhvr
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	1
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	1

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	2

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	4

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	6
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	6

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	8
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	8

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	9
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	9

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	10
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	11
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	11

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	12
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	13
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	13

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	14
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	14

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	15
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	1
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	16

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	2
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	17

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	3
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition
	On	Time
	Time	18

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	4
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	19

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	5
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	6
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	21

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	7
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	22

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	8
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	23

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	9
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	24

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	10
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	11
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	26

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	12
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	27

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	13
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	28

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	14
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

Condition

	On	Time
	Time	29

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chestnode
		AltPiv	15
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
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
		AltPiv	1
		part	:DeBuffEndFlash01.part
		part	:DeBuffEndFlash02.part
		part	:DeBuffEndGlows01.part
		part	:DeBuffEndGlows03.part
		part	:DeBuffEndGlows02.part
		Lifespan 60
	End
End

############################################################

Condition
	On	Cycle
	Time	90

	Event
		ENAME	CrossHairNode
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX Genericcontinuing\DeBuffEnd2.fx
		Bhvr	:BuffHPSpin.bhvr
		Lifespan 60
	End
End

#############################################################

End
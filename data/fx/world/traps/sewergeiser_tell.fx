#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo
lifespan 500

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Steam
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 -1 0
			Scale		1.25 1.25 1.25
		End
		Part	:Bottom_Foam_Flat.part
		Part	:Bottom_Foam_Flat.part
	End
End

Condition
	On	Time
	Time	15

	Event
		Ename	Bubbles
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	:Sewer_Bubbles_Small.fx
		Sound	Sewer_SewerBubble_Start_01 100 100 .8
	End

	Event
		Type	Local
		At	Root
		Sound 	Sewer_SewerBubble_Loop 100 100 .8
		bhvr	behaviors/Sound/SoundIn2Out2.bhvr
	End

End

#############################################################

End
#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo
Lifespan 350

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Steam
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:Toxic_Fumes.part
		Part	:Toxic_Ripples.part
		Lifespan 100
	End

	Event
		Ename	Steam
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
	#	ChildFX	:Sewer_Bubbles_Small.fx
		Part	:Green_Bubbles.part
		Sound 	Sewer_SewerBubble_Start_01 100 100 .8
		Lifespan 70
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Bhvr	:Toxic_Bubble_Grow.bhvr
		Part	:Toxic_Bubble_Small.part
		Part	:Toxic_Bubble_big.part
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:Toxic_Ripple_Big.part
	End
End

Condition
	On	Time
	Time	128

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.2 0.0
		End
		Part	:ToxicGas_Flash.part
		Part	:Toxic_Splash_Main.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	130

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.4 0.0
		End
		Part	:Toxic_Splash_Droplets.part
		Part	:SewerBubbleSplash_Spray.part
		Part	:SewerBubbleSplash_Splatter.part
		Sound	Sewer_SewerGeiser_Start_01 100 100 .8
		Lifespan 20
	End
End

Condition
	On	Time
	Time	135

	Event
		Type	Start
		At	Root
		Sound	Sewer_SewerGeiser_Start_01 100 100 .8
		Lifespan 20
	End
End

#############################################################

End
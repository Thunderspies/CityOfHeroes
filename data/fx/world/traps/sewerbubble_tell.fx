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
		Part	:Toxic_Ripples.part
		Lifespan 330
	End
End

Condition
	On	Time
	Time	30

	Event
		Ename	Steam
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:Sewer_Ripples_Small.part
		Lifespan 200
	End
End

Condition
	On	Time
	Time	100

	Event
		Ename	Steam
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	:Sewer_Bubbles_Small.fx
		Sound 	Sewer_SewerBubble_Start_01 100 100 .8
		Lifespan 100
	End
End

#Condition
#	On	Time
#	Time	210
#
#	Event
#		Type	Start
#		At	Root
#		Bhvr	:Toxic_Bubble_Grow.bhvr
#		Part	:Toxic_Bubble_Small.part
#		Part	:Toxic_Bubble_big.part
#		Sound	Sewer_SewerBubble_Expand_01 100 100 .8
#	End
#
#	Event
#		Type	Start
#		At	Root
#		Part	:Toxic_Ripple_Big.part
#	End
#End
#
#Condition
#	On	Time
#	Time	328
#
#	Event
#		Type	Start
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 0.2 0.0
#		End
#		Part	:Toxic_Splash_Main.part
#		Lifespan 20
#	End
#End
#
#Condition
#	On	Time
#	Time	330
#
#	Event
#		Type	Start
#		At	Root
#		BhvrOverride
#			PositionOffset	0.0 0.4 0.0
#		End
#		Part	:Toxic_Splash_Droplets.part
#		Part	:SewerBubbleSplash_Spray.part
#		Part	:SewerBubbleSplash_Splatter.part
#		Lifespan 20
#	End
#End

#############################################################

End
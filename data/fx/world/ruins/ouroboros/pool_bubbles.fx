#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Toxic_Bubbles_Small.part
		Part	:Toxic_Ripples_Small.part
	End
End

Condition
	On	Cycle
	Time	6

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	30.0 0.0 30.0
		End
		ChildFX	:Child_Bubbles.fx
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time	24

	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound SludgeWater_Bubble_01 100 90 .8	
		Sound SludgeWater_Bubble_02 100 90 .8	
		Sound SludgeWater_Bubble_03 100 90 .8	
		Sound SludgeWater_Bubble_04 100 90 .8	
		Sound SludgeWater_Bubble_05 100 90 .8	
		Sound SludgeWater_Bubble_06 100 90 .8	
		Sound SludgeWater_Bubble_07 100 90 .8	
		Lifespan 30
	End
End

#############################################################

End
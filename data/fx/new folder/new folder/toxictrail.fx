#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble4.bhvr
		BhvrOverride
			PositionOffset		0 0.3 0
			Scale			0.01 0.01 0.01
			ScaleRate		1.5 1.5 1.5
 			EndScale		2.25 2.25 2.25
		End
	#	Part	:Toxic_Steam_Ring.part
	#	Lifespan 10
	End

	Event
		EName	PuddleAnchor
		Type	Local
		At	Root
		Part	:Toxic_Burn_Flat.part
		Part	:Toxic_Rings_Flat.part
	#	Lifespan 250
	End


	Event
		Type	Local
		At	PuddleAnchor
		BhvrOverride
			PositionOffset	0.0 -2.0 0.0
		End
		Part	:Toxic_Steam.part
	#	Lifespan 250
	End
End

Condition
	On	Cycle
	Time	6

	Event
		Type	Local
		At	PuddleAnchor
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
			StartJitter	3.0 0.0 3.0
		End
		ChildFX	:Child_Toxic_Bubbles.fx
		Lifespan 30
	End
End

#############################################################

End
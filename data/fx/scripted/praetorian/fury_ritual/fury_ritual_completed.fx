#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################
Condition
	On	Time
	Time	0


	Event
		EName 	MainAnchor
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 0 -3
		End
	End

	Event
		EName 	Crows_Initial
		Type	Start
		At	MainAnchor
		BhvrOverride
			PositionOffset	0 10 0
		End
		ChildFX	:CHILD_Crows_FlyOut.fx
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PyrRotate	-90 0 0
			PositionOffset	0 10 5
		End
		Part	:Glow_Beam.part
		Part	:Glow_Explode.part
		Part	:Glow.part
		Part	:Glow2.part
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PyrRotate	-90 0 0
			PositionOffset	0 0 -10
		End
		Part	:Glow.part
	End
End

Condition
	On 	Cycle
	Time 	2

	Event
		EName 	Prime
		Type	Start
		At	MainAnchor
		BhvrOverride
			PositionOffset	-2 10 0
			PYRRotate	-30 0 0
		End
		ChildFX	:CHILD_Crows.fx
		Lifespan 80
	End
End

#############################################################

End
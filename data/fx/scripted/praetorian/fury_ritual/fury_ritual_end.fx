#############################################################
## MurderOfCrows_TeleportIn.bhvr
#############################################################

FxInfo
Lifespan 120

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
			PositionOffset	0 30 -3
		End
	End

	Event
		EName 	Crows_Initial
		Type	Start
		At	MainAnchor
		BhvrOverride
			PositionOffset	0 10 0
		End
		ChildFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\Shrine_TeleportIn.fx
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PyrRotate	-90 0 0
			PositionOffset	0 10 5
		End
		Part	:Glow_Beam_Explode.part
	End
End

Condition
	On	Time
	Time	20

	Event
		EName 	Crows_Initial
		Type	Start
		At	MainAnchor
		BhvrOverride
			PositionOffset	0 10 0
		End
		ChildFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\Shrine_TeleportIn.fx
	End
End

Condition
	On	Time
	Time	40

	Event
		EName 	Crows_Initial
		Type	Start
		At	MainAnchor
		BhvrOverride
			PositionOffset	0 10 0
		End
		ChildFX	Scripted\Praetorian\Murder_Of_Crows_Teleport\Shrine_TeleportIn.fx
	End
End


#############################################################

End
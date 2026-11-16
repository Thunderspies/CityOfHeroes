#############################################################
## Binary_Body.fx
#############################################################

FxInfo

Condition
	On	Time
	Time 	0

	Event
		EName 	Anchor
		Type	Local
		At	Root
		Part	:LightBlobs.part
		Part	:DataBeamsUp.part
		#part	:DataStringsUp01.part
		BhvrOverride
			PositionOffset	0.0 3 0
		End
	End

	Event
		Type	PositOnly
		At	Hips
		BhvrOverride
			PositionOffset	0.0 0 0
		End
		ChildFX	:Child_DataStrings.fx
		#part	:DataStringsUp01.part
	End

	Event
		Type	Local
		At	Root
		Part	:LightBlobs02.part
		BhvrOverride
			PositionOffset	0 2 0.5
		End
	End

	Event
		Type	Local
		At	Root
		part	:DataStringsUp02.part
		BhvrOverride
			PositionOffset	0 2.5 0
		End
	End

	Event
		Type	Local
		At	Root
		Part	:DataBeamsUp02.part
		BhvrOverride
			PositionOffset	0 2 0
		End
	End

End
#########################################################
End
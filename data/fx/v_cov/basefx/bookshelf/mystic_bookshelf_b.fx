#########################################################
##	Arcane Battery 01
########################################################
FxInfo

##########################  Large Glitter #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 8 0
			pyrRotate	-90 0 0
		End
						
	End

	Event
		EName	Side1A
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 1.2 0
			
		End
		Part	:Mystic_Knowledge.part

	End

	Event
		EName	Side2A
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 -1.6 0
			
		End
		Part	:Mystic_Knowledge.part

	End

	Event
		EName	Side1B
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 1.25 0
			
		End
		Part	:Mana_Sparkles_01.part

	End

	Event
		EName	Side2B
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 -1.65 0
			
		End
		Part	:Mana_Sparkles_01.part

	End

	Event
		EName	Spine
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			pyrRotate	0 0 -90
		End
						
	End

	
	Event
		EName	Side1C
		Type	Local
		At	Spine
		BhvrOverride
			PositionOffset	0 9.4 .5
		End
		Part	:Spine_Glow.part

	End

	Event
		EName	Side2C
		Type	Local
		At	Spine
		BhvrOverride
			PositionOffset	0 -9.8 .5
		End
		Part	:Spine_Glow.part

	End

End


End
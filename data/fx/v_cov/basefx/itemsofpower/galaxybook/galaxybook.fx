#########################################################
##	book
########################################################
FxInfo

########################## galaxy #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 11 1
			pyrRotate	-90 0 0
		End
						
	End


	Event
		EName	Spinner
		Type	Local
		At	center
		BhvrOverride
			PositionOffset	0 0 0
			Spin		0 .01 0
		End	
		Part	:MagicSparkles.part
		Part	:RadiantCore.part
		Part	:RadiantFringe.part
		Part	:GlowWisps.part
	End


	Event
		EName	RightArm
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset	1 0 0
			pyrRotate	0 0 0
		End
		Part	:Penumbra.part
		Part	:PenumbraGlow.part
						
	End

	Event
		EName	LeftArm
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset	-1 0 0
			pyrRotate	0 180 0
		End
		Part	:Penumbra.part
		Part	:PenumbraGlow.part
						
	End


	Event
		EName	InnerSpinner
		Type	Local
		At	Spinner
		BhvrOverride
			PositionOffset	0 0 0
			Spin		0 .01 0
		End
		Part	:PenumbraDark.part
		Part	:PenumbraCore.part
		Part	:RadiantPower.part						
	End


End
###############################################################


End
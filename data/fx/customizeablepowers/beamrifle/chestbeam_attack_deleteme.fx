#############################################################
## ChestBeam_Attack.fx
#############################################################

FxInfo
Lifespan 60

#############################################################
#
#Condition
#	On 	Time
#	Time	0
#	Event
#		Type	Local
#		At	Root
#		SoundNoRepeat 3
#		Sound 	Shivan_ChestBeam_Attack_01 100 50 .8
#		Sound 	Shivan_ChestBeam_Attack_02 100 50 .8
#		Sound 	Shivan_ChestBeam_Attack_03 100 50 .8
#		Sound 	Shivan_ChestBeam_Attack_04 100 50 .8
#		Sound 	Shivan_ChestBeam_Attack_05 100 50 .8
#		Sound 	Shivan_ChestBeam_Attack_06 100 50 .8
#	End
#
#	Event
#		Type	Local
#		At	Root
#		SoundNoRepeat 2
#		Sound 	Shivan_ChestVOX_01 30 30 .8
#		Sound 	Shivan_ChestVOX_02 30 30 .8
#		Sound 	Shivan_ChestVOX_03 30 30 .8
#		Sound 	Shivan_ChestVOX_04 30 30 .8
#	End
#
#End



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0 -0.5 0.0
		End
		#Part	:Beam_Highlights_Small.part
		Part	:Beam_Small.part
		Part	:Beam_Small_Additive.part
		#Part	:Base_Splash_Middle.part
		#Part	:Base_Splash_Small.part
		#Part	:Base_Splash_Core_Small.part
		Lifespan	3
		POther	T_Chest
		LookAt	T_Chest
	End



End

#############################################################

End